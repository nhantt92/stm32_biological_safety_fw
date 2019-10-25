#include "ir_decode.h"
#include "main_screen.h"
#include "menu.h"
#include "buzzer.h"
#include "system.h"
#include "config.h"

const uint16_t IRAddr = 0x00ff;
NEC_Frame_TypeDef necFrame;
uint16_t IRCmd;

/* Private define ------------------------------------------------------------*/
#define   MIN_BIT_START_TIME_MS       12  /* Minimum Full bit start time in ms */
#define   MAX_BIT_START_TIME_MS       14  /* Maximum Full bit start time in ms */

#define   MIN_BIT_HIGH_TIME_MS         2 /* Minimum Full bit 1 time in ms */
#define   MAX_BIT_HIGH_TIME_MS         3 /* Maximum Full bit 1 time in ms */

#define   MIN_BIT_LOW_TIME_MS         1 /* Minimum Full bit 0 time in ms */
#define   MAX_BIT_LOW_TIME_MS         2 /* Maximum Full bit 0 time in ms */


TIM_HandleTypeDef htim3;

/* Private function prototypes -----------------------------------------------*/
static void NEC_RCC_Configuration(void);
static void NEC_GPIO_Configuration(void);
static void NEC_NVIC_Configuration(void);
static uint32_t NEC_TIM_GetCounterCLKValue(void);
static void NEC_WaitForNextFallingEdge(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the NEC reception.
  * @param  None
  * @retval None
  */
void NEC_Receiver_Init(void)
{
  necFrame.startBit = 0;
  necFrame.cnt = 0;
  necFrame.cntTmp = 0;
  necFrame.cntData = 0;
  necFrame.dataRev = 0;
  /* System Clocks Configuration for NEC reception */
  NEC_RCC_Configuration();

  /* NVIC configuration for NEC reception */
  NEC_NVIC_Configuration();

  /* Configure the GPIO port for NEC reception */
  NEC_GPIO_Configuration();

  
  NEC_WaitForNextFallingEdge();
}

/**
  * @brief  Configure the system to receive the next NEC frame.
  * @param  None
  * @retval None
  */

static void NEC_WaitForNextFallingEdge(void)
{
  /* Enable EXTI line x */
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 499;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  HAL_TIM_Base_Start_IT(&htim3);
}

/**
  * @brief  tim3 interrupt Callback 500us
  * @param  None
  * @retval None
  */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(++necFrame.cnt >= 50)
    necFrame.cnt = 0;
}

/**
  * @brief  Gpio_extx interrupt Callback.
  * @param  None
  * @retval None
  */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  NEC_MeasureFirstLowDuration();
}

/**
  * @brief  Measure the first low duration of the NEC frame.
  * @param  None
  * @retval None
  */
uint32_t remoteCmd;

void NEC_MeasureFirstLowDuration(void)
{
  /* If an edge is detected on NEC input pin */
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
  {
    necFrame.cntTmp = necFrame.cnt;
    necFrame.cnt = 0;
    //__HAL_TIM_SET_COUNTER(&htim3, 0);
    necFrame.cntData++;
    //HAL_TIM_Base_Start_IT(&htim3);
    if((necFrame.cntTmp >= MIN_BIT_START_TIME_MS * 2) & (necFrame.cntTmp <= MAX_BIT_START_TIME_MS * 2)) //Have start signal
    {
      necFrame.cntData = -1;
      necFrame.startBit = 1;
      necFrame.cntTmp = 0;
      necFrame.dataRev = 0;
    }
    else if((necFrame.startBit == 1) & (necFrame.cntData >= 0) & (necFrame.cntData <= 71)) //Start receive signal
    {
      if((necFrame.cntTmp >= MIN_BIT_HIGH_TIME_MS*2) & (necFrame.cntTmp <= MAX_BIT_HIGH_TIME_MS*2) & (necFrame.cntData % 2 != 0))
      {
        //printf("1");
        necFrame.dataRev |= (uint32_t)(1<<(31-(necFrame.cntData/2)));
        necFrame.cntTmp = 0;
      }
      else if((necFrame.cntTmp >= MIN_BIT_LOW_TIME_MS*2) & (necFrame.cntTmp <= MAX_BIT_LOW_TIME_MS*2) & (necFrame.cntData % 2 != 0))
      {
        //printf("0");
        necFrame.cntTmp = 0;
      }
      // else
      // {
      //   cnt_ms_tmp = 0;
      //   start_status = 0;
      //   cnt_data = 0;
      //   data_rev = 0;
      // }
    }
    else if((necFrame.startBit == 0) & (necFrame.cntTmp > MAX_BIT_START_TIME_MS*2)) //error detect start
    {
      necFrame.cntTmp = 0;
      necFrame.startBit = 0;
      necFrame.cntData = 0;
      necFrame.dataRev = 0;
    }
    if(necFrame.cntData == 61) //receive 32 bit
    {
      remoteCmd = necFrame.dataRev;
      necFrame.cmd = (uint16_t)necFrame.dataRev;
      necFrame.addr = (uint16_t)(necFrame.dataRev >> 16);
      if(necFrame.addr == IRAddr)
        IRCmd = necFrame.cmd;
      //printf("DataRev: %.08x\n", necFrame.dataRev);
    }
    if(necFrame.cntData == 62)
    {
      necFrame.cntData = 0;
      necFrame.dataRev = 0;
      necFrame.startBit = 0;
      necFrame.cntTmp = 0;
    }
  }
}
  
