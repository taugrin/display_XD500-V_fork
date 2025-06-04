#ifndef EEPROM_H_
#define EEPROM_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
//#include "string.h"
//#include "ftoa.h"
#include "math.h"


#include "stm32f1xx_hal.h"


//
// Defines
//
#define EEPROM_ADR 			0xA0
#define EEPROM_PAGE_SIZE 	64
#define EEPROM_MAX_PAGE		255
#define EEPROM_TIME_OUT 	100
#define EEPROM_DELAY 		5

#define MONITOR_SEL_BEGIN_ADR	EEPROM_MAX_PAGE * EEPROM_PAGE_SIZE

//
// TypeDefs
//


//
// Variables from other files
//
extern I2C_HandleTypeDef hi2c1;

//
// Variables for other files
//


//
// Functions for other files
//
uint8_t readMonitorSel(uint8_t num);

bool writeMonitorSel(uint8_t num, uint8_t *pData);

#endif /* EEPROM_H_ */
