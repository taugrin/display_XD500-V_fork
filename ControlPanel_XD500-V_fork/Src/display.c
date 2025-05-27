//
// Included Files
//
#include "display.h"

//
// Function Prototypes
//
int8_t CheckKeySem(SemaphoreHandle_t semaphore);

void ButtonsCheck(void);

void NextMainScreen(void);

void StatusBarDraw(void);

void MonitorScreenDraw(void);

void ReferenceScreenDraw(void);

void SettingsScreenDraw(void);

void DrawMenu(void);

void NavigateMenu(void);

void NavigateMenuDown(void);

void NavigateMenuUp(void);

void NavigateMenuEnter(void);

void NavigateMenuBack(void);

void NavigateMenuClearInfo(void);

void SoftVersionsScreenDraw(void);
//
// Global Variables
//


//
// Local Variables
//
uint16_t ReadData[64];

int16_t currentSelection = 0; // Номер строки выбранного пункта в текущем уровне
int16_t currentSelectionOld[5] = {0, 0, 0, 0, 0}; // массив для хранения номера строки, с которой вошли в дочернее меню
uint8_t currentMenuLvl = 0;
int16_t currentLevelStart = 0; // Индекс первого пункта текущего уровня
int16_t selectedItemIdx = 0; // Индекс выбранного пунтка текущего уровня


//--------------------------------------------------------------------
/*
* DisplayStatic - функция обработки связи и отображения данных на экране
*/
void DisplayStatic(void)
{

	// Вычитываю SW, SW1, FW, FW1, AW.
	UsbReadData(0x4200, 5, ReadData);
	StatusWord.all = ReadData[0];
	StatusWord1.all = ReadData[1];
	FaultWord.all = ReadData[2];
	FaultWord1.all = ReadData[3];
	AlarmWord.all = ReadData[4];

	// Обработка нажатий кнопок
	ButtonsCheck();

	/*
	 * ToDo: Добавлено на время отладки для того, чтобы в окне expressions
	 * отображались данные. Иначе компилятор просто выкидывает переменные.
	 */
	uint16_t a = AllGroups[0]->groupCnt;


	// отрисовка экранов
	switch (MainScreen)
	{
	case MonitorScr:
		MonitorScreenDraw();
		break;

	case ReferenceScr:
		ReferenceScreenDraw();
		break;

	case SettingsScr:
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
* ButtonsCheck - обработка нажатий кнопок
*/
void ButtonsCheck(void)
{
	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextMainScreen();
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NextMainScreen - переключение между главными экранами
*/
//--------------------------------------------------------------------
void NextMainScreen(void)
{
	switch (MainScreen)
	{
	case MonitorScr:
		MainScreen = ReferenceScr;
		break;

	case ReferenceScr:
		MainScreen = SettingsScr;
		break;

	case SettingsScr:
		MainScreen = MonitorScr;
		NavigateMenuClearInfo();
		break;
	}
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
	// отрисовка строки статуса
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
	// отрисовка строки статуса
	StatusBarDraw();

	ST7565_drawstring(20, 3, "Экран Задание");
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* SettingsScreenDraw - отрисовка экрана Настройки
*/
void SettingsScreenDraw(void)
{
	//ST7565_drawstring(20, 3, "Экран Настройки");

	switch (ChildScreen)
	{
	case NoScr:

		break;

	case MenuScr:
		// отрисовка строки статуса
		StatusBarDraw();

		// навигация по меню
		NavigateMenu();

		// отрисовка меню
		DrawMenu();

		break;

	case SoftVersionsScr:
		SoftVersionsScreenDraw();
		break;

	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* DrawMenu - отображает на экране 3 пункта текущего уровня в зависимости от выбранного пункта
*/
void DrawMenu(void)
{
	// Определяем для текущего уровня меню индекс начала и количество элементов
	GetCurrentLevelInfo(&currentLevelStart, &currentLevelCount);

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
        	selectedItemIdx = itemIdx;
        	ST7565_inv_fillrect(0, 15 + (16 * (i % 3)), 128, 9, 1);
        }

        // переписываю индекс для следующего пункта меню
        itemIdx = menuItems[itemIdx].nextIdx;
    }

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenu - навигация по меню
*/
void NavigateMenu(void)
{
	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		NavigateMenuDown();
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		NavigateMenuUp();
	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		NavigateMenuEnter();
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		NavigateMenuBack();
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenuDown - перемещение по меню вниз
*/
void NavigateMenuDown(void)
{
	// Определяем для текущего уровня меню индекс начала и количество элементов
	GetCurrentLevelInfo(&currentLevelStart, &currentLevelCount);

	if (currentSelection < (currentLevelCount-1)) {currentSelection++;}
	else {currentSelection = 0;}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenuUp - перемещение по меню вверх
*/
void NavigateMenuUp(void)
{
	// Определяем для текущего уровня меню индекс начала и количество элементов
	GetCurrentLevelInfo(&currentLevelStart, &currentLevelCount);

	if (currentSelection <= 0) {currentSelection = currentLevelCount-1;}
	else {currentSelection--;}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenuEnter - переход в дочернее меню
*/
void NavigateMenuEnter(void)
{
	// Если у пункта есть дочерние элементы - переходим к ним
	if(menuItems[selectedItemIdx].childIdx != -1)
	{
		currentParentIdx = selectedItemIdx;
		currentMenuLvl++;
		currentSelectionOld[currentMenuLvl] = currentSelection; // сохраняем номер строки в родительском меню
		currentSelection = 0; // в дочернем меню начнем с нулевой строки
	}
	else
	{
		if (menuItems[selectedItemIdx].childScreen != NoScr)
		{
			ChildScreen = menuItems[selectedItemIdx].childScreen;
		}
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenuBack - переход в родительское меню
*/
void NavigateMenuBack(void)
{
	// Если у пункта есть дочерние элементы - переходим к ним
	//if(menuItems[selectedItemIdx].parentIdx != -1)
	if(currentParentIdx != -1)
	{
		currentParentIdx = menuItems[currentParentIdx].parentIdx;
		currentSelection = currentSelectionOld[currentMenuLvl]; // восстанавливаем номер строки, с которой перешли в дочернее меню
		currentMenuLvl--;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* NavigateMenu - навигация по меню
*/
void NavigateMenuClearInfo(void)
{
	currentMenuIdx = 0;
	currentParentIdx = -1;
	currentLevelCount = 7;
	currentSelection = 0;
	memset(currentSelectionOld, 0, sizeof(currentSelectionOld));
	currentMenuLvl = 0;
	currentLevelStart = 0;
	selectedItemIdx = 0;
	ChildScreen = MenuScr;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* SoftVersionsScreenDraw - отрисовка экрана Версии ПО
*/
void SoftVersionsScreenDraw(void)
{
	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MenuScr;
	}

	ST7565_drawstring(20, 3, "Экран Версии ПО");
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
