#ifndef MENU_H_
#define MENU_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"


//
// Defines
//
#define MENU_NAME_MAXLENGTH	20

#define MAX_MENU_ITEMS 64   // Максимум пунктов меню

//
// TypeDefs
//

typedef struct
{
    char 		name[MENU_NAME_MAXLENGTH];	// Название пункта
    int8_t      parentIdx;  // Индекс родителя (-1 для корня)
    int8_t      childIdx;   // Индекс первого ребёнка (-1 если нет)
    int8_t      nextIdx;    // Индекс следующего пункта (-1 если нет)
    void*       data;       // Указатель на связанные данные (группы/параметры)
} MenuItem;


//
// Variables from other files
//


//
// Variables for other files
//

extern MenuItem menuItems[MAX_MENU_ITEMS];
extern uint16_t menuItemsCount;

extern int16_t currentMenuIdx;
extern int16_t currentParentIdx;
extern uint16_t currentLevelCount;

//
// Functions for other files
//
void InitBasicMenu(void);

void GetCurrentLevelInfo(int16_t* startIdx, uint16_t* count);


#endif /* MENU_H_ */
