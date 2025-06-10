//
// Included Files
//
#include "display.h"

//
// Function Prototypes
//
int8_t CheckKeySem(SemaphoreHandle_t semaphore);

void ControlSystem(void);

void DrawStringWithAlign(uint8_t line, uint8_t align, const char *c);

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

void GroupViewScreenDraw(void);

void SetBufferForDisplayParamData(const tParam* param, uint16_t value, bool withUnits, bool withZeros);

void DisplayParameterValue(uint8_t x, uint8_t y, const tParam* param, uint16_t value);

void ParameterEditScreenDraw(void);

void EventArciveScreenDraw(void);

void FastSettingsScreenDraw(void);

void MonitorSettingsScreenViewDraw(void);

void MonitorSettingsScreenEditDraw(void);

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

int16_t groupIdx = 0;
int16_t paramIdx = 0;

uint8_t stringLen, stringLenOld;

int8_t centerPos;
int8_t rightPos;

const tParam* param;
uint16_t paramData;
char paramDataCharBuf[21]; // Буфер для форматированного значения параметра

bool ParameterEditScreen1stStep;
bool editDigitBlink = false;     // Состояние мигания
uint8_t editDigitBlinkCnt = 0;   // Счетчик для мигания
int8_t paramDataEditDigit = 0; // Текущий редактируемый разряд (индекс в строке)
uint16_t paramDataEditStepU = 1; // Шаг изменения значения параметра
int16_t paramDataEditStepI = 1; // Шаг изменения значения параметра

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

	// Управление XD500-V
	ControlSystem();


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
* ControlSystem - управление XD500-V
*/
void ControlSystem(void)
{
	//---Формирую ControlWord---


	// Бит Run
	// Сброс бита Run при аварии или отсутствии готовности
	if ((StatusWord.bit.fault) || (!StatusWord.bit.ready))
	{
		ControlWord.bit.run = false;
	}
	else
	{
		// Кнопка Run
		if (CheckKeySem(xButtonRunSemaphore))
		{
			ControlWord.bit.run = true;
		}

		// Кнопка Stop
		if (CheckKeySem(xButtonStopSemaphore))
		{
			ControlWord.bit.run = false;
		}
	}

	// Бит Reset
	if (StatusWord.bit.fault)
	{
		// Удержание кнопки Reset
		if (CheckKeySem(xButtonResetAlarmsSemaphore))
		{
			ControlWord.bit.reset = true;
		}
	}
	if (ControlWordOld.bit.reset)
	{
		// сброс бита reset после записи
		ControlWord.bit.reset = false;
	}

	// Запись CW
	if (ControlWordOld.all != ControlWord.all)
	{
		UsbWriteReg(CW_ADR, ControlWord.all);
		ControlWordOld.all = ControlWord.all;
	}
	//---

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* DrawStringWithAlign - отрисовка текстовых данных с выравниванием
*/
void DrawStringWithAlign(uint8_t line, uint8_t align, const char *c)
{
	uint8_t len;
	int8_t pos;

	switch (align)
	{
	case 0: // left
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, line, c);
	break;

	case 1: // center
		len = strlen(c);
		pos = DISP_CENTER_CHAR_POS - len/2;
		if (pos < 0) {pos = 0;}
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * pos, line, c);

	break;

	case 2: // right
		len = strlen(c);
		pos = DISP_RIGHT_CHAR_POS - len;
		if (pos < 0) {pos = 0;}
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * pos, line, c);
	break;

	default: // default left
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, line, c);
	break;
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
	if (StatusWord.bit.fault)
	{
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 5, 0, "E");
		uint16_to_hex_str(FaultWord.all, CharArray, 2/*4*/);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 6, 0, CharArray); // значение FaultWord в формате hex, 2 символа
	}

	// Вывод сигнала Alarm
	if (StatusWord.bit.alarm)
	{
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 9, 0, "A");
		uint16_to_hex_str(AlarmWord.all, CharArray, 2);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 10, 0, CharArray); // значение AlarmWord в формате hex, 2 символа
	}

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

	// Монитор 1
	DrawStringWithAlign(2, ALIGN_LEFT, Monitor[MonitorSelect[0]].name);
	UsbReadData(Monitor[MonitorSelect[0]].adr, 1, ReadData);
	SetBufferForDisplayParamData(&Monitor[MonitorSelect[0]], ReadData[0], true, false);
	DrawStringWithAlign(2, ALIGN_RIGHT, paramDataCharBuf);

	// Монитор 2
	DrawStringWithAlign(4, ALIGN_LEFT, Monitor[MonitorSelect[1]].name);
	UsbReadData(Monitor[MonitorSelect[1]].adr, 1, ReadData);
	SetBufferForDisplayParamData(&Monitor[MonitorSelect[1]], ReadData[0], true, false);
	DrawStringWithAlign(4, ALIGN_RIGHT, paramDataCharBuf);

	// Монитор 3
	DrawStringWithAlign(6, ALIGN_LEFT, Monitor[MonitorSelect[2]].name);
	UsbReadData(Monitor[MonitorSelect[2]].adr, 1, ReadData);
	SetBufferForDisplayParamData(&Monitor[MonitorSelect[2]], ReadData[0], true, false);
	DrawStringWithAlign(6, ALIGN_RIGHT, paramDataCharBuf);


	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextMainScreen();
	}

	vTaskDelay(200);
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

	DrawStringWithAlign(3, ALIGN_CENTER, "Экран Задание");

	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextMainScreen();
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* SettingsScreenDraw - отрисовка экрана Настройки
*/
void SettingsScreenDraw(void)
{

	switch (ChildScreen)
	{
	case NoScr:

		break;

	// экран для отображения всего меню
	case MenuScr:
		// отрисовка строки статуса
		StatusBarDraw();

		// навигация по меню
		NavigateMenu();

		// отрисовка меню
		DrawMenu();


		paramIdx = 0; // сброс номера параметра, чтобы при входе в группу начиналось с 0
		EventNum = 0; // сброс номера события, чтобы при входе в меню начиналось с 0
		MonitorNum = 0; // сброс номера мониторинга, чтобы при входи в меню мониторинг всегда начиналось с 0
		FastSettingsNum = 0; // сброс номера параметра быстрых настроек, чтобы при входи в меню мониторинг всегда начиналось с 0

		break;

	// экран для отображения параметров в группе
	case GroupViewScr:
		GroupViewScreenDraw();
		break;

	// экран для ввода значения параметра
	case ParameterEditScr:
		ParameterEditScreenDraw();
		break;

	// экран для отображения архива событий
	case EventArciveScr:
		EventArciveScreenDraw();
		break;

	// экран для отображения быстрых настроек
	case FastSettingsScr:
		FastSettingsScreenDraw();
		break;

	// экран для отображения значения настроек мониторинга
	case MonitorSettingsViewScr:
		MonitorSettingsScreenViewDraw();
		break;

	// экран для редактирования значения настроек мониторинга
	case MonitorSettingsEditScr:
		MonitorSettingsScreenEditDraw();
		break;

	// экран для отображения версий ПО пульта и привода
	case SoftVersionsScr:
		SoftVersionsScreenDraw();
		break;

	}



	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextMainScreen();
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
* GroupViewScreenDraw - отрисовка экрана параметров в группе
*/
void GroupViewScreenDraw(void)
{
	// отрисовка строки статуса
	StatusBarDraw();

	// вывод названия группы
	groupIdx = selectedItemIdx - currentLevelStart;
	DrawStringWithAlign(2, ALIGN_LEFT, MenuGroups[groupIdx]->name);

	// вывод названия параметра
	DrawStringWithAlign(4, ALIGN_LEFT, MenuGroups[groupIdx]->params[paramIdx].name);

	// считывание значения параметра
	UsbReadData(MenuGroups[groupIdx]->params[paramIdx].adr, 1, ReadData);
	paramData = ReadData[0];

	// вывод значения параметра
	param = &MenuGroups[groupIdx]->params[paramIdx];
	DisplayParameterValue(DISP_LEFT_BOUND + FONT_GAP * 0, 6, param, paramData);


	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		if (paramIdx < (MenuGroups[groupIdx]->paramCnt - 1)) {paramIdx++;}
		else {paramIdx = 0;}
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		if (paramIdx > 0) {paramIdx--;}
		else {paramIdx = (MenuGroups[groupIdx]->paramCnt -1);}
	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		if (param->writeEn)
		{
			PrevChildScreen = ChildScreen; // для возврата либо в группы, либо в быстрые настройки
			ChildScreen = ParameterEditScr;
			ParameterEditScreen1stStep = true;
			editDigitBlink = false;
			editDigitBlinkCnt = 0;
			SetBufferForDisplayParamData(param, paramData, false, true);
			paramDataEditDigit = strlen(paramDataCharBuf) - 1;
			paramDataEditStepU = 1;
			paramDataEditStepI = 1;
		}
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MenuScr;
	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* SetBufferForDisplayParamData - подготовка буффера для отображения значения параметра на экране
*/
void SetBufferForDisplayParamData(const tParam* param, uint16_t value, bool withUnits, bool withZeros)
{
	char units[4] = "   "; // Буфер для единиц измерения (3 символа + '\0')

    // Копируем единицы измерения из параметра
    strncpy(units, param->units, 3);
    units[3] = '\0';

    switch(param->type)
	{
        case PAR_IS_UINT: // Беззнаковое целое
            if(param->scale > 1) {
                // Дробное значение (целое с масштабированием)
                uint16_t int_part = value / param->scale; // целая часть
                uint16_t frac_part = value % param->scale; // дробная часть
                int frac_digits = (int)log10(param->scale);
                // формируем строку и записываем её в paramDataCharBuf
				if (withUnits)
				{
					if (withZeros)
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%0*u.%0*u %s",
								 (int)(5-frac_digits),
								 int_part,
								 frac_digits,
								 frac_part,
								 units);
					}
					else
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u.%0*u %s",
								 int_part,
								 frac_digits,
								 frac_part,
								 units);
					}
				}
				else
				{
					if (withZeros)
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%0*u.%0*u",
								 (int)(5-frac_digits),
								 int_part,
								 frac_digits,
								 frac_part);
					}
					else
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u.%0*u",
								 int_part,
								 frac_digits,
								 frac_part);
					}

				}
				/*
                 %u		Беззнаковое целое число (int_part)
                 . 		Разделитель целой и дробной части
                 %0*u	Динамическое форматирование:
                 	 	 	 0 — ведущие нули, нужны например если frac_part = 1, но param->scale = 100, тогда будет вывод 01
                 	 	 	 * — подстановка числа из аргумента. log10(100) = 2, поэтому будет %0*2
                 %s		Строка (units — единицы измерения, например, "Hz").
                */
            } else {
                // Просто целое беззнаковое
            	// формируем строку и записываем её в paramDataCharBuf
                if (withUnits)
                {
                	if (withZeros)
					{
                		snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%05u %s", value, units);
					}
                	else
                	{
                		snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u %s", value, units);
                	}
                }
				else
				{
                	if (withZeros)
					{
                		snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%05u", value);
					}
                	else
                	{
                		snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u", value);
                	}
				}
                /*
				 %u		Беззнаковое целое число (int_part)
				 %s		Строка (units — единицы измерения, например, "Hz").
				*/
            }
            break;

        case PAR_IS_INT: // Знаковое целое
            if(param->scale > 1) {
                // Дробное значение со знаком
                int16_t signed_value = (int16_t)value;
                int16_t int_part = signed_value / param->scale;
                int16_t frac_part = abs(signed_value % param->scale);
                int frac_digits = (int)log10(param->scale);
                // формируем строку и записываем её в paramDataCharBuf
				if (withUnits)
				{
					if (withZeros)
					{
		                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%+0*d.%0*d %s",
		                		 (int)(6-frac_digits),
		                		 int_part,
								 frac_digits,
		                         frac_part,
		                         units);
					}
					else
					{
		                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d.%0*d %s",
		                         int_part,
								 frac_digits,
		                         frac_part,
		                         units);
					}

				}
				else
				{
					if (withZeros)
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%+0*d.%0*d",
								 (int)(6-frac_digits),
								 int_part,
								 frac_digits,
								 frac_part);
					}
					else
					{
		                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d.%0*d",
		                         int_part,
								 frac_digits,
		                         frac_part);
					}

				}
                /*
                 %d		Целое число со знаком (int_part)
                 . 		Разделитель целой и дробной части
                 %0*d	Динамическое форматирование:
                 	 	 	 0 — ведущие нули, нужны например если frac_part = 1, но param->scale = 100, тогда будет вывод 01
                 	 	 	 * — подстановка числа из аргумента. log10(100) = 2, поэтому будет %0*2
                 %s		Строка (units — единицы измерения, например, "Hz").
                */
            } else {
                // Просто целое знаковое
            	// формируем строку и записываем её в paramDataCharBuf
                if (withUnits)
                {
					if (withZeros)
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%+06d %s", (int16_t)value, units);
					}
					else
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d %s", (int16_t)value, units);
					}
                }
				else
				{
					if (withZeros)
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%+06d", (int16_t)value);
					}
					else
					{
						snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d", (int16_t)value);
					}
				}
                /*
				 %d		Целое число со знаком (int_part)
				 %s		Строка (units — единицы измерения, например, "Hz").
				*/
            }
            break;

        case PAR_IS_LIST: // Значение из списка
        { // если не обернуть фигурными скобками, не даст создать const char* list_item
        	const char* list_item = GetParamListItem(param, value);
            if(list_item)
            {
            	// формируем строку и записываем её в paramDataCharBuf
                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%s", list_item);
                /*
				 %s	Вывод строки.
				*/
            }
            else
            {
                // Вывод сообщения, если нет такого значения в списке
            	snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "НЕТ В СПИСКЕ");
            }
        } // если не обернуть фигурными скобками, не даст создать const char* list_item
            break;

        case PAR_IS_BIN: // Значение в бинарном виде
        	uint16_to_bin_str(value, paramDataCharBuf, param->scale);
            break;

        case PAR_IS_HEX: // Значение в 16-ричном виде
        	uint16_to_hex_str(value, paramDataCharBuf, param->scale);
            break;

        default:
            // Вывод сообщения, если неправильно указан тип данных.
        	snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "ОШИБКА ТИПА");
            break;
    }

}

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* DisplayParameterValue - Функция для вывода параметров на дисплей
* @param x - позиция по X (0-127)
* @param y - линия (позиция по Y (0-7), где каждый шаг = 8 пикселям)
* @param param - указатель на параметр (из tParam)
* @param value - значение параметра (сырое значение из регистра)
*/
void DisplayParameterValue(uint8_t x, uint8_t line, const tParam* param, uint16_t value)
{
	SetBufferForDisplayParamData(param, value, true, false);

    // Очищаем область перед выводом
    //ST7565_fillrect(x, line * 8, 128 - x, 8, 0);

    // Выводим значение на дисплей
    ST7565_drawstring(x, line, paramDataCharBuf);

}

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* ParameterEditScreenDraw - отрисовка экрана изменения параметра
*/
void ParameterEditScreenDraw(void)
{

	// отрисовка строки статуса
	StatusBarDraw();



	switch(param->type)
	{
		case PAR_IS_UINT:
		case PAR_IS_INT:
		{
			int16_t paramDataI;

			// Вывод названия параметра по центру.
			DrawStringWithAlign(2, ALIGN_CENTER, param->name);

			// Вывод значения параметра (начало).
			if (editDigitBlinkCnt < 7) {editDigitBlinkCnt++;}
			else {editDigitBlinkCnt = 0; editDigitBlink = !editDigitBlink;}

			SetBufferForDisplayParamData(param, paramData, false, true);

			stringLen = strlen(paramDataCharBuf);


			// Кнопка вниз
			if (CheckKeySem(xButtonDownSemaphore))
			{
				if (param->type == PAR_IS_UINT)
				{
					if (paramData <= param->minVal) {paramData = param->minVal;}
					else
					{
						if (paramData <= param->minVal + paramDataEditStepU) {paramData = param->minVal;}
						else {paramData -= paramDataEditStepU;}
					}
				}
				else
				{
					paramDataI = (int16_t)(paramData);
					if (paramDataCharBuf[paramDataEditDigit] == '-')
					{
						paramDataI = -paramDataI;
						if (paramDataI >= (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
						if (paramDataI <= (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
					}
					else if (paramDataCharBuf[paramDataEditDigit] == '+')
					{
						paramDataI = -paramDataI;
						if (paramDataI >= (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
						if (paramDataI <= (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
					}
					else
					{

						if (paramDataI <= (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
						else
						{
							if (paramDataI <= (int16_t)(param->minVal) + paramDataEditStepI) {paramDataI = (int16_t)(param->minVal);}
							else {paramDataI -= paramDataEditStepI;}
						}

					}
					paramData = (uint16_t)(paramDataI);

				}
			}

			// Кнопка вверх
			if (CheckKeySem(xButtonUpSemaphore))
			{
				if (param->type == PAR_IS_UINT)
				{
					if (paramData >= param->maxVal) {paramData = param->maxVal;}
					else
					{
						if (paramData >= param->maxVal - paramDataEditStepU) {paramData = param->maxVal;}
						else {paramData += paramDataEditStepU;}
					}
				}
				else
				{
					paramDataI = (int16_t)(paramData);
					if (paramDataCharBuf[paramDataEditDigit] == '-')
					{
						paramDataI = -paramDataI;
						if (paramDataI >= (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
						if (paramDataI <= (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
					}
					else if (paramDataCharBuf[paramDataEditDigit] == '+')
					{
						paramDataI = -paramDataI;
						if (paramDataI >= (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
						if (paramDataI <= (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
					}
					else
					{

						if (paramDataI >= (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
						else
						{
							if (paramDataI >= (int16_t)(param->maxVal) - paramDataEditStepI) {paramDataI = (int16_t)(param->maxVal);}
							else {paramDataI += paramDataEditStepI;}
						}

					}
					paramData = (uint16_t)(paramDataI);

				}
			}

			// Кнопка F
			if (CheckKeySem(xButtonFuncSemaphore))
			{
				//editDigitBlinkCnt = 0; editDigitBlink = true;
				paramDataEditDigit--; // перемещаю курсор влево
				paramDataEditStepU *= 10; // увеличиваю шаг изменения параметр в 10 раз
				paramDataEditStepI *= 10; // увеличиваю шаг изменения параметр в 10 раз
				if (paramDataCharBuf[paramDataEditDigit] == '.') {paramDataEditDigit--;} // если попалась точка, перемещаю еще влево
				if (paramDataEditDigit < 0) // если крайняя левая позиция
				{
					paramDataEditDigit = stringLen - 1; // перемещаю в крайнюю правую позицию
					paramDataEditStepU = 1; // сбрасываю шаг изменения параметра в наименьшее значение
					paramDataEditStepI = 1; // сбрасываю шаг изменения параметра в наименьшее значение
				}
			}

			// Вывод значения параметра (продолжение).
			centerPos = DISP_LEFT_BOUND + FONT_GAP * (DISP_CENTER_CHAR_POS - stringLen/2);
			if (centerPos < 0) {centerPos = 0;}

			if (editDigitBlink)
			{
				paramDataCharBuf[paramDataEditDigit] = '_'; // мигаю редактируемым разрядом
			}

			DrawStringWithAlign(4, ALIGN_CENTER, paramDataCharBuf);


			// Вывод минального и максимального значений.
			SetBufferForDisplayParamData(param, param->minVal, false, false);
			DrawStringWithAlign(6, ALIGN_LEFT, paramDataCharBuf);

			SetBufferForDisplayParamData(param, param->maxVal, false, false);
			DrawStringWithAlign(6, ALIGN_RIGHT, paramDataCharBuf);

			// подписи мин слева и макс справа
			DrawStringWithAlign(7, ALIGN_LEFT, "мин             макс");

		}
		break;

		case PAR_IS_LIST:
		{
			uint8_t BeginListItem = (paramData / 3)*3; // верхний пункт списка на экране в зависимости от выбранного
			uint8_t EndListItem = BeginListItem + 3; // нижний пункт списка на экране в зависимости от выбранного

			// отрисовываю три пункта списка на экране в зависимости от выбранного
			uint8_t j = 2;
			for(uint8_t i = BeginListItem; i <= param->maxVal && i < EndListItem; i++)
			{
				// Отображение названия пункта
				DrawStringWithAlign(j, ALIGN_LEFT, GetParamListItem(param, i));
				j+=2;

				// Выделение текущего выбранного пункта
				if(i == paramData)
				{
					ST7565_inv_fillrect(0, 15 + (16 * (i % 3)), 128, 9, 1);
				}

			}

			// Кнопка вниз
			if (CheckKeySem(xButtonDownSemaphore))
			{
				if (paramData == param->maxVal) {paramData = param->minVal;}
				else {paramData++;}
			}

			// Кнопка вверх
			if (CheckKeySem(xButtonUpSemaphore))
			{
				if (paramData == param->minVal) {paramData = param->maxVal;}
				else {paramData--;}
			}
		}
		break;

	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		UsbWriteReg(param->adr, paramData);
		ChildScreen = PrevChildScreen; // возврат либо в группы, либо в быстрые настройки
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = PrevChildScreen; // возврат либо в группы, либо в быстрые настройки
	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* EventArciveScreenDraw - экран для отображения архива событий
*/
void EventArciveScreenDraw(void)
{
	// отрисовка строки статуса
	StatusBarDraw();

	// Надпись Событие
	DrawStringWithAlign(2, ALIGN_LEFT, "СОБЫТИЕ ");

	// Вывод номера события
	char CharArray[2];
	my_itoa((EventNum+1), CharArray);
	if (EventNum < 9)
	{
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 8, 2, "0");
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 9, 2, CharArray);
	}
	else {ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 8, 2, CharArray);}


	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		if (EventNum == 9) {EventNum = 0;}
		else {EventNum++;}
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		if (EventNum == 0) {EventNum = 9;}
		else {EventNum--;}
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MenuScr;
	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* FastSettingsScreenDraw - экран для отображения быстрых настроек
*/
void FastSettingsScreenDraw(void)
{
	char CharArray[21];
	param = FastSettings[FastSettingsNum];

	// отрисовка строки статуса
	StatusBarDraw();

	// Вывод номера параметра
	DrawStringWithAlign(2, ALIGN_LEFT, "ПАРАМЕТР ");
	// вывожу номер параметра на основе его адреса в формате №группы."параметра
	snprintf(CharArray, sizeof(CharArray), "%u.%u",
			((param->adr >> 8) & 0xFF),
			((param->adr) & 0xFF));
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 9, 2, CharArray);

	// название параметра, без номера в начале (т.е. начиная с третьего символа)
	DrawStringWithAlign(4, ALIGN_LEFT, ((FastSettings[FastSettingsNum]->name)+3));

	// считывание значения параметра
	UsbReadData(param->adr, 1, ReadData);
	paramData = ReadData[0];

	// вывод значения параметра
	DisplayParameterValue(DISP_LEFT_BOUND + FONT_GAP * 0, 6, param, paramData);

	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		if (FastSettingsNum == (FastSettingsCnt-1)) {FastSettingsNum = 0;}
		else {FastSettingsNum++;}
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		if (FastSettingsNum == 0) {FastSettingsNum = FastSettingsCnt-1;}
		else {FastSettingsNum--;}
	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		if (param->writeEn)
		{
			PrevChildScreen = ChildScreen; // для возврата либо в группы, либо в быстрые настройки
			ChildScreen = ParameterEditScr;
			ParameterEditScreen1stStep = true;
			editDigitBlink = false;
			editDigitBlinkCnt = 0;
			SetBufferForDisplayParamData(param, paramData, false, true);
			paramDataEditDigit = strlen(paramDataCharBuf) - 1;
			paramDataEditStepU = 1;
			paramDataEditStepI = 1;
		}
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MenuScr;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* MonitorSettingsScreenViewDraw - экран для отображения значения настроек мониторинга
*/
void MonitorSettingsScreenViewDraw(void)
{
	// отрисовка строки статуса
	StatusBarDraw();

	// Надпись Мониторинг
	DrawStringWithAlign(2, ALIGN_LEFT, "МОНИТОРИНГ");

	// номер мониторинга
	switch (MonitorNum)
	{
	case 0: DrawStringWithAlign(4, ALIGN_LEFT, "МОНИТОР 1"); break;
	case 1: DrawStringWithAlign(4, ALIGN_LEFT, "МОНИТОР 2"); break;
	case 2: DrawStringWithAlign(4, ALIGN_LEFT, "МОНИТОР 3"); break;
	}

	// значение мониторинга
	DrawStringWithAlign(6, ALIGN_LEFT, Monitor[MonitorSelect[MonitorNum]].name);

	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		if (MonitorNum == 2) {MonitorNum = 0;}
		else {MonitorNum++;}
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		if (MonitorNum == 0) {MonitorNum = 2;}
		else {MonitorNum--;}
	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		MonitorVal = MonitorSelect[MonitorNum];
		ChildScreen = MonitorSettingsEditScr;
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MenuScr;
	}

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* MonitorSettingsScreenEditDraw - экран для редактирования значения настроек мониторинга
*/
void MonitorSettingsScreenEditDraw(void)
{
	// отрисовка строки статуса
	StatusBarDraw();

	uint8_t BeginListItem = (MonitorVal / 3)*3; // верхний пункт списка на экране в зависимости от выбранного
	uint8_t EndListItem = BeginListItem + 3; // нижний пункт списка на экране в зависимости от выбранного

	// отрисовываю три пункта списка на экране в зависимости от выбранного
	uint8_t j = 2;
	for(uint8_t i = BeginListItem; i < MonitorCnt && i < EndListItem; i++)
	{
		// Отображение названия пункта
		DrawStringWithAlign(j, ALIGN_LEFT, Monitor[i].name);
		j+=2;

		// Выделение текущего выбранного пункта
		if(i == MonitorVal)
		{
			ST7565_inv_fillrect(0, 15 + (16 * (i % 3)), 128, 9, 1);
		}

	}

	// Кнопка вниз
	if (CheckKeySem(xButtonDownSemaphore))
	{
		if (MonitorVal == (MonitorCnt - 1)) {MonitorVal = 0;}
		else {MonitorVal++;}
	}

	// Кнопка вверх
	if (CheckKeySem(xButtonUpSemaphore))
	{
		if (MonitorVal == 0) {MonitorVal = MonitorCnt - 1;}
		else {MonitorVal--;}
	}

	// Кнопка enter
	if (CheckKeySem(xButtonEnterSemaphore))
	{
		MonitorSelect[MonitorNum] = MonitorVal;
		writeMonitorSel(MonitorNum, &MonitorVal);

		ChildScreen = MonitorSettingsViewScr;
	}

	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = MonitorSettingsViewScr;
	}

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

	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 1, "     ВЕРСИИ ПО:");

	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 3, "ПО ПУЛЬТА:");
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 12, 3, SOFT_VERSION);

	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 5, "ПО XD500-V:");

	UsbReadData(0x4400, 1, ReadData);
    // Разбиваем число на части и форматируем
    snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d.%02d.%02d",
    		 ReadData[0] / 10000,         // A (первая часть)
             (ReadData[0] / 100) % 100,   // BB (вторая часть)
			 ReadData[0] % 100);          // CC (третья часть)

	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 12, 5, paramDataCharBuf);

	vTaskDelay(200);

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
