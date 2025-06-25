#ifndef PARAMETERS_H_
#define PARAMETERS_H_

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

// максимальная длина символов в названии группы или параметра
#define MAX_NAME_LENGTH		21

// максимальное количество элементов в списке
#define MAX_LIST_CNT		12

#define PAR_IS_UINT			0
#define PAR_IS_INT			1
#define PAR_IS_LIST			2
#define PAR_IS_BIN			3
#define PAR_IS_HEX			4

#define UNITS_VOID			" "
#define UNITS_HZ  			"Hz"
#define UNITS_RPM  			"rpm"
#define UNITS_PROC			"%"
#define UNITS_VOLT  		"V"
#define UNITS_KWT  			"kW"
#define UNITS_AMP  			"A"
#define UNITS_CELSIUM		"§С"
#define UNITS_OHM 			"Ohm"
#define UNITS_SEC 			"s"
#define UNITS_MSEC 			"ms"


#define MODBUSADR_ADR           0x3200

#define PU1_SOURCE_ADR          0x0A00
#define PU2_SOURCE_ADR          0x0A01
#define PU1_PU2_SELECT_ADR      0x0A04
#define LOC_REM_ADR             0x0A05

#define FREQ_EST_ADR			0x4401
#define FREQ_REFERENCE_ADR      0x440E

#define CW_ADR					0x4000
#define CW1_ADR					0x4001
#define REFERENCE_ADR			0x4002


//
// TypeDefs
//

typedef struct
{
	char		name[MAX_NAME_LENGTH];
	uint16_t	adr;
	//uint16_t 	fatoryVal;
	uint16_t 	minVal;
	uint16_t 	maxVal;
	uint16_t	type;	// 0 - unsigned; 1 - signed; 2 - list;
	uint16_t 	scale;
	const char	*listItems;
	bool		writeEn;
	bool 		writeWhileRun;
	char		units[3];
} tParam;

typedef struct
{
	char			name[MAX_NAME_LENGTH];
	const tParam    *params;
	uint16_t		paramCnt;
	bool 			saveInEeprom;
	//bool 			viewInMenu;
} tGroup;



typedef union
{
    struct
    {
        uint16_t    run:1;                  //bit_0
        uint16_t    dir:1;                  //bit_1
        uint16_t    reset:1;                //bit_2
        uint16_t    off2:1;                 //bit_3
        uint16_t    pc_control:1;           //bit_4
        uint16_t    rstToFactory:1;         //bit_5
        uint16_t    remoteCmd:1;            //bit_6
        uint16_t    ext2sel:1;              //bit_7
        uint16_t    reserv:7;               //bit_8 - 14
        uint16_t    FirmwareUpdate:1;       //bit_15
    } bit;
    uint16_t all;
} tControlWord;

typedef union
{
    struct
    {
        uint16_t    reserved:16;             //bit_0-15
    } bit;
    uint16_t all;
} tControlWord1;

typedef union
{
    struct
    {
        uint16_t    ready:1;                //bit_0
        uint16_t    run:1;                  //bit_1
        uint16_t    dir:1;                  //bit_2
        uint16_t    fault:1;                //bit_3
        uint16_t    alarm:1;                //bit_4
        uint16_t    atRef:1;                //bit_5
        uint16_t    zeroSpd:1;              //bit_6
        uint16_t    fluxRdy:1;              //bit_7
        uint16_t    off2:1;                 //bit_8
        uint16_t    off3:1;                 //bit_9
        uint16_t    reserved:6;             //bit_10-15
    } bit;
    uint16_t all;
} tStatusWord;

typedef union
{
    struct
    {
        uint16_t    reserved:16;             //bit_0-15
    } bit;
    uint16_t all;
} tStatusWord1;

typedef union
{
    struct
    {
        uint16_t OC_Flag:1;                   //  0 Защита от токов короткого замыкания (аппаратная)
        uint16_t MC_Flag:1;                   //  1 Защита от токов короткого замыкания (программная)
        uint16_t OL_Flag:1;                   //  2 Защита от токов перегрузки
        uint16_t OV_Flag:1;                   //  3 Защита от максимального напряжения
        uint16_t UV_Flag:1;                   //  4 Защита от минимального напряжения
        uint16_t INV_Temp:1;                  //  5 Перегрев инвертора
        uint16_t INV_Error:1;                 //  6 Отказ инвертора
        uint16_t SYS_Error:1;                 //  7 Ошибка системы
        uint16_t MOTOR_Lost:1;                //  8 Мотор не подключен
        uint16_t POWER_Error:1;               //  9 Обрыв фазы питания
        uint16_t MOTOR_Temp:1;                // 10 Перегрев двигателя
        uint16_t EXT_Error:1;                 // 11 Ошибка внешняя
        uint16_t PULT_Comm:1;                 // 12 Потеря связи с пультом
        uint16_t MODBUS_Comm:1;               // 13 Потеря связи в сети MODBUS
        uint16_t FAN_Error:1;                 // 14 Отказ вентилятора охлаждения
        uint16_t MOTOR_Data:1;                // 15 Неверные данные электродвигателя/ Ошибка идентификации двигателя
    } bit;
    uint16_t all;
} tFaultWord;

typedef union
{
    struct
    {
        uint16_t    reserved:16;             //bit_0-15
    } bit;
    uint16_t all;
} tFaultWord1;

typedef union
{
    struct
    {
        uint16_t    reserved:16;             //bit_0-15
    } bit;
    uint16_t all;
} tAlarmWord;







//
// Variables from other files
//


//
// Variables for other files
//
extern tControlWord ControlWord, ControlWordOld;
extern tControlWord1 ControlWord1;

extern int16_t Reference, ReferenceOld;

extern tStatusWord StatusWord;
extern tStatusWord1 StatusWord1;
extern tFaultWord FaultWord;
extern tFaultWord1 FaultWord1;
extern tAlarmWord AlarmWord;

extern const tParam Monitor[];
extern const uint16_t MonitorCnt;
extern uint8_t MonitorSelect[3];
extern uint8_t MonitorNum;
extern uint8_t MonitorVal;


extern uint8_t EventNum;


extern const tGroup* const AllGroups[];
extern const uint16_t AllGroupsCnt;

extern const tGroup Group64;

extern const tGroup* const MenuGroups[];
extern const uint16_t MenuGroupsCnt;

extern const tParam* const FastSettings[];
extern const uint16_t FastSettingsCnt;
extern uint8_t FastSettingsNum;

//
// Functions for other files
//

const char* GetParamListItem(const tParam* param, uint8_t index);

void InitParams(void);

#endif /* PARAMETERS_H_ */
