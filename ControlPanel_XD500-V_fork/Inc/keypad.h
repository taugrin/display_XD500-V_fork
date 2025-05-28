#include "stdbool.h"

#define KEY_FUNC  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)==GPIO_PIN_RESET
#define KEY_UP  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)==GPIO_PIN_RESET
#define KEY_DOWN    HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)==GPIO_PIN_RESET
#define KEY_ENTER HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET
#define KEY_RESET HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)==GPIO_PIN_RESET
#define KEY_RUN   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)==GPIO_PIN_RESET
#define KEY_STOP  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15)==GPIO_PIN_RESET

typedef struct
{
    _Bool    read;
    _Bool    state;
    _Bool    P_rise;
    _Bool    P_fall;
    uint16_t deBouncCount;
    uint16_t pushCount;

} pkey_t;

typedef struct
{
    pkey_t Start;
    pkey_t Stop;
    pkey_t Run;
    pkey_t Reset;
    pkey_t Up;
    pkey_t Down;
    pkey_t Func;
    pkey_t Enter;

} keypad_t;



void KeyChekUp_AutoScroll (uint8_t state,  pkey_t* Key,  SemaphoreHandle_t xButton);
void KeyChekUp_RiseFront (uint8_t state,  pkey_t* Key,  SemaphoreHandle_t xButton);
void KeyFunction (keypad_t* pKeyPad);
