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

struct Screen *ActiveScr; // текущий активный экран

struct Screen MonitorScr; // экран Монитор

struct Screen ReferenceScr; // экран Задание

struct Screen  SettingsScr; // экран Настройка

	struct Screen  ParametersScr; // экран Параметры

/*		struct Screen  ParamGroupsScr; // экран Группы параметров

			struct Screen  ParamEditScr; // экран Группы параметров

	struct Screen  EventArchiveScr; // экран Архив событий

	struct Screen  QuickStartScr; // экран Быстрый старт

	struct Screen  ParamFuncsScr; // экран Функции копирования

	struct Screen  SoftVersionsScr; // экран Версия ПО

	struct Screen  MonitorSettingsScr; // экран Мониторинг (настройка)

	struct Screen  TimersScr; // экран Показания таймеров
*/
//
// Local Variables
//


//--------------------------------------------------------------------
/*
*
*/
void InitScreens(void)
{
	// экран Монитор
	MonitorScr.Parent = NULL; MonitorScr.Child = NULL;
	MonitorScr.Next = &ReferenceScr;
	MonitorScr.level = 0;

	// экран Задание
	ReferenceScr.Parent = NULL; ReferenceScr.Child = NULL;
	ReferenceScr.Next = &SettingsScr;
	ReferenceScr.level = 0;

	// экран Настройка
	SettingsScr.Parent = NULL; SettingsScr.Child = &ParametersScr;
	SettingsScr.Next = &MonitorScr;
	SettingsScr.level = 0;




	ActiveScr = &MonitorScr;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/
//--------------------------------------------------------------------
void ScreenGoToNext(void)
{
	ActiveScr = ActiveScr->Next;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/*
*
*/
//--------------------------------------------------------------------
/*void ScreenGoToChild(void)
{
	if (ActiveScr->Child != NULL)
	{
		ActiveScr = ActiveScr->Child;
	}
}*/
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/
//--------------------------------------------------------------------
/*void ScreenGoToParrent(void)
{
	if (ActiveScr->Parent != NULL)
	{
		ActiveScr = ActiveScr->Parent;
	}
}*/
//--------------------------------------------------------------------


/*
*
*/

//--------------------------------------------------------------------

