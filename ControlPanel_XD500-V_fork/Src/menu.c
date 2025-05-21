//
// Included Files
//
#include "menu.h"

//
// Function Prototypes
//
int16_t Menu_AddItem(const char* name, int8_t parentIdx, int8_t nextIdx, void* data);

void Menu_ClearChildren(int16_t parentIdx);

//
// Global Variables
//
tMainScreen MainScreen = MonitorScreen;

MenuItem menuItems[MAX_MENU_ITEMS];
uint16_t menuItemsCount = 0;

int16_t currentMenuIdx = 0;
int16_t currentParent = -1;
int16_t currentLevelCount = 7; // Количество пунктов в текущем уровне (для корня - 7) //ToDo: как автоматически вычислять?

//
// Local Variables
//

//--------------------------------------------------------------------
/*
* NextScreen - переключение между экранами
*/
//--------------------------------------------------------------------
void NextScreen(void)
{
	switch (MainScreen)
	{
	case MonitorScreen: MainScreen = ReferenceScreen; break;
	case ReferenceScreen: MainScreen = SettingsScreen; break;
	case SettingsScreen: MainScreen = MonitorScreen; break;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* InitBasicMenu - инициализация корневого меню
*/
void InitBasicMenu(void)
{
    Menu_AddItem("ПАРАМЕТРЫ", -1, 1, NULL); //[0]
    Menu_AddItem("АРХИВ СОБЫТИЙ", -1, 2, NULL); //[1]
    Menu_AddItem("БЫСТРЫЙ СТАРТ", -1, 3, NULL); //[2]
    Menu_AddItem("ФУНКЦИИ КОПИРОВАНИЯ", -1, 4, NULL); //[3]
    Menu_AddItem("ВЕРСИИ ПО", -1, 5, NULL); //[4]
    Menu_AddItem("МОНИТОРИНГ", -1, 6, NULL); //[5]
    Menu_AddItem("ПОКАЗАНИЯ ТАЙМЕРОВ", -1, 0, NULL); //[6]

}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
/*
* Menu_AddItem - Добавление пункта
*/
int16_t Menu_AddItem(const char* name, int8_t parentIdx, int8_t nextIdx, void* data)
{
    if(menuItemsCount >= MAX_MENU_ITEMS) return -1;

    // Копируем имя с ограничением длины
    strncpy(menuItems[menuItemsCount].name, name, MENU_NAME_MAXLENGTH-1);
    menuItems[menuItemsCount].name[MENU_NAME_MAXLENGTH-1] = '\0'; // Гарантируем завершающий ноль

    menuItems[menuItemsCount].parentIdx = parentIdx;
    menuItems[menuItemsCount].childIdx = -1;
    menuItems[menuItemsCount].nextIdx = nextIdx;
    menuItems[menuItemsCount].data = data;

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

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------
