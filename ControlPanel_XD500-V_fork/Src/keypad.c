#include "stdbool.h"
#include "display.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "usertasks.h"

#include "keypad.h"

uint16_t delay_time_max = 2;

void KeyDeBounce(uint8_t state, uint16_t delay_time_max, pkey_t* Key)
{
    Key->read = state;
    Key->P_rise = false;
    Key->P_fall = false;
    if (Key->read != Key->state)
    {
        Key->deBouncCount++;
        if (Key->deBouncCount >= delay_time_max)
        {
            Key->state = Key->read;
            if (Key->state == true)
            {
                Key->P_rise = true;
                Key->P_fall = false;
            } else
            {
                Key->P_rise = false;
                Key->P_fall = true;
            }
        }
    } else
    {
        Key->deBouncCount = 0;
    }
}

void KeyChekUp_AutoScroll(uint8_t state, pkey_t* Key, SemaphoreHandle_t xButton)
{

    KeyDeBounce(state, delay_time_max, Key);

    if (Key->state == true)
    {
        Key->pushCount++;
        if (Key->pushCount >= 120)
        {
            Key->P_rise = true;
            Key->P_fall = false;
            Key->pushCount = 100;
        }

    } else
    {
        Key->pushCount = 0;
    }

    if (Key->P_rise == true)
    {
        xSemaphoreGive(xButton);
    }
}

void KeyChekUp_LongPush(uint8_t state, pkey_t* Key, SemaphoreHandle_t xButtonRes, SemaphoreHandle_t xButtonAlarmRes)
{

    KeyDeBounce(state, delay_time_max, Key);

    if (Key->state == true)
    {
        Key->pushCount++;
        if (Key->pushCount >= 500)
        {
            xSemaphoreGive(xButtonAlarmRes);
            Key->pushCount = 0;
        }

    } else
    {
        Key->pushCount = 0;

    }

    if (Key->P_rise == true)
    {
        xSemaphoreGive(xButtonRes);
    }

}

void KeyChekUp_RiseFront(uint8_t state, pkey_t* Key, SemaphoreHandle_t xButton)
{

    KeyDeBounce(state, delay_time_max, Key);

    if (Key->P_rise == true)
    {
        xSemaphoreGive(xButton);
    }
}

void KeyChekUp_FallFront(uint8_t state, pkey_t* Key, SemaphoreHandle_t xButton, SemaphoreHandle_t xButton_alarm)
{

    KeyDeBounce(state, delay_time_max, Key);

    if (Key->state == true)
    {
        if (Key->pushCount < 65535)
        {
            Key->pushCount++;
        } else
        {
            Key->pushCount = 0;
        }

    }

    if (Key->P_fall == true)
    {
        if (Key->pushCount < 350)
        {

            xSemaphoreGive(xButton);
            Key->pushCount = 0;
        } else
        {
            xSemaphoreGive(xButton_alarm);
            Key->pushCount = 0;
        }
    }
}

void KeyFunction(keypad_t* pKeyPad)
{

    KeyChekUp_RiseFront(KEY_FUNC, &pKeyPad->Func, xButtonFuncSemaphore);
    KeyChekUp_AutoScroll(KEY_UP, &pKeyPad->Up, xButtonUpSemaphore);
    KeyChekUp_AutoScroll(KEY_DOWN, &pKeyPad->Down, xButtonDownSemaphore);
    KeyChekUp_RiseFront(KEY_ENTER, &pKeyPad->Enter, xButtonEnterSemaphore);
    KeyChekUp_LongPush(KEY_RESET, &pKeyPad->Reset, xButtonResetSemaphore, xButtonResetAlarmsSemaphore);
    KeyChekUp_RiseFront(KEY_RUN, &pKeyPad->Run, xButtonRunSemaphore);
    KeyChekUp_RiseFront(KEY_STOP, &pKeyPad->Stop, xButtonStopSemaphore);

}
