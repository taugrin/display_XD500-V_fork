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
#include "eeprom.h"


SemaphoreHandle_t xButtonUpSemaphore = NULL;
SemaphoreHandle_t xButtonDownSemaphore = NULL;
SemaphoreHandle_t xButtonEnterSemaphore = NULL;
SemaphoreHandle_t xButtonFuncSemaphore = NULL;
SemaphoreHandle_t xButtonResetSemaphore = NULL;
SemaphoreHandle_t xButtonRunSemaphore = NULL;
SemaphoreHandle_t xButtonStoptSemaphore = NULL;

void InitMonitorSel(void);


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

	// инициализация переменных
	ControlWord.all = 0; ControlWordOld.all = 0;

	// инициализация корневого меню
	InitBasicMenu();

	// инициализация переменных мониторинга
	InitMonitorSel();

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

void InitMonitorSel(void)
{
	uint8_t tmpMonitorSel;

	// Монитор 1
	tmpMonitorSel = readMonitorSel(0);
	if ((tmpMonitorSel >= 0) && (tmpMonitorSel < MonitorCnt)) {MonitorSelect[0] = tmpMonitorSel;}

	// Монитор 2
	tmpMonitorSel = readMonitorSel(1);
	if ((tmpMonitorSel >= 0) && (tmpMonitorSel < MonitorCnt)) {MonitorSelect[1] = tmpMonitorSel;}

	// Монитор 3
	tmpMonitorSel = readMonitorSel(2);
	if ((tmpMonitorSel >= 0) && (tmpMonitorSel < MonitorCnt)) {MonitorSelect[2] = tmpMonitorSel;}
}
