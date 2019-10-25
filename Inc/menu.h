#ifndef __MENU_H
#define __MENU_H

#include "main.h"

#define		UP_PIN			GPIO_PIN_9
#define 	DOWN_PIN 		GPIO_PIN_2
#define 	NEXT_PIN		GPIO_PIN_9
#define 	PREV_PIN		GPIO_PIN_0
#define 	MENU_PIN 		GPIO_PIN_1

#define  	Up_In()			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)
#define  	Down_In()		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)
#define  	Next_In()		HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)
#define  	Prev_In()		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define		Menu_In()		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)

#define  SAMPLE_KEY              5

typedef enum {
	BTN_UP,
	BTN_DOWN,
    BTN_PREV,
    BTN_NEXT,
    BTN_MENU
} BTN_NAME;

typedef enum {
	BTN_PRESS,
	BTN_RELEASE
} BTN_EVENT;

typedef uint16_t (*MenuHandler)(uint8_t add, uint16_t key);
typedef void (*KeyHandler)(uint8_t key);

typedef struct _TREE_VIEW {
	uint16_t keyMode;
	const uint8_t display[90];
	// const uint8_t display2[90];
	// const uint8_t display3[90];
	// const uint8_t display4[90];
	MenuHandler handler;
	const struct _TREE_VIEW *parrent;
} TREE_VIEW;

typedef struct {
	KeyHandler PressedEventHandler;
	KeyHandler ReleasedEventHandler;
	uint8_t pressed;
    uint8_t press;
    uint8_t waitRelease;
    uint8_t cnt;       
} BUTTON;

extern const TREE_VIEW treeView[];
extern BUTTON keyPress[5];
extern uint16_t menuIdx;
extern uint8_t menuIdxCh;
extern uint16_t menuValueSet;
extern uint16_t menuIdxLast;
extern uint16_t menuCursor;
extern uint16_t menuTimePress;
extern uint16_t menuTimeOut;
extern uint16_t menuTimeNext;

void BUTTON_Get(uint8_t port_id);
void BUTTON_Manage(void);
void BUTTON_Init(void);
void MENU_Task(uint8_t key);
void MENU_CheckTime(void);
void MENU_Manager(uint8_t key);
#endif
