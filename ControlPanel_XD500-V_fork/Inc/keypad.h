#include "stdbool.h"


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