/**
  * @brief  Configures the different system clocks for NEC reception.
  * @param  None
  * @retval None
  */
static void NEC_RCC_Configuration(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/**
  * @brief  Configure the NEC input pin.
  * @param  None
  * @retval None
  */
static void NEC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  Configure the nested vectored interrupt controller for NEC reception.
  * @param  None
  * @retval None
  */
static void NEC_NVIC_Configuration(void)
{
  
  /* Enable the EXTIx global Interrupt */
  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  /* Enable the RC5_TIM global Interrupt */
  HAL_NVIC_SetPriority(TIM3_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void NEC_Manager(void)
{
  switch (IRCmd)
  {
  case POWER:
    toggle = 1;
    dev.status.socket = !dev.status.socket;
    Device_Save_Status();
    buzzer_short_beep();
    IRCmd = 0;
    // printf("REMOTE SOCKET\n");
    break;
  case LIST:
    toggle = 1;
    dev.status.lamp = !dev.status.lamp;
    Device_Save_Status();
    buzzer_short_beep();
    IRCmd = 0;
    // printf("REMOTE NEON\n");
    break;
  case BACK:
    toggle = 1;
    dev.status.uv = !dev.status.uv;
    dev.status.uv?UV_Init_Time(sys_cfg.uvMode):UV_Clear_Time();
    Device_Save_Status();
    buzzer_short_beep();
    IRCmd = 0;
    // printf("REMOTE UV\n");
    break;
  case HOME:
    toggle = 1;
    dev.status.fan = !dev.status.fan;
    Device_Save_Status();
    buzzer_short_beep();
    IRCmd = 0;
    // printf("REMOTE FAN\n");
    break;
  case PLAY:
    if(menuIdx == 0)
    {
      menuTimeOut = 60*100;
      menuIdx = 0x0100;
      MENU_ToEvt(menuIdx, 4);
    }
    else
    {
      MENU_ToEvt(menuIdx, 4);
    }
    if(menuTimeOut != 0)menuTimeOut--;
    if(menuTimeOut == 0)
    {
      menuIdx = 0;
    }
    IRCmd = 0;
    break;
  case UP:
    if(menuIdx != 0)
    {
      if((menuIdx == 0x0100)||(menuIdx == 0x0200)||(menuIdx == 0x0300)) //||(menuIdx == 0x0400)
      {
        if(--menuIdxCh == 0)
        {
          menuIdxCh = 4;
          menuIdx -= 0x0100;
          if(menuIdx < 0x0100)
          {
            menuIdx = 0x0400;
            menuIdxCh = 1;
          }
        }
      }
     else if(menuIdx == 0x0400)
      {
        if(--menuIdxCh == 0)
        {
          menuIdxCh = 4;
          menuIdx = 0x0300;
        }
        //printf("idx: %.4x - IdxCh: %d\n", menuIdx, menuIdxCh);
      }
      MENU_ToEvt(menuIdx, 0);
    }
    IRCmd = 0;
    break;
  case DOWN:
    if(menuIdx != 0)
    {
      if((menuIdx == 0x0100)||(menuIdx == 0x0200)||(menuIdx == 0x0300)) //||(menuIdx == 0x0400)
      {
        if(++menuIdxCh > 4)
        {
          menuIdxCh = 1;
          menuIdx += 0x0100;
          if(menuIdx > 0x0400)menuIdx = 0x0100;
        }
      }
      else if(menuIdx == 0x0400)
      {
        if(++menuIdxCh > 1)
        {
          menuIdxCh = 1;
          menuIdx = 0x0100;
        }
        printf("idx: %.4x - IdxCh: %d\n", menuIdx, menuIdxCh);
      }
      MENU_ToEvt(menuIdx, 1);
    }
    IRCmd = 0;
    break;
  case PREV:
    toggle = 0;
    if((menuIdx != 0x0100)&&(menuIdx != 0x0200)&&(menuIdx != 0x0300)&&(menuIdx != 0x0400))
    {
      menuIdx = (menuIdx&0xFF00);
    }
    else
    {
      menuIdx = 0;
      menuIdxCh = 1;
    }
    MENU_ToEvt(menuIdx, 2);
    IRCmd = 0;
    break;
  case NEXT:
    toggle = TIME_PAGE;
    if((menuIdx == 0x0100)||(menuIdx == 0x0200)||(menuIdx == 0x0300)||(menuIdx == 0x0400))
    {
      menuIdx = menuIdx+menuIdxCh;
      menuTimeNext = 200;
      MENU_ToEvt(menuIdx, 0xFF);
    }
    else
    {
      if(menuTimeNext == 0)
        MENU_ToEvt(menuIdx, 3);
    }
    IRCmd = 0;
    break;
  default:
    break;
  }
}
