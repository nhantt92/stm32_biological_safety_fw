/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "iwdg.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "u8g2.h"
#include "key.h"
#include "logo.h"
#include "main_screen.h"
#include "info_screen.h"
#include "output.h"
#include "input.h"
#include "clock_rtc.h"
#include "buzzer.h"
#include "spi.h"
#include "ir_decode.h"
#include "menu.h"
#include "system.h"
#include "sm5852.h"

int _write(int file, char *data, int len);

#define LCD_RST_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define LCD_RST_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define LCD_RS_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_RS_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define USB_EN() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

void SystemClock_Config(void);
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

u8g2_t u8g2;
SM5852_T sm5852_1;
SM5852_T sm5852_2;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_IWDG_Init();
  // MX_USART1_UART_Init();
  // MX_USART2_UART_Init();
  sm5852_1.i2cHandle.Instance = I2C1;
  sm5852_2.i2cHandle.Instance = I2C2;
  SM5852_Init(&sm5852_1);
  SM5852_Init(&sm5852_2);
  NEC_Receiver_Init();
  MX_USB_DEVICE_Init();
  KeyInit();
  u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8g2_gpio_and_delay_stm32); // init u8g2 structure
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display

  LCD_RST_0;
  HAL_Delay(10);
  LCD_RST_1;

  Logo();
  HAL_Delay(500);
  BUTTON_Init();
  MX_TIM1_Init();
  Output_Init();
  Input_Init();
  buzzer_init();
  RTC_Init();
  System_Init();
  uint32_t timeRefesh = HAL_GetTick();
  uint32_t tick = HAL_GetTick();
  u8g2_ClearBuffer(&u8g2);
  while (1)
  {
    /* Debug measure pressure */
    // if(HAL_GetTick() - tick > 1000)
    // {
    //   printf("Pressure1: %.2f\n", sm5852_1.pressure);
    //   printf("Temp1: %.2f\n", sm5852_1.temp);
    //   printf("Pressure2: %.2f\n", sm5852_2.pressure);
    //   printf("Temp2: %.2f\n", sm5852_2.temp);
    //   tick = HAL_GetTick();
    // }
    /* Init meansure Time exec program */
    //uint32_t time = HAL_GetTick();
    System_Manager();
    SM5852_Manager(&sm5852_1);
    SM5852_Manager(&sm5852_2);
    NEC_Manager();
    BUTTON_Manage();
    KeyManage();
    Output_Manage();
    Input_Manage();
    handle_buzzer();
    /* print meansure Time exec program */
    //printf("Time ex: %d\n", HAL_GetTick() - time);
    HAL_IWDG_Refresh(&hiwdg);
    if (HAL_GetTick() - timeRefesh > 100)
    {
      u8g2_SendBuffer(&u8g2);
      timeRefesh = HAL_GetTick();
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_RTC;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
    //Function to define the logic level of the RESET line
    case U8X8_MSG_GPIO_RESET:
      if (arg_int) LCD_RST_1;
      else LCD_RST_0;
      break;
    default:
      return 0; //A message was received which is not implemented, return 0 to indicate an error
  }

  return 1; // command processed successfully.
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t byte;
  uint8_t *data;

  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;
    while (arg_int > 0)
    {
      byte = *data;
      data++;
      arg_int--;
      HAL_SPI_Transmit(&hspi1, &byte, 1, 100);
    }
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    //HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    LCD_RS_1;
    __NOP(); // 21 ns
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    __NOP(); // 21 ns
    //HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    LCD_RS_0;
    break;
  default:
    return 0;
  }
  return 1;
}

void Error_Handler(void)
{
  printf("Error!");
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */

int _write(int file, char *data, int len)
{
  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
  {
    errno = EBADF;
    return -1;
  }
  uint8_t status = CDC_Transmit_FS((uint8_t *)data, len);
  return status;
}