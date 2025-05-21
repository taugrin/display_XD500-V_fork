//
// Included Files
//
#include "display.h"

//
// Function Prototypes
//
int8_t CheckKeySem(SemaphoreHandle_t semaphore);

void StatusBarDraw(void);

void MonitorScreenDraw(void);

void ReferenceScreenDraw(void);

void SettingsScreenDraw(void);

//
// Global Variables
//


//
// Local Variables
//
uint16_t ReadData[64];


//--------------------------------------------------------------------
/*
* DisplayStatic - функция обработки связи и отображения данных на экране
*/
void DisplayStatic(void)
{

	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextScreen();
	}

	// Вычитываю SW, SW1, FW, FW1, AW.
	UsbReadData(0x4200, 5, ReadData);
	StatusWord.all = ReadData[0];
	StatusWord1.all = ReadData[1];
	FaultWord.all = ReadData[2];
	FaultWord1.all = ReadData[3];
	AlarmWord.all = ReadData[4];


	// отрисовка экранов
	switch (MainScreen)
	{
	case MonitorScreen:
		MonitorScreenDraw();
		break;

	case ReferenceScreen:
		ReferenceScreenDraw();
		break;

	case SettingsScreen:
		SettingsScreenDraw();
		break;
	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* CheckKey - возвращает значение бинарного семафора кнопки
*/
int8_t CheckKeySem(SemaphoreHandle_t semaphore)
{
    int8_t res;

    if (semaphore != NULL)
	{
        if (xSemaphoreTake(semaphore, (portTickType)0) == pdTRUE) {res = 1;}
        else {res = 0;}
	}
    else {res = -1;}

    return res;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* StatusBar - отрисовка строки статуса
*/
void StatusBarDraw(void)
{
	ST7565_drawline(0, 10, 128, 10, 1);

	//Вывод номера преобразователя//
	char modbusADR[12];
	UsbReadData(MODBUSADR_ADR, 1, ReadData);
	my_itoa(ReadData[0], modbusADR);
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 0,  modbusADR);

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* MonitorScreenDraw - отрисовка экрана Монитор
*/
void MonitorScreenDraw(void)
{
	StatusBarDraw();

	ST7565_drawstring(20, 3, "Экран Монитор");
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* ReferenceScreenDraw - отрисока экрана Задание
*/
void ReferenceScreenDraw(void)
{


	ST7565_drawstring(20, 3, "Экран Задание");
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* SettingsScreenDraw - отрисовка экрана Настройки
*/
void SettingsScreenDraw(void)
{


	ST7565_drawstring(20, 3, "Экран Настройки");
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------
