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

	EventArchiveMenu.Parent = NULL;
	EventArchiveMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	EventArchiveMenu.Previous = &ParametersMenu;
	EventArchiveMenu.Next = &QuickStartMenu;

	QuickStartMenu.Parent = NULL;
	QuickStartMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	QuickStartMenu.Previous = &EventArchiveMenu;
	QuickStartMenu.Next = &ParamFuncsMenu;

	ParamFuncsMenu.Parent = NULL;
	ParamFuncsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	ParamFuncsMenu.Previous = &QuickStartMenu;
	ParamFuncsMenu.Next = &SoftVersionsMenu;

	SoftVersionsMenu.Parent = NULL;
	SoftVersionsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	SoftVersionsMenu.Previous = &ParamFuncsMenu;
	SoftVersionsMenu.Next = &MonitorSettingsMenu;

	MonitorSettingsMenu.Parent = NULL;
	MonitorSettingsMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	MonitorSettingsMenu.Previous = &SoftVersionsMenu;
	MonitorSettingsMenu.Next = &TimersMenu;

	TimersMenu.Parent = NULL;
	TimersMenu.Child = NULL; //ToDo: Пока NULL, но надо будет исправить
	TimersMenu.Previous = &MonitorSettingsMenu;
	TimersMenu.Next = &ParametersMenu;
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

