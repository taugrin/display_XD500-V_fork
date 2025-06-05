#ifndef SCREENS_H_
#define SCREENS_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"


//
// Defines
//


//
// TypeDefs
//

typedef enum
{
	MonitorScr = 0,
	ReferenceScr,
	SettingsScr
}tMainScreens;

typedef enum
{
	NoScr = 0,
	MenuScr,
	GroupViewScr,
	ParameterEditScr,
	MonitorSettingsViewScr,
	EventArciveScr,
	FastSettingsScr,
	MonitorSettingsEditScr,
	SoftVersionsScr
}tChildScreens;

//
// Variables from other files
//


//
// Variables for other files
//

extern tMainScreens MainScreen;

extern tChildScreens ChildScreen, PrevChildScreen;

//
// Functions for other files
//



#endif /* SCREENS_H_ */
