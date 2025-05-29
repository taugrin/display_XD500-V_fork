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

void GroupViewScreenDraw(void);

void SetBufferForDisplayParamData(const tParam* param, uint16_t value, bool withUnits);

void DisplayParameterValue(uint8_t x, uint8_t y, const tParam* param, uint16_t value);

void ParameterEditScreenDraw(void);

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

const tParam* param;
uint16_t paramData;
char paramDataCharBuf[21]; // Буфер для форматированного значения параметра
bool ParameterEditScreen1stStep;
uint8_t stringLen, stringLenOld;
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

	// Обработка нажатий кнопок
	ButtonsCheck();

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


	// Переключение между экранами по нажатию кнопки F.
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextMainScreen();
	}
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
	//ST7565_drawstring(20, 3, "Экран Настройки");

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

		break;

	// экран для отображения параметров в группе
	case GroupViewScr:
		GroupViewScreenDraw();
		break;

	// экран для ввода значения параметра
	case ParameterEditScr:
		ParameterEditScreenDraw();
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
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 2, MenuGroups[groupIdx]->name);

	// вывод названия параметра
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 4, MenuGroups[groupIdx]->params[paramIdx].name);

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
			ChildScreen = ParameterEditScr;
			ParameterEditScreen1stStep = true;
			editDigitBlink = false;
			editDigitBlinkCnt = 0;
			paramDataEditDigit = strlen(paramDataCharBuf) - 5; // крайняя правая цифра = длина массива - длина ед.изм (4) - 1;
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
void SetBufferForDisplayParamData(const tParam* param, uint16_t value, bool withUnits)
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
                // формируем строку и записываем её в paramDataCharBuf
				if (withUnits)
				{
                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u.%0*u %s",
                         int_part,
                         (int)log10(param->scale),
                         frac_part,
                         units);
				}
				else
				{
				snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u.%0*u",
                         int_part,
                         (int)log10(param->scale),
                         frac_part);
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
                if (withUnits)  {snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u %s", value, units);}
				else {snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%u", value);}
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
                // формируем строку и записываем её в paramDataCharBuf
				if (withUnits)
				{
                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d.%0*d %s",
                         int_part,
                         (int)log10(param->scale),
                         frac_part,
                         units);
				}
				else
				{
                snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d.%0*d",
                         int_part,
                         (int)log10(param->scale),
                         frac_part);
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
                if (withUnits) {snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d %s", (int16_t)value, units);}
				else {snprintf(paramDataCharBuf, sizeof(paramDataCharBuf), "%d", (int16_t)value);}
                /*
				 %d		Целое число со знаком (int_part)
				 %s		Строка (units — единицы измерения, например, "Hz").
				*/
            }
            break;

        case PAR_IS_LIST: // Значение из списка
        { // если не обернуть фигурными скобками, не даст создать const char* list_item
        	const char* list_item = GetListItem(param, value);
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
	SetBufferForDisplayParamData(param, value, true);

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
			//uint8_t stringLen, stringLenOld;
			int8_t centerPos;
			int8_t rightPos;
			int16_t paramDataI;

			// Вывод названия параметра по центру.
			stringLen = strlen(param->name);
			centerPos = DISP_LEFT_BOUND + FONT_GAP * (DISP_CENTER_CHAR_POS - stringLen/2);
			if (centerPos < 0) {centerPos = 0;}
			ST7565_drawstring(centerPos, 2, param->name);

			// Вывод значения параметра.
			if (editDigitBlinkCnt < 7) {editDigitBlinkCnt++;}
			else {editDigitBlinkCnt = 0; editDigitBlink = !editDigitBlink;}

			SetBufferForDisplayParamData(param, paramData, false);

			stringLen = strlen(paramDataCharBuf);
			if (ParameterEditScreen1stStep) {ParameterEditScreen1stStep = false; stringLenOld = stringLen;}
			if (stringLen > stringLenOld)
			{
				// значение увеличилось на порядок, надо переместить курсор вправо
				paramDataEditDigit++;
				if (paramDataCharBuf[paramDataEditDigit] == '.') {paramDataEditDigit++;}
				if (paramDataEditDigit > stringLen) {paramDataEditDigit = stringLen;}

			}

			if (stringLen < stringLenOld)
			{
				// значение уменьшилось на порядок, надо переместить курсор влево
				paramDataEditDigit--;
				if (paramDataCharBuf[paramDataEditDigit] == '.') {paramDataEditDigit--;}
				if (paramDataEditDigit < 0) {paramDataEditDigit = 0;}
				paramDataEditStepU /= 10; // уменьшаю шаг изменения параметр в 10 раз
				paramDataEditStepI /= 10; // уменьшаю шаг изменения параметр в 10 раз
			}

			stringLenOld = stringLen;

			centerPos = DISP_LEFT_BOUND + FONT_GAP * (DISP_CENTER_CHAR_POS - stringLen/2);
			if (centerPos < 0) {centerPos = 0;}

			if (editDigitBlink)
			{
				paramDataCharBuf[paramDataEditDigit] = '_'; // мигаю редактируемым разрядом
			}

			ST7565_drawstring(centerPos, 4, paramDataCharBuf);



			// Кнопка вниз
			if (CheckKeySem(xButtonDownSemaphore))
			{
				if (param->type == PAR_IS_UINT)
				{
					paramData -= paramDataEditStepU;
					if (paramData < param->minVal) {paramData = param->minVal;}
				}
				else
				{
					paramDataI = (int16_t)(paramData);
					paramDataI -= paramDataEditStepI;
					if (paramDataI < (int16_t)(param->minVal)) {paramDataI = (int16_t)(param->minVal);}
					paramData = (uint16_t)(paramDataI);
				}
			}

			// Кнопка вверх
			if (CheckKeySem(xButtonUpSemaphore))
			{
				if (param->type == PAR_IS_UINT)
				{
					paramData += paramDataEditStepU;
					if (paramData > param->maxVal) {paramData = param->maxVal;}
				}
				else
				{
					paramDataI = (int16_t)(paramData);
					paramDataI += paramDataEditStepI;
					if (paramDataI > (int16_t)(param->maxVal)) {paramDataI = (int16_t)(param->maxVal);}
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
				if ((paramDataEditDigit < 0) || (paramDataCharBuf[paramDataEditDigit] == '-')) // если крайняя левая позиция или знак минус
				{
					paramDataEditDigit = stringLen - 1; // перемещаю в крайнюю правую позицию
					paramDataEditStepU = 1; // сбрасываю шаг изменения параметра в наименьшее значение
					paramDataEditStepI = 1; // сбрасываю шаг изменения параметра в наименьшее значение
				}
			}

			// Кнопка enter
			if (CheckKeySem(xButtonEnterSemaphore))
			{
				UsbWriteReg(param->adr, paramData);
				ChildScreen = GroupViewScr;
			}



			// Вывод минального и максимального значений.
			SetBufferForDisplayParamData(param, param->minVal, false);
			ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 6, paramDataCharBuf);

			SetBufferForDisplayParamData(param, param->maxVal, false);
			rightPos = DISP_RIGHT_CHAR_POS - strlen(paramDataCharBuf);
			if (rightPos < 0) {rightPos = 0;}
			ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * rightPos, 6, paramDataCharBuf);

			// подписи мин слева и макс справа
			ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 7, "мин             макс");

		}
		break;

		case PAR_IS_LIST:

		break;

	}



	// Кнопка reset
	if (CheckKeySem(xButtonResetSemaphore))
	{
		ChildScreen = GroupViewScr;
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
