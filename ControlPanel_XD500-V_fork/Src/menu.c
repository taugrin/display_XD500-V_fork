//
// Included Files
//
#include "menu.h"

//
// Function Prototypes
//


//
// Global Variables
//
tMainScreen MainScreen = MonitorScreen;

struct Menu *ActiveMenu; // текущее активное меню

struct Menu ParametersMenu; // меню Параметры

struct Menu EventArchiveMenu; // меню Архив событий

struct Menu QuickStartMenu; // меню Быстрый старт

struct Menu ParamFuncsMenu; // меню Функции копирования

struct Menu SoftVersionsMenu; // меню Версия ПО

struct Menu MonitorSettingsMenu; // меню Мониторинг

struct Menu TimersMenu; // меню Показания таймеров

//
// Local Variables
//


//--------------------------------------------------------------------
/*
*
*/
void InitMenu(void)
{
	ParametersMenu.Parent = NULL;
	ParametersMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	ParametersMenu.Previous = &TimersMenu;
	ParametersMenu.Next = &EventArchiveMenu;
	ParametersMenu.lvl = 0; ParametersMenu.id = 1000;

	EventArchiveMenu.Parent = NULL;
	EventArchiveMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	EventArchiveMenu.Previous = &ParametersMenu;
	EventArchiveMenu.Next = &QuickStartMenu;
	EventArchiveMenu.lvl = 0; EventArchiveMenu.id = 2000;

	QuickStartMenu.Parent = NULL;
	QuickStartMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	QuickStartMenu.Previous = &EventArchiveMenu;
	QuickStartMenu.Next = &ParamFuncsMenu;
	QuickStartMenu.lvl = 0; QuickStartMenu.id = 3000;

	ParamFuncsMenu.Parent = NULL;
	ParamFuncsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	ParamFuncsMenu.Previous = &QuickStartMenu;
	ParamFuncsMenu.Next = &SoftVersionsMenu;
	ParamFuncsMenu.lvl = 0; ParamFuncsMenu.id = 4000;

	SoftVersionsMenu.Parent = NULL;
	SoftVersionsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	SoftVersionsMenu.Previous = &ParamFuncsMenu;
	SoftVersionsMenu.Next = &MonitorSettingsMenu;
	SoftVersionsMenu.lvl = 0; SoftVersionsMenu.id = 5000;

	MonitorSettingsMenu.Parent = NULL;
	MonitorSettingsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	MonitorSettingsMenu.Previous = &SoftVersionsMenu;
	MonitorSettingsMenu.Next = &TimersMenu;
	MonitorSettingsMenu.lvl = 0; MonitorSettingsMenu.id = 6000;

	TimersMenu.Parent = NULL;
	TimersMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	TimersMenu.Previous = &MonitorSettingsMenu;
	TimersMenu.Next = &ParametersMenu;
	TimersMenu.lvl = 0; TimersMenu.id = 7000;



	ActiveMenu = &ParametersMenu;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
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
*
*/
//--------------------------------------------------------------------

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

