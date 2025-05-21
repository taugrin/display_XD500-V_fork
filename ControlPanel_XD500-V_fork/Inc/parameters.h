#ifndef PARAMETERS_H_
#define PARAMETERS_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"


//
// Defines
//
#define MODBUSADR_ADR           0x3200

#define PU1_SOURCE_ADR          0x0A00
#define PU2_SOURCE_ADR          0x0A01
#define PU1_PU2_SELECT_ADR      0x0A04
#define LOC_REM_ADR             0x0A05

//
// TypeDefs
//

typedef union
{
    struct
    {
        uint16_t    start:1;                //bit_0
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
extern tControlWord ControlWord;
extern tControlWord1 ControlWord1;

extern tStatusWord StatusWord;
extern tStatusWord1 StatusWord1;
extern tFaultWord FaultWord;
extern tFaultWord1 FaultWord1;
extern tAlarmWord AlarmWord;

//
// Functions for other files
//



#endif /* PARAMETERS_H_ */
