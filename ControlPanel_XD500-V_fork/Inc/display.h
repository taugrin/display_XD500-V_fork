#ifndef DISPLAY_H_
#define DISPLAY_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "ftoa.h"
#include "stdio.h"
#include "math.h"

#include "cmsis_os.h"

#include "usertasks.h"

#include "ST7565.h"

#include "usb_funcs.h"

#include "string_funcs.h"

#include "parameters.h"

#include "screens.h"

#include "menu.h"




//
// Defines
//

#define SOFT_VERSION "1.00.00"

#define FONT_SIZE 5
#define CHAR_GAP  1
#define FONT_GAP (FONT_SIZE+1*CHAR_GAP)

#define NUM_MAX_LEN 6

#define DISP_RIGHT_BOUND 127
#define DISP_LEFT_BOUND 2
#define DISP_CENTR 64

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

#define DISP_RIGHT_CHAR_POS 20
#define DISP_CENTER_CHAR_POS 10

#define VISIBLE_ITEMS 3 //  оличество отображаемых пунктов за раз

//
// TypeDefs
//



//
// Variables from other files
//


//
// Variables for other files
//


//
// Functions for other files
//
void DisplayStatic(void);




#endif /* DISPLAY_H_ */
