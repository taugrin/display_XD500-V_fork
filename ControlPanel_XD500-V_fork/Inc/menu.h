#ifndef MENU_H_
#define MENU_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"



//
// Defines
//
#define MENU_NAME_MAXLENGTH	20

//
// TypeDefs
//
typedef enum
{
	MonitorScreen = 0,
	ReferenceScreen,
	SettingsScreen
}tMainScreen;


struct Menu
{
	struct Menu	*Previous;
	struct Menu	*Next;
	struct Menu	*Parent;
	struct Menu	*Child;
	uint8_t		id;

	//char  			Name[MENU_NAME_MAXLENGTH];
};



//
// Variables from other files
//


//
// Variables for other files
//
extern tMainScreen MainScreen;


//
// Functions for other files
//
void InitMenu(void);

void NextScreen(void);

/*void ScreenGoToChild(void);

void ScreenGoToParrent(void);
*/


#endif /* MENU_H_ */
