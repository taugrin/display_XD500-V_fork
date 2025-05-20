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



struct Screen
{
	struct Screen	*Next;
	struct Screen	*Parent;
	struct Screen	*Child;
	uint8_t			level;

	//char  			Name[MENU_NAME_MAXLENGTH];
};



//
// Variables from other files
//


//
// Variables for other files
//
extern struct Screen *ActiveScr;


//
// Functions for other files
//
void InitScreens(void);

void ScreenGoToNext(void);

/*void ScreenGoToChild(void);

void ScreenGoToParrent(void);
*/


#endif /* MENU_H_ */
