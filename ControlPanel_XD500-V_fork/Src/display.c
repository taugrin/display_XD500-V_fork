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

void DrawMenu(void);

//
// Global Variables
//


//
// Local Variables
//
uint16_t ReadData[64];

int16_t currentSelection = 0; // Индекс выбранного пункта в текущем уровне
int16_t currentLevelStart = 0; // Индекс первого пункта текущего уровня


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

	// отрисовка строки статуса
	StatusBarDraw();


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
	char CharArray[12];

	ST7565_drawline(0, 10, 128, 10, 1);

	// Вывод номера преобразователя
	UsbReadData(MODBUSADR_ADR, 1, ReadData);
	my_itoa(ReadData[0], CharArray);
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 0, CharArray);

	// Вывод LOCAL/REMOTE
	UsbReadData(LOC_REM_ADR, 1, ReadData);
	if (ReadData[0] == 0) {ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 3, 0, "R");}
	else {ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 3, 0, "L");}

	// Вывод сигнала Fault
	//if (StatusWord.bit.fault)
	//{
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 5, 0, "E");
		uint16_to_hex_str(FaultWord.all, CharArray, 2/*4*/);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 6, 0, CharArray); // значение FaultWord в формате hex, 2 символа
	//}

	// Вывод сигнала Alarm
	//if (StatusWord.bit.alarm)
	//{
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 9, 0, "A");
		uint16_to_hex_str(AlarmWord.all, CharArray, 2);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 10, 0, CharArray); // значение AlarmWord в формате hex, 2 символа
	//}

	// Вывод направления вращения
	if (StatusWord.bit.dir) {ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 13, 0, "<-");}
	else {ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 13, 0, "->");}

	// Вывод задания частоты
	UsbReadData(FREQ_REFERENCE_ADR, 1, ReadData);
	ftoa((float)(ReadData[0])/10.0, CharArray, 1);
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 16, 0, CharArray);

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* MonitorScreenDraw - отрисовка экрана Монитор
*/
void MonitorScreenDraw(void)
{

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
	//ST7565_drawstring(20, 3, "Экран Настройки");

	DrawMenu();

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* DrawMenu - отображает на экране 3 пункта текущего уровня в зависимости от выбранного пункта
*/
void DrawMenu(void)
{
    int16_t itemIdx = currentLevelStart; // индекс начала меню

    uint8_t BeginMenuItem = (currentSelection / 3)*3; // верхний пункт меню на экране в зависимости от выбранного
    uint8_t EndMenuItem = BeginMenuItem + 3; // нижний пункт меню на экране в зависимости от выбранного

    // определяю индекс меню для верхнего пункта на экране
    for(uint8_t i = 0; i < BeginMenuItem; i++)
    {
    	itemIdx = menuItems[itemIdx].nextIdx;
    }

    // отрисовываю три пункта меню на экране в зависимости от выбранного пункта
    uint8_t j = 2;
    for(uint8_t i = BeginMenuItem; i < currentLevelCount && i < EndMenuItem; i++)
    {
        // Отображение названия пункта
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, j, menuItems[itemIdx].name);
        j+=2;

        // Выделение текущего выбранного пункта
        if(i == currentSelection)
        {
        	ST7565_inv_fillrect(0, 15 + (16 * (i % 3)), 128, 9, 1);
        }

        // переписываю индекс для следующего пункта меню
        itemIdx = menuItems[itemIdx].nextIdx;
    }

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
