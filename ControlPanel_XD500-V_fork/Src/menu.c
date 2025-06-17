//
// Included Files
//
#include "menu.h"

//
// Function Prototypes
//
int16_t Menu_AddItem(const char* name, int8_t parentIdx, int8_t nextIdx, tChildScreens childScreen/*, void *groups*/);

void Menu_ClearChildren(int16_t parentIdx);

//
// Global Variables
//

MenuItem menuItems[MAX_MENU_ITEMS];
uint16_t menuItemsCount = 0;

int16_t currentMenuIdx = 0;
int16_t currentParentIdx = -1;
uint16_t currentLevelCount = 7; // Количество пунктов в текущем уровне (для корня - 7)

//
// Local Variables
//


//--------------------------------------------------------------------
/*
* InitBasicMenu - инициализация корневого меню
*/
void InitBasicMenu(void)
{
	int16_t MenuItemsCnt, MenuFirstIdx, MenuNextIdx;

    // Добавляю элементы в корневое меню
	Menu_AddItem("ПАРАМЕТРЫ", -1, 1, NoScr/*, NULL*/); //[0]
	Menu_AddItem("АРХИВ СОБЫТИЙ", -1, 2, EventArciveScr); //[1]
	Menu_AddItem("БЫСТРЫЙ СТАРТ", -1, 3, FastSettingsScr); //[2]
	Menu_AddItem("ФУНКЦИИ КОПИРОВАНИЯ", -1, 4, NoScr); //[3]
	Menu_AddItem("ВЕРСИИ ПО", -1, 5, SoftVersionsScr); //[4]
	MenuItemsCnt = Menu_AddItem("МОНИТОРИНГ", -1, 0/*6*/, MonitorSettingsViewScr); //[5]
	//MenuItemsCnt = Menu_AddItem("ПОКАЗАНИЯ ТАЙМЕРОВ", -1, 0, NoScr); //[6]


	// Добавляю элементы в меню "ПАРАМЕТРЫ" группы для отображения в меню (parentIdx = 0)
	int16_t i = 0;
	while (i < MenuGroupsCnt)
	{
		if (i == (MenuGroupsCnt-1)) {MenuNextIdx = MenuFirstIdx;}
		else {MenuNextIdx = MenuItemsCnt+2;}
		MenuItemsCnt = Menu_AddItem(MenuGroups[i]->name, 0, MenuNextIdx, GroupViewScr);
		if (i == 0) {MenuFirstIdx = MenuItemsCnt;}
		i++;
	}


}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
/*
* Menu_AddItem - Добавление пункта
*/
int16_t Menu_AddItem(const char* name, int8_t parentIdx, int8_t nextIdx, tChildScreens childScreen/*, void *groups*/)
{
    if(menuItemsCount >= MAX_MENU_ITEMS) return -1;

    // Копируем имя с ограничением длины
    strncpy(menuItems[menuItemsCount].name, name, MENU_NAME_MAXLENGTH-1);
    menuItems[menuItemsCount].name[MENU_NAME_MAXLENGTH-1] = '\0'; // Гарантируем завершающий ноль

    menuItems[menuItemsCount].parentIdx = parentIdx;
    menuItems[menuItemsCount].childIdx = -1;
    menuItems[menuItemsCount].nextIdx = nextIdx;
    menuItems[menuItemsCount].childScreen = childScreen;
    //menuItems[menuItemsCount].groups = groups;

    // Обновляем childIdx у родителя
    if(parentIdx >= 0 && menuItems[parentIdx].childIdx == -1) {
        menuItems[parentIdx].childIdx = menuItemsCount;
    }

    return menuItemsCount++;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* Menu_ClearChildren - удаление всех детей у родительского меню
*/
void Menu_ClearChildren(int16_t parentIdx)
{
    if(parentIdx < 0) return;

    int16_t childIdx = menuItems[parentIdx].childIdx;
    while(childIdx != -1) {
        int16_t next = menuItems[childIdx].nextIdx;
        // Рекурсивно удаляем детей (если есть)
        if(menuItems[childIdx].childIdx != -1) {
            Menu_ClearChildren(childIdx);
        }
        childIdx = next;
    }
    menuItems[parentIdx].childIdx = -1;
}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
/*
* GetCurrentLevelInfo - получаем информацию о текущем уровне меню
*/
void GetCurrentLevelInfo(int16_t* startIdx, uint16_t* count)
{
    if (currentParentIdx == -1)
    {
        // Корневой уровень меню
        *startIdx = 0;
        *count = 0;

        // Подсчитываем количество элементов в корневом меню
        /*
         * Первый элемент меню в корне имеет индекс равный 0.
         * Но последний элемент меню в корне ссылается на первый для зацикливания меню.
         * Поэтому подсчет начинаем не с первого элемента, а со второго.
         * И останавливаем подсчет, когда индекс следующего элемента меню станет первым.
         */
        int16_t idx = 1;
        while (idx != 0 && *count < MAX_MENU_ITEMS)
        {
            (*count)++;
            idx = menuItems[idx].nextIdx;
        }
        (*count)++; // Увеличиваем количество на 1, поскольку считать начали не с 1ого, а со 2ого элемента меню.

    }
    else
    {
        // Уровень дочерних элементов
        *startIdx = menuItems[currentParentIdx].childIdx;
        *count = 0;

        // Подсчитываем количество дочерних элементов
        /*
         * Начинаем подчет не с первого элемента в подменю, а со следующего.
         * См. комментарий для корневого меню.
         */
        int16_t first_idx = *startIdx;
        int16_t idx = menuItems[first_idx].nextIdx;
        while (idx != first_idx && *count < MAX_MENU_ITEMS)
        {
            (*count)++;
            idx = menuItems[idx].nextIdx;
        }
        (*count)++; // Увеличиваем количество на 1, поскольку считать начали не с 1ого, а со 2ого элемента подменю.

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
