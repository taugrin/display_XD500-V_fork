#include "display.h"

#include "usertasks.h"
#include "usb_host.h"
#include "usb_user.h"
#include "usb_funcs.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "lcd.h"
#include "dwt_stm32_delay.h"
#include "ST7565.h"
#include "usb_host.h"


SemaphoreHandle_t xButtonUpSemaphore = NULL;
SemaphoreHandle_t xButtonDownSemaphore = NULL;
SemaphoreHandle_t xButtonEnterSemaphore = NULL;
SemaphoreHandle_t xButtonFuncSemaphore = NULL;
SemaphoreHandle_t xButtonResetSemaphore = NULL;
SemaphoreHandle_t xButtonRunSemaphore = NULL;
SemaphoreHandle_t xButtonStoptSemaphore = NULL;


void vTaskDisplay(void const * argument)
{
    DWT_Delay_Init(); // инициализация счетчика тактов DWT_CYCCNT
	ST7565_st7565_init(); // инициализация дисплея

	ST7565_clear();
	ST7565_StartImage(); // запись стартовой картинки в массив
	ST7565_display(); // вывод картинки на экран
	//enablePartialUpdate = true;

	MX_USB_HOST_Init(); // инициализация usb

	vTaskDelay(5000); // Ожидание готовности usb. В это же время выводится стартовая картинка.

	uint8_t i; // отправляю те самые 150 запросов, после которых USB падает и переинициализируется
	uint16_t tmpInitBuf[2];
	for (i = 0; i < 151; i++) {UsbReadData(0x4200, 1, tmpInitBuf);} // всё, дальше USB падать не должно

	// инициализация корневого меню
	InitBasicMenu();

    //RefInit(); // считывание задания пульта из eeprom


    for (;;)
    {
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //ToDo: Отладка! Начало задачи дисплея.

    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //ToDo: Отладка!
        ST7565_clear();
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //ToDo: Отладка!

        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //ToDo: Отладка!
		DisplayStatic(); // моя функция
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //ToDo: Отладка!

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //ToDo: Отладка!
        ST7565_display();
        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //ToDo: Отладка!

        	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //ToDo: Отладка! Конец задачи дисплея.
		vTaskDelay(2);

    }

}


//void vTaskKeysCheck(void *pvParameters)
//{
//
//
//    key_cont_t countUp = {0,0};
//    key_cont_t countDown = {0,0};
//    key_cont_t countFunc = {0,0};
//    key_cont_t countReset = {0,0};
//    key_cont_t countRun = {0,0};
//    key_cont_t countStop = {0,0};
//    key_cont_t countEnter = {0,0};
//
//
//
//    xButtonUpSemaphore = xSemaphoreCreateBinary();
//    xButtonDownSemaphore = xSemaphoreCreateBinary();
//    xButtonEnterSemaphore = xSemaphoreCreateBinary();
//    xButtonFuncSemaphore = xSemaphoreCreateBinary();
//    xButtonResetSemaphore = xSemaphoreCreateBinary();
//    xButtonRunSemaphore = xSemaphoreCreateBinary();
//    xButtonStopSemaphore = xSemaphoreCreateBinary();
//
//    void ChekKey(uint8_t state, SemaphoreHandle_t xButton, key_cont_t* count)
//    {
//        if (state)
//        {
//            count->Push++;
//
//        } else
//        {
//            if (count->Push >= 2)
//            {
//                count->Pull++;
//                if (count->Pull >= 20)
//                {
//                    count->Pull = 0;
//                    count->Push = 0;
//                    xSemaphoreGive(xButton);
//
//                }
//
//            }
//
//        }
//        if (count->Push >= 68)
//        {
//            count->Pull = 0;
//            count->Push = 0;
//            xSemaphoreGive(xButton);
//        }
//
//    }
//
//    //check if semaphores were created successfully
//    if        ((xButtonUpSemaphore != NULL)
//            && (xButtonDownSemaphore != NULL)
//            && (xButtonEnterSemaphore != NULL)
//            && (xButtonFuncSemaphore != NULL)
//            && (xButtonResetSemaphore != NULL)
//            && (xButtonRunSemaphore != NULL)
//            && (xButtonStopSemaphore != NULL))
//    {
//        //successfully created
//        //resets initial semaphores to 0
//        xSemaphoreTake(xButtonUpSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonDownSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonEnterSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonFuncSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonResetSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonRunSemaphore, (portTickType)0);
//        xSemaphoreTake(xButtonStopSemaphore, (portTickType)0);
//
//    } else
//    {
//        //send error of failure
//    }
//
//    for (;;)
//    {
//
//
//        ChekKey(KEY_FUNC, xButtonFuncSemaphore, &countFunc);
//
//        ChekKey(KEY_UP, xButtonUpSemaphore, &countUp);
//
//        ChekKey(KEY_DOWN, xButtonDownSemaphore, &countDown);
//
//        ChekKey(KEY_ENTER, xButtonEnterSemaphore, &countEnter);
//
//        ChekKey(KEY_RESET, xButtonResetSemaphore, &countReset);
//
//        ChekKey(KEY_RUN, xButtonRunSemaphore, &countRun);
//
//        ChekKey(KEY_STOP, xButtonStopSemaphore, &countStop);
//
//        vTaskDelay(4);
//    }
//}

