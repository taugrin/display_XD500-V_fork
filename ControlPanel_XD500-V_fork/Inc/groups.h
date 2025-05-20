#ifndef GROUPS_H_
#define GROUPS_H_

#include <stdbool.h>

#define MAX_PARAM 24

#define MAX_LIST_NUMB 22

#define MAX_STRING 21

#define MAX_STRING_UNIT 6

#define SEC "s"
#define MSEC "ms"
#define HZ  "Hz"
#define RPM  "rpm"
#define PRC  "%"
#define VOLT  "V"
#define KWT  "kW"
#define AMP  "A"
#define OHM "Ohm"
#define NUL  ""

#define PAR_LIST 0
#define PAR_INT 1
#define PAR_UINT 3
#define PAR_ARCHIVE 4
#define PAR_BLIST 5// blocked list
#define PAR_MLIST 6// masked list

typedef struct
{
 int16_t adr;
 int value;
 int min;
 int max;
 uint8_t type;  //1 -int, 0 - selected from list, 2 - reserved, 3 - uint (65536), 4 - archive, 5 - blocked list, - 6 masked list
 uint8_t factor;
 int pointer;
 char name [MAX_STRING];
 //char list [MAX_LIST][MAX_STRING];
 uint8_t listBase_id;
 char units [MAX_STRING_UNIT];
} param_t;

typedef struct
{
    int maxParam;
    char name [MAX_STRING];
    param_t param [MAX_PARAM];
    bool save_to_eeprom;
} group_t;


// ================== √–”œœ¿ 00 ==========
#define GROUP_00_TEXT "00 œ≈–≈Ã «Õ¿◊"
#define PAR_NUMB_G00 3
#define G00_P03_TEXT "03 —¡–Œ— «¿Ÿ»“"
#define G00_P09_TEXT "09 —“¿–“/—“Œœ"
#define G00_P10_TEXT "10 «¿ƒ¿Õ œŒ ◊¿—“"
#define G00_P11_TEXT "03 —¡–Œ— «¿Ÿ»“"
// adr, value, min, max, type, factor, pointer, name, listbase_id, units
#define G00P03 {0x0002, 0,    0, 1,   PAR_INT, 1, 0,    G00_P03_TEXT, 0, NUL}
#define G00P09 {0x0008, 0,    0, 1,   PAR_INT, 1, 0,    G00_P09_TEXT, 0, NUL}
#define G00P10 {0x0009, 0, -100, 100, PAR_INT, 1, 0,    G00_P10_TEXT, 0, PRC}
#define G00P11 {0x000A, 0,    0, 1,   PAR_INT, 1, 0,    G00_P11_TEXT, 0, NUL}

#define G00P10_RPM {0x0009, 0, -5000, 5000, PAR_INT, 1, 0, G00_P10_TEXT, 0, "RPM"}

#define G00_PARAMS {G00P03, G00P09, G00P10}
#define GROUP00 {PAR_NUMB_G00, GROUP_00_TEXT, G00_PARAMS, false}


// ================== √–”œœ¿ 01 ==========
#define GROUP_01_TEXT "01 ÃŒÕ»“Œ–»Õ√"


#define MONITOR_TEXT "01 ÃŒÕ»“Œ–»Õ√"
#define MONITORS_NUMBER 3
#define MONITOR_P01_TEXT "ÃŒÕ»“Œ– 1"
#define MONITOR_P02_TEXT "ÃŒÕ»“Œ– 2"
#define MONITOR_P03_TEXT "ÃŒÕ»“Œ– 3"
#define MONITOR_LIST_NUMBUR 22
#define MONITOR_LIST {\
		"RO STATUS",\
		"DI STATUS",\
		"AI1 SCALE, %",\
		"AI2 SCALE, %",\
		"AI3 SCALE, %",\
		"DC BUS VOLT, V",\
		"VOLTAGE, V",\
		"CURRENT, A",\
		"REFERENCE, Hz",\
		"FREQUENCY, Hz",\
		"POWER, kW",\
		"IGBT TEMP, ß—",\
		"MOT SPEED, RPM",\
		"MOTOR FLUX",\
		"PID REF VALUE, %",\
		"PID FEEDBACK, %",\
		"PID ERROR, %",\
		"PID OUT, %",\
		"CONTROL WORD",\
		"STATUS WORD",\
		"FAULT WORD",\
		"ALARM WORD",\
}// 35
#define MONITOR_LIST_BASE_NUM 35
#define MONITOR_P01 {0xF000, 0, 0, MONITOR_LIST_NUMBUR-1, PAR_MLIST, 1, 0, MONITOR_P01_TEXT, MONITOR_LIST_BASE_NUM, NUL}
#define MONITOR_P02 {0xF001, 0, 0, MONITOR_LIST_NUMBUR-1, PAR_MLIST, 1, 0, MONITOR_P02_TEXT, MONITOR_LIST_BASE_NUM, NUL}
#define MONITOR_P03 {0xF002, 0, 0, MONITOR_LIST_NUMBUR-1, PAR_MLIST, 1, 0, MONITOR_P03_TEXT, MONITOR_LIST_BASE_NUM, NUL}

#define MONITOR_PARAMS {MONITOR_P01, MONITOR_P02, MONITOR_P03}
#define MONITOR_GROUP {MONITORS_NUMBER, MONITOR_TEXT, MONITOR_PARAMS, true}


// ================== √–”œœ¿ 10 =============
//                       12345678901234567890
#define GROUP_10_TEXT   "10 œ”— /—“Œœ/Õ¿œ–¿¬À"
#define PAR_NUMB_G10 7
#define G10_P01_TEXT    "01 œ”1 —“¿–“/—“Œœ"
#define G10_P02_TEXT    "02 œ”2 —“¿–“/—“Œœ"
#define G10_P03_TEXT    "03 Õ¿œ–¿¬À≈Õ»≈"
#define G10_P04_TEXT    "04 –¿«–≈ÿ≈Õ»≈ –¿¡Œ“€"
#define G10_P05_TEXT    "05 ¬€¡Œ– œ”1/œ”2"
#define G10_P06_TEXT    "06 –≈∆»Ã ”œ–¿¬À≈Õ»ﬂ"
#define G10_P07_TEXT    "07 –≈∆»Ã –¿¡Œ“€"
#define ALL_PARAMS_G10 \
{\
G10_P01_TEXT,\
G10_P02_TEXT,\
G10_P03_TEXT,\
G10_P04_TEXT,\
G10_P05_TEXT,\
G10_P06_TEXT,\
G10_P07_TEXT,\
}
#define G10P01_LIST {"NOT SEL", "DI1", "DI1, DI2", "DI1P, DI2P", "DI1P, DI2P, DI3", "DI1P, DI2P, DI3P", "DI6", "DI6, DI5", "KEYPAD", "FIELDBUS"}// 0
#define G10P03_LIST {"FORWARD", "REVERSE", "REQUEST"}// 1
#define G10P04_LIST {"NO", "YES", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}// 2
#define G10P05_LIST {"œ” 1", "œ” 2", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}//3
#define G10P06_LIST {"REMOTE", "LOCAL"}   // 21
#define G10P07_LIST {"HAND/AUTO", "PID"}  // 22

// adr, value, min, max, type, factor, pointer, name, listbase_id, units
#define G10P01 {0x0A00, 0, 0, 9, PAR_LIST, 1, 0, G10_P01_TEXT, 0, NUL}
#define G10P02 {0x0A01, 0, 0, 9, PAR_LIST, 1, 0, G10_P02_TEXT, 0, NUL}
#define G10P03 {0x0A02, 2, 0, 2, PAR_LIST, 1, 0, G10_P03_TEXT, 1, NUL}
#define G10P04 {0x0A03, 1, 0, 7, PAR_LIST, 1, 0, G10_P04_TEXT, 2, NUL}
#define G10P05 {0x0A04, 0, 0, 7, PAR_LIST, 1, 0, G10_P05_TEXT, 3, NUL}
#define G10P06 {0x0A05, 0, 0, 1, PAR_LIST, 1, 0, G10_P06_TEXT, 21, NUL}
#define G10P07 {0x0A06, 0, 0, 1, PAR_LIST, 1, 0, G10_P07_TEXT, 22, NUL}

#define G10_PARAMS {G10P01, G10P02, G10P03, G10P04, G10P05, G10P06, G10P07}
#define GROUP10 {PAR_NUMB_G10, GROUP_10_TEXT, G10_PARAMS, true}

// ================== √–”œœ¿ 11 ==========
//                       12345678901234567890
#define GROUP_11_TEXT   "11 ¬€¡Œ– «¿ƒ¿Õ»ﬂ"
#define PAR_NUMB_G11 7
#define G11_P01_TEXT	"01 «¿ƒ¿Õ»≈ — œ¿Õ≈À»"
#define G11_P02_TEXT	"02 œ”1 «¿ƒ¿Õ»≈"
#define G11_P03_TEXT	"03 œ”1 Ã»Õ»Ã”Ã"
#define G11_P04_TEXT	"04 œ”1 Ã¿ —»Ã”Ã"
#define G11_P05_TEXT	"05 œ”2 «¿ƒ¿Õ»≈"
#define G11_P06_TEXT	"06 œ”2 Ã»Õ»Ã”Ã"
#define G11_P07_TEXT	"07 œ”2 Ã¿ —»Ã”Ã"

#define G11P01_LIST {"REF1 Hz", "REF2 %"}//9
#define G11P03_LIST {"AI1", "AI2", "AI3",  "KEYPAD", "FIELDBUS"}//11

#define G11P01 {0x0B00,      0,    0,       1,      PAR_LIST,   1,      0,     G11_P01_TEXT,   8,    NUL}
#define G11P02 {0x0B01,      0,    0,       4,      PAR_LIST,   1,      0,     G11_P02_TEXT,   9,    NUL}
#define G11P03 {0x0B02,      0,    0,     500,      PAR_INT ,  10,      0,     G11_P03_TEXT,   0,     HZ}
#define G11P04 {0x0B03,    500,    0,    1000,      PAR_INT ,  10,    500,     G11_P04_TEXT,   0,     HZ}
#define G11P05 {0x0B04,      0,    0,       4,      PAR_LIST,   1,      0,     G11_P05_TEXT,   9,    NUL}
#define G11P06 {0x0B05,      0,    0,    1000,      PAR_INT ,  10,      0,     G11_P06_TEXT,   0,    PRC}
#define G11P07 {0x0B06,   1000,    0,    2000,      PAR_INT ,  10,   1000,     G11_P07_TEXT,   0,    PRC}

#define G11_PARAMS {G11P01, G11P02, G11P03, G11P04, G11P05, G11P06, G11P07}
#define GROUP11 {PAR_NUMB_G11, GROUP_11_TEXT, G11_PARAMS, true}

// ================== √–”œœ¿ 12 =============
//                       12345678901234567890
#define GROUP_12_TEXT   "12 œŒ—“ — Œ–Œ—“»"
#define PAR_NUMB_G12 9
#define G12_P01_TEXT	"01 ¬€¡Œ– œŒ—“ — Œ–"
#define G12_P02_TEXT	"02 œŒ—“ — Œ– 1"
#define G12_P03_TEXT	"03 œŒ—“ — Œ– 2"
#define G12_P04_TEXT	"04 œŒ—“ — Œ– 3"
#define G12_P05_TEXT	"05 œŒ—“ — Œ– 4"
#define G12_P06_TEXT	"06 œŒ—“ — Œ– 5"
#define G12_P07_TEXT	"07 œŒ—“ — Œ– 6"
#define G12_P08_TEXT	"08 œŒ—“ — Œ– 7"
#define G12_P09_TEXT	"09 œŒ—“ — Œ– 8"
#define G12P01_LIST {"NOT SEL", "DI1", "DI2", "DI3", "DI1,2", "DI3,4", "DI5,6", "DI1,2,3", "DI4,5,6"}//34
#define G12P01 {0x0C00,     0,    0,      8,      PAR_LIST,  1,      0,     G12_P01_TEXT,   34, NUL}
#define G12P02 {0x0C01,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P02_TEXT,   0, RPM}
#define G12P03 {0x0C02,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P03_TEXT,   0, RPM}
#define G12P04 {0x0C03,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P04_TEXT,   0, RPM}
#define G12P05 {0x0C04,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P05_TEXT,   0, RPM}
#define G12P06 {0x0C05,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P06_TEXT,   0, RPM}
#define G12P07 {0x0C06,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P07_TEXT,   0, RPM}
#define G12P08 {0x0C07,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P08_TEXT,   0, RPM}
#define G12P09 {0x0C08,     0,-6000,      6000,   PAR_INT ,  1,      0,     G12_P09_TEXT,   0, RPM}

#define G12_PARAMS {G12P01, G12P02, G12P03, G12P04, G12P05, G12P06, G12P07, G12P08, G12P09}
#define GROUP12 {PAR_NUMB_G12, GROUP_12_TEXT, G12_PARAMS, true}

// ================== √–”œœ¿ 13 =============
//                       12345678901234567890
#define GROUP_13_TEXT   "13 ¿Õ¿ÀŒ√Œ¬€≈ ¬’Œƒ€"
#define PAR_NUMB_G13 24
#define G13_P01_TEXT	"01 AI1 “»œ"
#define G13_P02_TEXT	"02 AI1 ¬–≈Ãﬂ ‘»À‹“–¿"
#define G13_P03_TEXT	"03 AI1 Ã¿—ÿ“¿¡ X1"
#define G13_P04_TEXT	"04 AI1 Ã¿—ÿ“¿¡ Y1"
#define G13_P05_TEXT	"05 AI1 Ã¿—ÿ“¿¡ X2"
#define G13_P06_TEXT	"06 AI1 Ã¿—ÿ“¿¡ Y2"
#define G13_P07_TEXT	"07 AI1 «ŒÕ¿ Õ≈◊”¬—“"
#define G13_P08_TEXT	"08 AI1 œŒ“≈–ﬂ —»√Õ"

#define G13_P09_TEXT "09 AI2 “»œ"
#define G13_P10_TEXT "10 AI2 ¬–≈Ãﬂ ‘»À‹“–¿"
#define G13_P11_TEXT "11 AI2 Ã¿—ÿ“¿¡ X1"
#define G13_P12_TEXT "12 AI2 Ã¿—ÿ“¿¡ Y1"
#define G13_P13_TEXT "13 AI2 Ã¿—ÿ“¿¡ X2"
#define G13_P14_TEXT "14 AI2 Ã¿—ÿ“¿¡ Y2"
#define G13_P15_TEXT "15 AI2 «ŒÕ¿ Õ≈◊”¬—“"
#define G13_P16_TEXT "16 AI2 œŒ“≈–ﬂ —»√Õ"

#define G13_P17_TEXT "17 AI3 “»œ"
#define G13_P18_TEXT "18 AI3 ¬–≈Ãﬂ ‘»À‹“–¿"
#define G13_P19_TEXT "19 AI3 Ã¿—ÿ“¿¡ X1"
#define G13_P20_TEXT "20 AI3 Ã¿—ÿ“¿¡ Y1"
#define G13_P21_TEXT "21 AI3 Ã¿—ÿ“¿¡ X2"
#define G13_P22_TEXT "22 AI3 Ã¿—ÿ“¿¡ Y2"
#define G13_P23_TEXT "23 AI3 «ŒÕ¿ Õ≈◊”¬—“"
#define G13_P24_TEXT "24 AI3 œŒ“≈–ﬂ —»√Õ"

#define G13P01_LIST {"CURRENT", "CURRENT W/C", "VOLTAGE", "VOLTAGE W/C"}// 14
#define G13P17_LIST {"CURRENT", "CURRENT W/C", "TEMP SENS"}// 36
#define G13P01 {0x1300,     2,         0,         3,         PAR_LIST,  1,         0,     G13_P01_TEXT, 14, NUL}
#define G13P02 {0x1301,    10,         0,      1000,         PAR_INT ,  1,         3,     G13_P02_TEXT, 0,  MSEC}
#define G13P03 {0x1302,     0,         0,       200,         PAR_INT , 10,         0,     G13_P03_TEXT, 0,  NUL}
#define G13P04 {0x1303,     0,    -10000,     10000,         PAR_INT , 10,         0,     G13_P04_TEXT, 0,  NUL}
#define G13P05 {0x1304,   100,         0,       200,         PAR_INT , 10,       100,     G13_P05_TEXT, 0,  NUL}
#define G13P06 {0x1305,  1000,    -10000,     10000,         PAR_INT , 10,       100,     G13_P06_TEXT, 0,  NUL}
#define G13P07 {0x1306,     0,         0,       500,         PAR_INT ,100,         0,     G13_P07_TEXT, 0,  NUL}
#define G13P08 {0x1307,    10,         0,      1000,         PAR_INT ,  1,        10,     G13_P08_TEXT, 0,  MSEC}

#define G13P09 {0x1308,     0,         0,         1,         PAR_LIST,  1,         0,     G13_P09_TEXT, 14, NUL}
#define G13P10 {0x1309,    10,         0,      1000,         PAR_INT ,  1,         3,     G13_P10_TEXT, 0,  MSEC}
#define G13P11 {0x130A,     0,         0,       200,         PAR_INT , 10,         0,     G13_P11_TEXT, 0,  NUL}
#define G13P12 {0x130B,     0,    -10000,     10000,         PAR_INT , 10,         0,     G13_P12_TEXT, 0,  NUL}
#define G13P13 {0x130C,   100,         0,       200,         PAR_INT , 10,       100,     G13_P13_TEXT, 0,  NUL}
#define G13P14 {0x130D,  1000,    -10000,     10000,         PAR_INT , 10,       100,     G13_P14_TEXT, 0,  NUL}
#define G13P15 {0x130E,     0,         0,       500,         PAR_INT ,100,         0,     G13_P15_TEXT, 0,  NUL}
#define G13P16 {0x130F,    10,         0,      1000,         PAR_INT ,  1,        10,     G13_P16_TEXT, 0,  MSEC}

#define G13P17 {0x1310,     0,         0,         2,         PAR_LIST,  1,         0,     G13_P17_TEXT, 36, NUL}
#define G13P18 {0x1311,    10,         0,      1000,         PAR_INT ,  1,         3,     G13_P18_TEXT, 0,  MSEC}
#define G13P19 {0x1312,     0,         0,       200,         PAR_INT , 10,         0,     G13_P19_TEXT, 0,  NUL}
#define G13P20 {0x1313,     0,    -10000,     10000,         PAR_INT , 10,         0,     G13_P20_TEXT, 0,  NUL}
#define G13P21 {0x1314,   100,         0,       200,         PAR_INT , 10,       100,     G13_P21_TEXT, 0,  NUL}
#define G13P22 {0x1315,  1000,    -10000,     10000,         PAR_INT , 10,       100,     G13_P22_TEXT, 0,  NUL}
#define G13P23 {0x1316,     0,         0,       500,         PAR_INT ,100,         0,     G13_P23_TEXT, 0,  NUL}
#define G13P24 {0x1317,    10,         0,      1000,         PAR_INT ,  1,        10,     G13_P24_TEXT, 0,  MSEC}

#define G13_PARAMS {G13P01, G13P02, G13P03, G13P04, G13P05, G13P06, G13P07, G13P08,\
                    G13P09, G13P10, G13P11, G13P12, G13P13, G13P14, G13P15, G13P16,\
                    G13P17, G13P18, G13P19, G13P20, G13P21, G13P22, G13P23, G13P24,\
                    }
#define GROUP13 {PAR_NUMB_G13, GROUP_13_TEXT, G13_PARAMS, true}

// ================== √–”œœ¿ 14 =============
//                       12345678901234567890
#define GROUP_14_TEXT	"14 –≈À≈…Õ€≈ ¬€’Œƒ€"
#define PAR_NUMB_G14 3
#define G14_P01_TEXT	"01 ¬€’Œƒ RO1"
#define G14_P02_TEXT	"02 ¬€’Œƒ RO2"
#define G14_P03_TEXT	"03 ¬€’Œƒ RO3"
#define G14P01_LIST {"NOT USED", "READY", "RUN", "FAULT", "FAULT (-1)", "ALARM"}// 15
#define G14P01 {0x0E00,    3,  0,   6,  PAR_LIST,  1,     0,   G14_P01_TEXT, 15, NUL}
#define G14P02 {0x0E01,    1,  0,   6,  PAR_LIST,  1,     0,   G14_P02_TEXT, 15, NUL}
#define G14P03 {0x0E02,    2,  0,   6,  PAR_LIST,  1,     0,   G14_P03_TEXT, 15, NUL}

#define G14_PARAMS {G14P01, G14P02, G14P03}
#define GROUP14 {PAR_NUMB_G14, GROUP_14_TEXT, G14_PARAMS, true}

// ================== √–”œœ¿ 15 =============
//                       12345678901234567890
#define GROUP_15_TEXT	"15 ¿Õ¿ÀŒ√Œ¬€≈ ¬€’Œƒ€"
#define PAR_NUMB_G15 10
#define G15_P01_TEXT	"01 AO1 ‘”Õ ÷»ﬂ"
#define G15_P02_TEXT	"02 AO1 Ã»Õ»Ã”Ã"
#define G15_P03_TEXT	"03 AO1 ‘»À‹“–"

#define G15_P04_TEXT	"04 AO2 ‘”Õ ÷»ﬂ"
#define G15_P05_TEXT	"05 AO2 Ã»Õ»Ã”Ã"
#define G15_P06_TEXT	"06 AO2 ‘»À‹“–"

#define G15_P07_TEXT	"07 — Œ–Œ—“‹ 20m¿"
#define G15_P08_TEXT	"08 ◊¿—“Œ“¿ 20 m¿"
#define G15_P09_TEXT	"09 “Œ  20 m¿"
#define G15_P10_TEXT	"10 Õ¿œ–ﬂ∆≈Õ»≈ 20 m¿"



#define G15P01_LIST {"NOT USED", "SPEED REF", "FREQUENCY", "CURRENT", "OUT VOLTAGE", "DC BUS VOLTAGE"}// 16
#define G15P02_LIST {"0 mA", "4 mA"}// 17

#define G15P01 {0x0F00,    0,     0,      5,     PAR_LIST,   1,    0,     G15_P01_TEXT,  16,  NUL}
#define G15P02 {0x0F01,    0,     0,      1,     PAR_LIST,   1,    0,     G15_P02_TEXT,  17, NUL}
#define G15P03 {0x0F02,    0,     0,  10000,     PAR_INT ,   1,    0,     G15_P03_TEXT,   0, MSEC}

#define G15P04 {0x0F03,    0,     0,      5,     PAR_LIST,   1,    0,     G15_P04_TEXT,  16,  NUL}
#define G15P05 {0x0F04,    0,     0,      1,     PAR_LIST,   1,    0,     G15_P05_TEXT,  17, NUL}
#define G15P06 {0x0F05,    0,     0,  10000,     PAR_INT ,   1,    0,     G15_P06_TEXT,   0, MSEC}

#define G15P07 {0x0F06,  3000,    0,   6000,     PAR_UINT,   1, 3000,     G15_P07_TEXT,   0, RPM}
#define G15P08 {0x0F07,    50,    0,    200,     PAR_UINT,   1,   50,     G15_P08_TEXT,   0,  HZ}
#define G15P09 {0x0F08,    50,    0,    100,     PAR_UINT,   1,   50,     G15_P09_TEXT,   0, AMP}
#define G15P10 {0x0F09,  1000,    0,   1000,     PAR_UINT,   1, 1000,     G15_P10_TEXT,   0, VOLT}



#define G15_PARAMS {G15P01, G15P02, G15P03, G15P04, G15P05, G15P06, G15P07, G15P08, G15P09, G15P10}
#define GROUP15 {PAR_NUMB_G15, GROUP_15_TEXT, G15_PARAMS, true}

// ================== √–”œœ¿ 17 =============
//                       12345678901234567890
#define GROUP_17_TEXT	"17 Õ¿—“–Œ… » ”œ–¿¬À"
#define PAR_NUMB_G17 4
//#define G17_P01_TEXT	"01 ¬–≈Ãﬂ ƒ–≈¡≈«√A"
#define G17_P01_TEXT	"01 –≈∆ œŒ“≈Õ÷»ŒÃ≈“–"
#define G17_P02_TEXT	"02 –≈∆»Ã  ÕŒœ » —“Œœ"
#define G17_P03_TEXT	"03 œ≈–≈’Œƒ ƒ»—“/Ã≈—“"
#define G17_P04_TEXT	"04 »—“Œ◊Õ»  ¿¬¿–»»"
#define G17_P05_TEXT	"05 FIELDBUS —“¿–“"
#define G17_P06_TEXT	"06 »—“ —¡–Œ—A ¿¬¿–»»"
#define G17_P07_TEXT	"07 œ≈–≈’Œƒ ƒ»—“/Ã≈—“"


//#define G17P01_LIST {"0.0 ms", "2.5 ms", "8.2 ms", "12.3 ms"}// 6
#define G17P01_LIST {"RESET TO ZERO", "RESTORE"}// 6
#define G17P02_LIST {"LOCAL",  "LOCAL/REMOTE"}// 29
#define G17P03_LIST {"STOP",   "OPERATION", "OPER & REFERENCE"}// 33
#define G17P04_LIST {"NOT SEL",  "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}// 38

#define G17P05_LIST {"NOT SEL",  "DI1P", "DI2P", "DI3P", "DI4P", "DI5P",}// 30
#define G17P06_LIST {"NO",  "YES"}// 31
#define G17P07_LIST {"LEVEL",  "FRONT"}// 32


#define G17P01 {0x1100, 0,    0,    1,    PAR_LIST,   1,    0,   G17_P01_TEXT,  6,   NUL}
#define G17P02 {0x1101, 0,    0,    1,    PAR_LIST,   1,    0,   G17_P02_TEXT,  29,  NUL}
#define G17P03 {0x1102, 0,    0,    2,    PAR_LIST,   1,    0,   G17_P03_TEXT,  33,  NUL}
#define G17P04 {0x1103, 0,    0,    6,    PAR_LIST,   1,    0,   G17_P04_TEXT,  38,  NUL}
#define G17P05 {0x1104, 0,    0,    5,    PAR_LIST,   1,    0,   G17_P05_TEXT,  30,  NUL}
#define G17P06 {0x1105, 0,    0,    1,    PAR_LIST,   1,    0,   G17_P06_TEXT,  31,  NUL}
#define G17P07 {0x1106, 0,    0,    1,    PAR_LIST,   1,    0,   G17_P07_TEXT,  32,  NUL}

//#define G17_PARAMS {G17P01, G17P02, G17P03, G17P04, G17P05, G17P06, G17P07}
#define G17_PARAMS {G17P01, G17P02, G17P03, G17P04}
#define GROUP17 {PAR_NUMB_G17, GROUP_17_TEXT, G17_PARAMS, true}


// ================== √–”œœ¿ 20 =============
//                       12345678901234567890
#define GROUP_20_TEXT	"20 œ–≈ƒ≈À€"
#define PAR_NUMB_G20 6
#define G20_P01_TEXT	"01 Ã»Õ — Œ–Œ—“‹"
#define G20_P02_TEXT	"02 Ã¿ — — Œ–Œ—“‹"
#define G20_P03_TEXT	"03 Ã¿ — “Œ "
#define G20_P04_TEXT	"04 Ã¿ — ÃŒÃ≈Õ“"
#define G20_P05_TEXT	"05 Ã»Õ Õ¿œ–ﬂ∆≈Õ»≈"
#define G20_P06_TEXT	"06 Ã¿ — Õ¿œ–ﬂ∆≈Õ»≈"
#define G20P01 {0x1400, -3000, -6000,    0, PAR_INT, 1, -3000, G20_P01_TEXT, 0,RPM}
#define G20P02 {0x1401,  3000,     0, 6000, PAR_INT, 1,  3000, G20_P02_TEXT, 0,RPM}
#define G20P03 {0x1402,   100,    50,  250, PAR_INT, 1,   100, G20_P03_TEXT, 0,PRC}
#define G20P04 {0x1403,   100,    50,  250, PAR_INT, 1,   100, G20_P04_TEXT, 0,PRC}
#define G20P05 {0x1404,    85,    70,  100, PAR_INT, 1,    85, G20_P05_TEXT, 0,PRC}
#define G20P06 {0x1405,   115,   111,  125, PAR_INT, 1,   115, G20_P06_TEXT, 0,PRC}

#define G20_PARAMS {G20P01, G20P02, G20P03, G20P04, G20P05, G20P06}
#define GROUP20 {PAR_NUMB_G20, GROUP_20_TEXT, G20_PARAMS, true}


// ================== √–”œœ¿ 21 =============
//                       12345678901234567890
#define GROUP_21_TEXT	"21 ”œ–. ƒ¬»√¿“≈À≈Ã"
#define PAR_NUMB_G21 4
#define G21_P01_TEXT	"01 –≈∆»Ã ”œ–¿¬À≈Õ»ﬂ"
#define G21_P02_TEXT	"02 “»œ ”œ–¿¬À≈Õ»ﬂ"
#define G21_P03_TEXT	"03 —œŒ—Œ¡ “Œ–ÃŒ∆≈Õ»ﬂ"
#define G21_P04_TEXT	"04 ◊¿—“Œ“¿ ÿ»Ã"

#define G21P01_LIST {"SCALAR", "VECTOR", "DTC", "EMF CONTROL"}                                      // 10
#define G21P02_LIST {"SPEED CONTROL", "TORQUE CONTROL"}                                             // 11
#define G21P03_LIST {"COST", "RAMP"}                                                                // 12
#define G21P04_LIST {"2kHz", "3kHz", "4kHz", "5kHz", "6kHz", "8kHz", "10kHz", "12kHz", "16kHz"}     // 13

#define G21P01 {0x1500,   1,  0,     3,  PAR_LIST,  1,     1,   G21_P01_TEXT,  10,NUL}
#define G21P02 {0x1501,   0,  0,     1,  PAR_LIST,  1,     0,   G21_P02_TEXT,  11,NUL}
#define G21P03 {0x1502,   0,  0,     1,  PAR_LIST,  1,     0,   G21_P03_TEXT,  12,NUL}
#define G21P04 {0x1503,   3,  0,     8,  PAR_LIST,  1,     3,   G21_P04_TEXT,  13,NUL}


#define G21_PARAMS {G21P01, G21P02, G21P03, G21P04}
#define GROUP21 {PAR_NUMB_G21, GROUP_21_TEXT, G21_PARAMS, true}


// adr, value, min, max, type, factor, pointer, name, listbase_id, units
// ================== √–”œœ¿ 22 =============
//                       12345678901234567890
#define GROUP_22_TEXT	"22 ”— Œ–/“Œ–ÃŒ∆≈Õ»≈"
#define PAR_NUMB_G22 8
#define G22_P01_TEXT	"01 ¬€¡ œ¿–€ ”— /«¿Ã"
#define G22_P02_TEXT	"02 ¬–≈Ãﬂ ”— Œ–≈Õ»ﬂ 1"
#define G22_P03_TEXT	"03 ¬–≈Ãﬂ «¿Ã≈ƒÀ≈Õ 1"
#define G22_P04_TEXT	"04 ¬–≈Ãﬂ ”— Œ–≈Õ»ﬂ 2"
#define G22_P05_TEXT	"05 ¬–≈Ãﬂ «¿Ã≈ƒÀ≈Õ 2"
#define G22_P06_TEXT	"06  –»¬¿ﬂ –¿«√ŒÕ¿"
#define G22_P07_TEXT	"07 ‘Œ–Ã¿ S- –»¬Œ…"
#define G22_P08_TEXT	"08 ¬–≈Ãﬂ «¿Ã ¿¬¿–"
#define G22P01_LIST {"ACC/DEC 1", "ACC/DEC 2","DI1", "DI2", "DI3", "DI4", "DI5"}// 7
#define G22P06_LIST {"LINEAR", "S-CURVE"}// 39
#define G22P07_LIST {"S01-CURVE", "S02-CURVE","S03-CURVE"}// 40
#define G22P01 {0x1600,    0,  0,     6,  PAR_LIST,  1,      0,   G22_P01_TEXT,  7,NUL}
#define G22P02 {0x1601,   20,  0,  6000,  PAR_INT ,  10,    20,   G22_P02_TEXT,  0,SEC}
#define G22P03 {0x1602,   20,  0,  6000,  PAR_INT ,  10,    20,   G22_P03_TEXT,  0,SEC}
#define G22P04 {0x1603,   40,  0,  6000,  PAR_INT ,  10,    40,   G22_P04_TEXT,  0,SEC}
#define G22P05 {0x1604,   40,  0,  6000,  PAR_INT ,  10,    40,   G22_P05_TEXT,  0,SEC}
#define G22P06 {0x1605,    0,  0,     1,  PAR_LIST ,  1,     0,   G22_P06_TEXT, 39,NUL}
#define G22P07 {0x1606,    1,  0,     2,  PAR_LIST ,  1,     1,   G22_P07_TEXT, 40,NUL}
#define G22P08 {0x1607, 2000,  0, 60000,  PAR_UINT,  100, 2000,   G22_P08_TEXT,  0,SEC}

#define G22_PARAMS {G22P01, G22P02, G22P03, G22P04, G22P05, G22P06, G22P07, G22P08}
#define GROUP22 {PAR_NUMB_G22, GROUP_22_TEXT, G22_PARAMS, true}


// ================== √–”œœ¿ 30 =============
//                       12345678901234567890
#define GROUP_30_TEXT	"30 ‘”Õ ÷»» «¿Ÿ»“"
#define PAR_NUMB_G30 4
#define G30_P01_TEXT	"01 Œ¡–€¬ AI"
#define G30_P02_TEXT	"02 —¬ﬂ«‹ MODBUS"
#define G30_P03_TEXT	"03 —¬ﬂ«‹ œ”À‹“"
#define G30_P04_TEXT	"04 “≈Ãœ. IGBT"
#define G30P01_LIST {"NO ACTION", "FAULT", "STOP", "CONST SPEED 8"}// 37
#define G30P01 {0x3000,   1,  0,     3,  PAR_LIST,  1,     1,   G30_P01_TEXT, 37,NUL}
#define G30P02 {0x3001,   0,  0,     3,  PAR_LIST,  1,     0,   G30_P02_TEXT, 37,NUL}
#define G30P03 {0x3002,   1,  0,     3,  PAR_LIST,  1,     1,   G30_P03_TEXT, 37,NUL}
#define G30P04 {0x3003,   0,  0,     3,  PAR_LIST,  1,     0,   G30_P04_TEXT, 37,NUL}

#define G30_PARAMS {G30P01, G30P02, G30P03, G30P04}
#define GROUP30 {PAR_NUMB_G30, GROUP_30_TEXT, G30_PARAMS, true}



// ================== √–”œœ¿ 31 =============
//                       12345678901234567890
#define GROUP_31_TEXT	"31 ¿¬“Œ œ≈–≈«¿œ”— "
#define PAR_NUMB_G31 7
#define G31_P01_TEXT	"01  ŒÀ»◊ œŒœ€“Œ "
#define G31_P02_TEXT	"02 ¬–≈Ãﬂ ¿œ¬"
#define G31_P03_TEXT	"03 «¿ƒ≈–∆ ¿ ¿œ¬"
#define G31_P04_TEXT	"04 œ≈–≈√–”« ¿"
#define G31_P05_TEXT	"05 Ã»Õ Õ¿œ–ﬂ∆≈Õ»≈"
#define G31_P06_TEXT	"06 Ã¿ — Õ¿œ–ﬂ∆≈Õ"
#define G31_P07_TEXT	"07 Ã»Õ —»√Õ¿À AI"
#define G31P04_LIST {"NO", "YES"}// 4
#define G31P01 {0x1F00, 4,    0,   10,    PAR_INT ,  1,    4,     G31_P01_TEXT,      0,NUL}
#define G31P02 {0x1F01, 40,   1,  6000,   PAR_INT ,  10,  40,     G31_P02_TEXT,      0,SEC}
#define G31P03 {0x1F02, 3,    0,  6000,   PAR_INT ,  10,   3,     G31_P03_TEXT,      0,SEC}
#define G31P04 {0x1F03, 0,    0,    1,    PAR_LIST,  1,    0,     G31_P04_TEXT,      4,NUL}
#define G31P05 {0x1F04, 1,    0,    1,    PAR_LIST,  1,    0,     G31_P05_TEXT,      4,NUL}
#define G31P06 {0x1F05, 0,    0,    1,    PAR_LIST,  1,    0,     G31_P06_TEXT,      4,NUL}
#define G31P07 {0x1F06, 0,    0,    1,    PAR_LIST,  1,    0,     G31_P07_TEXT,      4,NUL}

#define G31_PARAMS {G31P01, G31P02, G31P03, G31P04, G31P05, G31P06, G31P07}
#define GROUP31 {PAR_NUMB_G31, GROUP_31_TEXT, G31_PARAMS, true}

// ================== √–”œœ¿ 40 =============
//                       12345678901234567890
#define GROUP_40_TEXT	"40 œ»ƒ-–≈√”Àﬂ“Œ–"
#define PAR_NUMB_G40 18
#define G40_P01_TEXT	"01 œ»ƒ KP"
#define G40_P02_TEXT	"02 œ»ƒ KI"
#define G40_P03_TEXT	"03 œ»ƒ KD"
#define G40_P04_TEXT	"04 ‘»À‹“– ƒ»‘‘"
#define G40_P05_TEXT	"05 »Õ¬≈–“ Œÿ»¡"
#define G40_P06_TEXT	"06 ¬€¡Œ– —»√Õ Œ—"
#define G40_P07_TEXT	"07 ¬’Œƒ —»√Õ Œ—1"
#define G40_P08_TEXT	"08 ¬’Œƒ —»√Õ Œ—2"
#define G40_P09_TEXT	"09 —»√Õ Œ—1 Ã»Õ"
#define G40_P10_TEXT	"10 —»√Õ Œ—1 Ã¿ —"
#define G40_P11_TEXT	"11 —»√Õ Œ—2 Ã»Õ"
#define G40_P12_TEXT	"12 —»√Õ Œ—2 Ã¿ —"
#define G40_P13_TEXT	"13 ‘»À‹“– —»√Õ Œ—"
#define G40_P14_TEXT	"14 ¬€¡Œ– ‘”Õ . —ŒÕ"
#define G40_P15_TEXT	"15 ”–Œ¬≈Õ‹ ¿ “»¬"
#define G40_P16_TEXT	"16 «¿ƒ≈–∆ ¿ ¿ “»¬"
#define G40_P17_TEXT	"17 ”–Œ¬≈Õ‹ Œ“ À"
#define G40_P18_TEXT	"18 «¿ƒ≈–∆ ¿ Œ“ À"



#define G40P05_LIST {"NO", "YES"}// 18
#define G40P06_LIST {"ACT1", "ACT1-ACT2", "ACT1+ACT2", "ACT1*ACT2", "ACT1/ACT2", "MIN(ACT1, ACT2)", "MAX(ACT1, ACT2)"}// 19
#define G40P07_LIST {"AI1", "AI2", "AI3", "POWER"}// 20
#define G40P14_LIST {"OFF", "INTERNAL", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}// 5
#define G40P01 {0x2800,    10,     1,   1000,     PAR_UINT,   10,      100,     G40_P01_TEXT,  0, NUL}
#define G40P02 {0x2801,    10,     1,   1000,     PAR_UINT,   10,        1,     G40_P02_TEXT,  0, NUL}
#define G40P03 {0x2802,     0,     0,   1000,     PAR_UINT,   10,        0,     G40_P03_TEXT,  0, NUL}
#define G40P04 {0x2803,   100,     4,   1000,     PAR_UINT,  100,        0,     G40_P04_TEXT,  0, SEC}
#define G40P05 {0x2804,     0,     0,      1,     PAR_LIST,    1,        0,     G40_P05_TEXT, 18, NUL}
#define G40P06 {0x2805,     0,     0,      6,     PAR_LIST,    1,        0,     G40_P06_TEXT, 19, NUL}
#define G40P07 {0x2806,     1,     0,      3,     PAR_LIST,    1,        0,     G40_P07_TEXT, 20, NUL}
#define G40P08 {0x2807,     1,     0,      3,     PAR_LIST,    1,        1,     G40_P08_TEXT, 20, NUL}
#define G40P09 {0x2808,     0, -1000,   1000,     PAR_INT ,   10,        0,     G40_P09_TEXT,  0, PRC}
#define G40P10 {0x2809,  1000, -1000,   1000,     PAR_INT ,   10,      100,     G40_P10_TEXT,  0, PRC}
#define G40P11 {0x280A,     0, -1000,   1000,     PAR_INT ,   10,        0,     G40_P11_TEXT,  0, PRC}
#define G40P12 {0x280B,  1000, -1000,   1000,     PAR_INT ,   10,      100,     G40_P12_TEXT,  0, PRC}
#define G40P13 {0x280C,     4,     0,   1000,     PAR_UINT,  100,        0,     G40_P13_TEXT,  0, SEC}
#define G40P14 {0x280D,     0,     0,      7,     PAR_LIST,    1,        0,     G40_P14_TEXT,  5, NUL}
#define G40P15 {0x280E,     0,     0,  60000,     PAR_UINT,   10,        0,     G40_P15_TEXT,  0, RPM}
#define G40P16 {0x280F,     0,     0,  36000,     PAR_UINT,   10,        0,     G40_P16_TEXT,  0, SEC}
#define G40P17 {0x2810,     0,     0,   1000,     PAR_UINT,   10,        0,     G40_P17_TEXT,  0, PRC}
#define G40P18 {0x2811,     0,     0,  36000,     PAR_UINT,   10,        0,     G40_P18_TEXT,  0, SEC}


#define G40_PARAMS {G40P01, G40P02, G40P03, G40P04, G40P05, G40P06, G40P07, G40P08, G40P09, G40P10,\
                    G40P11, G40P12, G40P13, G40P14, G40P15, G40P16, G40P17, G40P18}
#define GROUP40 {PAR_NUMB_G40, GROUP_40_TEXT, G40_PARAMS, true}








// ================== √–”œœ¿ 52 =============
//                       12345678901234567890
#define GROUP_52_TEXT	"52 MODBUS"
#define PAR_NUMB_G52 6
#define G52_P01_TEXT	"01 ¿ƒ–≈— ”—“–Œ…—“¬¿"
#define G52_P02_TEXT	"02 — Œ–Œ—“‹"
#define G52_P03_TEXT	"03 ◊≈“ÕŒ—“‹"
#define G52_P04_TEXT	"04 —“Œœ ¡»“"
#define G52_P05_TEXT	"05 –≈∆»Ã œ≈–≈ƒ¿◊»"
#define G52_P06_TEXT	"06 “¿…Ã¿”“"
#define G52P02_LIST {"9600", "19200"} //24
#define G52P03_LIST {"NONE", "EVEN", "ODD"} //25
#define G52P04_LIST {"ONE", "TWO"} //26
#define G52P08_LIST {"SINGLE", "RESERVED", "DUBLICATED", "SPLIT"} //27
#define G52P01 {0x3400, 1, 1,   247,   PAR_INT ,  1,   1, G52_P01_TEXT,  0, NUL}
#define G52P02 {0x3401, 1, 0,     1,   PAR_LIST,  1,   1, G52_P02_TEXT, 24, "bit/s"}
#define G52P03 {0x3402, 0, 0,     2,   PAR_LIST,  1,   0, G52_P03_TEXT, 25, NUL}
#define G52P04 {0x3403, 0, 0,     1,   PAR_LIST,  1,   0, G52_P04_TEXT, 26, NUL}
#define G52P05 {0x3404, 0, 0,     3,   PAR_LIST,  1,   0, G52_P05_TEXT, 27, NUL}
#define G52P06 {0x3405,15, 1,  1200,   PAR_INT,  10,  15, G52_P06_TEXT,  0, SEC}

#define G52_PARAMS {G52P01, G52P02, G52P03, G52P04, G52P05, G52P06}
#define GROUP52 {PAR_NUMB_G52, GROUP_52_TEXT, G52_PARAMS, true}


// ================== √–”œœ¿ 64 ==========
#define GROUP_64_TEXT "64 USB CONTROL"
#define PAR_NUMB_G64 3
#define G64_P01_TEXT "01 CONTROL WORD"
#define G64_P02_TEXT "02 CONTROL WORD1"
#define G64_P03_TEXT "03 «¿ƒ¿Õ»≈"


// adr, value, min, max, type, factor, pointer, name, listbase_id, units
#define G64P01 {0x4000, 0,       0,      1,   PAR_INT,    1,   0,    G64_P01_TEXT, 0, NUL}
#define G64P02 {0x4001, 0,       0,      1,   PAR_INT,    1,   0,    G64_P02_TEXT, 0, NUL}
#define G64P03 {0x4002, 0,  -10000,  10000,   PAR_INT,  100,   0,    G64_P03_TEXT, 0, HZ}
//#define G00P10 {0x0009, 0, -100, 100, PAR_INT,   1,  0,    G00_P10_TEXT, 0, PRC}

#define G64P03_PRC {0x4002, 0, -20000, 20000, PAR_INT, 100, 0, G64_P03_TEXT, 0, PRC}

#define G64_PARAMS {G64P01, G64P02, G64P03}
#define GROUP64 {PAR_NUMB_G64, GROUP_64_TEXT, G64_PARAMS, false}





// ================== √–”œœ¿ 65 ==========
#define GROUP_65_TEXT "65 PROCESS DATA IN"
#define PAR_NUMB_G65 3
#define G65_P01_TEXT "01 CONTROL WORD"
#define G65_P02_TEXT "02 CONTROL WORD1"
#define G65_P03_TEXT "03 «¿ƒ¿Õ œŒ ◊¿—“"

// adr, value, min, max, type, factor, pointer, name, listbase_id, units
#define G65P01 {0x4100, 0,       0,      1,   PAR_INT,    1,   0,    G65_P01_TEXT, 0, NUL}
#define G65P02 {0x4101, 0,       0,      1,   PAR_INT,    1,   0,    G65_P02_TEXT, 0, NUL}
#define G65P03 {0x4102, 0,  -20000,  20000,   PAR_INT,  100,   0,    G65_P03_TEXT, 0, PRC}
//#define G00P10 {0x0009, 0, -100, 100, PAR_INT,   1,  0,    G00_P10_TEXT, 0, PRC}

#define G65P03_RPM {0x4102, 0, -10000, 10000, PAR_INT, 100, 0, G65_P03_TEXT, 0, HZ}

#define G65_PARAMS {G65P01, G65P02, G65P03}
#define GROUP65 {PAR_NUMB_G65, GROUP_65_TEXT, G65_PARAMS, false}



// ================== √–”œœ¿ 66 ==========
#define GROUP_66_TEXT "66 PROCESS DATA OUT"
#define PAR_NUMB_G66 3
#define G66_P01_TEXT "01 STATUS WORD"
#define G66_P02_TEXT "02 STATUS WORD1"
#define G66_P03_TEXT "03 FAULT WORD"

// adr, value, min, max, type, factor, pointer, name, listbase_id, units
#define G66P01 {0x4200, 0,    0, 1,   PAR_INT, 1, 0,    G66_P01_TEXT, 0, NUL}
#define G66P02 {0x4201, 0,    0, 1,   PAR_INT, 1, 0,    G66_P02_TEXT, 0, NUL}
#define G66P03 {0x4202, 0, -100, 100, PAR_INT, 1, 0,    G66_P03_TEXT, 0, PRC}

#define G66_PARAMS {G66P01, G66P02, G66P03}
#define GROUP66 {PAR_NUMB_G66, GROUP_66_TEXT, G66_PARAMS, false}




// ================== √–”œœ¿ 91 ==========
#define GROUP_91_TEXT    "91 ƒ»Õ¿Ã»◊≈— ¿ﬂ  Œ––"

// ================== √–”œœ¿ 92 ==========
#define GROUP_92_TEXT    "92 Õ¿¡Àﬁƒ¿“ œŒ“Œ ¿"

// ================== √–”œœ¿ 93 ==========
#define GROUP_93_TEXT    "93 –≈√”Àﬂ“Œ–¿ ›ƒ—"

// ================== √–”œœ¿ 95 ==========
#define GROUP_95_TEXT    "95 –≈√”À ¿ “»¬ “Œ ¿"

// ================== √–”œœ¿ 96 ==========
#define GROUP_96_TEXT    "96 –≈√”À –≈¿ “ “Œ ¿"

/*
// ================== √–”œœ¿ 97 =============
#define GROUP_97_TEXT	"97 –≈√”Àﬂ“Œ– “Œ ¿"
#define PAR_NUMB_G97 7
#define G97_P01_TEXT	"01 «¿ƒ¿Õ»≈ “Œ ¿"
#define G97_P02_TEXT	"02  Œ›‘»÷ ”—»À"
#define G97_P03_TEXT	"03  Œ›‘»÷ »Õ“≈√–"
#define G97_P04_TEXT	"04 Ã»Õ»Ã"
#define G97_P05_TEXT	"05 Ã¿ —»Ã"
#define G97_P06_TEXT	"06 œ–≈ƒ¬ Õ¿Ã¿√Õ"
#define G97_P07_TEXT	"07 –¿—◊ “Œ  Õ¿Ã"
#define G97P06_LIST {"OFF", "ON"}// 8
#define G97P01 {0x9700,   400,       0,   1000,  PAR_INT ,   10,      400,   G97_P01_TEXT, 0,     AMP}
#define G97P02 {0x9701,    50,       0,  10000,  PAR_INT ,  100,       50,   G97_P02_TEXT, 0,     NUL}
#define G97P03 {0x9702,    10,       0,  50000,  PAR_UINT,   10,       10,   G97_P03_TEXT, 0,     NUL}
#define G97P04 {0x9703, -1000,   -1000,   1000,  PAR_INT ,   10,    -1000,   G97_P04_TEXT, 0,     NUL}
#define G97P05 {0x9704,  1000,   -1000,   1000,  PAR_INT ,   10,     1000,   G97_P05_TEXT, 0,     NUL}
#define G97P06 {0x9705,     1,       0,      1,  PAR_LIST,    1,        0,   G97_P06_TEXT, 8,     NUL}
#define G97P07 {0x9706,     1,       0,      1,  PAR_LIST,    1,        0,   G97_P07_TEXT, 8,     NUL}

#define G97_PARAMS {G97P01, G97P02, G97P03, G97P04, G97P05, G97P06, G97P07}
#define GROUP97 {PAR_NUMB_G97, GROUP_97_TEXT, G97_PARAMS}
*/

// ================== √–”œœ¿ 98 ==========
#define GROUP_98_TEXT    "98 œ–Œ‘»À‹ ¬/√÷"

// ================== √–”œœ¿ 99 ==========
//                       12345678901234567890
#define GROUP_99_TEXT	"99 œ¿–¿Ã≈“–€ ÃŒ“Œ–¿"
#define PAR_NUMB_G99 8
#define G99_P01_TEXT	"01 ÃŒƒ≈À‹ ƒ¬»√¿“≈Àﬂ"
#define G99_P02_TEXT	"02 ÕŒÃ Õ¿œ–ﬂ∆≈Õ»≈"
#define G99_P03_TEXT	"02 ÕŒÃ ◊¿—“Œ“¿"
#define G99_P04_TEXT	"03 ÕŒÃ ÃŒŸÕŒ—“‹"
#define G99_P05_TEXT	"04 ÕŒÃ — Œ–Œ—“‹"
#define G99_P06_TEXT	"05 ÕŒÃ “Œ "
#define G99_P07_TEXT	"06 ÕŒÃ  œƒ"
#define G99_P08_TEXT	"07 ÕŒÃ COS PHI"
#define G99P01_LIST {"¿»–160S2(15k)", "¿»–132M2(11k)", "A»–112M2(7k5)", "¿»–100L2(5k5)", "¿»–100S2(4k)", "¿»–90L2(3k)", "¿»–160M2(18k5)", "¿»–132S4(7k5)", "¿»–80B4(1k5)"}//41
#define G99P01 {0x6100,   0,     0,     10,  PAR_LIST ,    1,     0,    G99_P01_TEXT,    41,NUL}
#define G99P02 {0x6101, 380,     0,   1000,   PAR_INT ,    1,   380,    G99_P02_TEXT,    0,VOLT}
#define G99P03 {0x6102,  50,     0,    100,   PAR_INT ,    1,    50,    G99_P03_TEXT,      0,HZ}
#define G99P04 {0x6103, 220,     0,   1000,   PAR_UINT,   10,   110,    G99_P04_TEXT,     0,KWT}
#define G99P05 {0x6104, 1500,    0,   6000,   PAR_INT ,    1,  1500,    G99_P05_TEXT,     0,RPM}
#define G99P06 {0x6105, 110,     0,   3000,   PAR_INT ,  100,   219,    G99_P06_TEXT,     0,AMP}
#define G99P07 {0x6106,  92,     0,   1000,   PAR_UINT,   10,    88,    G99_P07_TEXT,     0,PRC}
#define G99P08 {0x6107, 970,     0,   1000,   PAR_UINT,  100,    88,    G99_P08_TEXT,     0,NUL}

#define G99_PARAMS {G99P01, G99P02, G99P03, G99P04, G99P05, G99P06, G99P07, G99P08}
#define GROUP99 {PAR_NUMB_G99, GROUP_99_TEXT, G99_PARAMS}

#define GROUP_FAST_TEXT "¡€—“–¿ﬂ Õ¿—“–Œ… ¿"
#define GROUP_FAST_PAR_NUMB 14
#define GROUP_FAST {GROUP_FAST_PAR_NUMB, GROUP_FAST_TEXT, GROUP_FAST_PARAMS}
//#define GROUP_FAST_PARAMS {G99P01, G99P02, G99P03, G99P04, G99P05, G99P06, G20P01, G20P02, G22P02, G22P03, G10P01, G10P03, G11P02, G10P07} // G97P06, G91P03}
#define GROUP_FAST_PARAMS {G99P01, G99P02, G99P03, G99P04, G99P05, G99P06, G20P01, G20P02, G22P02, G22P03, G10P01, G10P03, G11P02} // G97P06, G91P03}

// ========================================================================================================
// ================== ARCHIVE =============================================================================
// ========================================================================================================
#define EVENTS_PARAM_NUMBER 10
#define ARCHIVE_TEXT    "¿–’»¬ —Œ¡€“»…"
#define EVEVENTS_NUMBER 20                   //0           //1           //2             //3        //4               //5            //6            //7                   //8         //9                 //10        //11          //12
#define ARCHIVE_P01_TEXT "01 —Œ¡€“»≈"
#define ARCHIVE_P02_TEXT "02 —Œ¡€“»≈"
#define ARCHIVE_P03_TEXT "03 —Œ¡€“»≈"
#define ARCHIVE_P04_TEXT "04 —Œ¡€“»≈"
#define ARCHIVE_P05_TEXT "05 —Œ¡€“»≈"
#define ARCHIVE_P06_TEXT "06 —Œ¡€“»≈"
#define ARCHIVE_P07_TEXT "07 —Œ¡€“»≈"
#define ARCHIVE_P08_TEXT "08 —Œ¡€“»≈"
#define ARCHIVE_P09_TEXT "09 —Œ¡€“»≈"
#define ARCHIVE_P10_TEXT "10 —Œ¡€“»≈"
#define ARCHIVE_P11_TEXT "11 —Œ¡€“»≈"
#define ARCHIVE_P12_TEXT "12 —Œ¡€“»≈"
#define ARCHIVE_P13_TEXT "13 —Œ¡€“»≈"
#define ARCHIVE_P14_TEXT "14 —Œ¡€“»≈"
#define ARCHIVE_P15_TEXT "15 —Œ¡€“»≈"
#define ARCHIVE_P16_TEXT "16 —Œ¡€“»≈"
#define ARCHIVE_P17_TEXT "17 —Œ¡€“»≈"
#define ARCHIVE_P18_TEXT "18 —Œ¡€“»≈"
#define ARCHIVE_P19_TEXT "19 —Œ¡€“»≈"
#define ARCHIVE_P20_TEXT "20 —Œ¡€“»≈"
//                       12345678901234567890
#define ARCHIVE_LIST {	"Õ≈“ —Œ¡€“»ﬂ",\
						"¿œœ¿–¿“ Ã¿ — “Œ  «¿Ÿ",\
						"œ–Œ√–¿Ã Ã¿ — “Œ  «¿Ÿ",\
						"œ≈–≈√–”« ¿ œŒ “Œ ”",\
						"¿¬¿–»ﬂ ¬≈Õ“»Àﬂ“Œ–¿",\
						"œ≈–≈√–≈¬ IGBT",\
						"œ–≈¬€ÿ≈Õ»≈ DC Õ¿œ–ﬂ∆",\
						"œŒÕ»∆≈Õ»≈ DC Õ¿œ–ﬂ∆",\
						"Õ≈¬≈–Õ€≈ ƒ¿ÕÕ€≈ ƒ¬»√",\
						"Õ≈¬≈–Õ€≈ ƒ¿ÕÕ€≈ ƒ¬»√",\
						"¬Õ≈ÿÕ»… —»√Õ¿À ¿¬¿–",\
						"—¬ﬂ«‹ — œ”À‹“ŒÃ",\
						"—¬ﬂ«‹ œŒ MODBUS",\
						"AI < MIN",\
						"—¡–Œ— DI",\
						"—¡–Œ— œ”À‹“",\
						"—¡–Œ— ¿œ¬",\
						"—¡–Œ— MODBUS"}// 23
#define ARCHIVE_P01 {0x0300, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P01_TEXT, 23, NUL}
#define ARCHIVE_P02 {0x030A, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P02_TEXT, 23, NUL}
#define ARCHIVE_P03 {0x0314, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P03_TEXT, 23, NUL}
#define ARCHIVE_P04 {0x031E, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P04_TEXT, 23, NUL}
#define ARCHIVE_P05 {0x0328, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P05_TEXT, 23, NUL}
#define ARCHIVE_P06 {0x0332, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P06_TEXT, 23, NUL}
#define ARCHIVE_P07 {0x033C, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P07_TEXT, 23, NUL}
#define ARCHIVE_P08 {0x0346, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P08_TEXT, 23, NUL}
#define ARCHIVE_P09 {0x0350, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P09_TEXT, 23, NUL}
#define ARCHIVE_P10 {0x035A, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P10_TEXT, 23, NUL}
#define ARCHIVE_P11 {0x0364, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P11_TEXT, 23, NUL}
#define ARCHIVE_P12 {0x036E, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P12_TEXT, 23, NUL}
#define ARCHIVE_P13 {0x0378, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P13_TEXT, 23, NUL}
#define ARCHIVE_P14 {0x0382, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P14_TEXT, 23, NUL}
#define ARCHIVE_P15 {0x038C, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P15_TEXT, 23, NUL}
#define ARCHIVE_P16 {0x0396, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P16_TEXT, 23, NUL}
#define ARCHIVE_P17 {0x03A0, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P17_TEXT, 23, NUL}
#define ARCHIVE_P18 {0x03AA, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P18_TEXT, 23, NUL}
#define ARCHIVE_P19 {0x03B4, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P19_TEXT, 23, NUL}
#define ARCHIVE_P20 {0x03BE, 0, 0,  17,  PAR_ARCHIVE, 1, 0, ARCHIVE_P20_TEXT, 23, NUL}

#define ARCHIVE_PARAMS {ARCHIVE_P01, ARCHIVE_P02, ARCHIVE_P03, ARCHIVE_P04, ARCHIVE_P05, ARCHIVE_P06, ARCHIVE_P07, ARCHIVE_P08, ARCHIVE_P09, ARCHIVE_P10,\
                        ARCHIVE_P11, ARCHIVE_P12, ARCHIVE_P13, ARCHIVE_P14, ARCHIVE_P15, ARCHIVE_P16, ARCHIVE_P17, ARCHIVE_P18, ARCHIVE_P19, ARCHIVE_P20}

#define ARCHIVE_GROUP {EVEVENTS_NUMBER, ARCHIVE_TEXT, ARCHIVE_PARAMS}




// ========================================================================================================
// ================== GROUPS =============================================================================
// ========================================================================================================
#define GROUPS_NUMBER 18
#define ALL_GROUPS {GROUP10, GROUP11, GROUP12, GROUP13, GROUP14, GROUP15, GROUP17, GROUP20, GROUP21, GROUP22, GROUP31, GROUP40, GROUP52, GROUP64, GROUP65, GROUP66, GROUP99, MONITOR_GROUP }

//#define SAVED_GROUPS_NUMBER GROUPS_NUMBER - 1
//#define SAVED_GROUPS {GROUP10, GROUP11, GROUP12, GROUP13, GROUP14, GROUP15, GROUP17, GROUP20, GROUP22, GROUP30, GROUP31, GROUP40, GROUP52, MONITOR_GROUP }

#define MONITOR_GROUP_NUM GROUPS_NUMBER - 1

#define VOIDTEXT {""}

// G10P01_LIST {"NOT SEL", "DI1", "DI1, DI2", "DI1P, DI2P", "DI1P, DI2P, DI3", "DI1P, DI2P, DI3P", "DI6", "DI6, DI5", "KEYPAD", "FIELDBUS"}// 0
// G10P03_LIST {"FORWARD", "REVERSE", "REQUEST"}// 1
// G10P04_LIST {"NO", "YES", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}// 2
// G10P05_LIST {"œ” 1", "œ” 2", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"}//3
// G10P06_LIST {"REMOTE", "LOCAL"}   // 21
// G10P07_LIST {"HAND/AUTO", "PID"}  // 22

// G11P01_LIST {"REF1 Hz", "REF2 %"}                          //8
// G11P03_LIST {"AI1", "AI2", "AI3",  "KEYPAD", "FIELDBUS"}   //9

// G12P01_LIST {"NOT SEL", "DI1", "DI2", "DI3", "DI1,2", "DI3,4", "DI5,6", "DI1,2,3", "DI4,5,6"}//34

// G13P01_LIST {"CURRENT", "CURRENT W/C", "VOLTAGE", "VOLTAGE W/C"}           // 14
// G13P17_LIST {"CURRENT", "CURRENT W/C", "TEMP SENS"}                        // 36

// G14P01_LIST {"NOT USED", "READY", "RUN", "FAULT", "FAULT (-1)", "ALARM"}   // 15

// G15P01_LIST {"NOT USED", "SPEED REF", "FREQUENCY", "CURRENT", "OUT VOLTAGE", "DC BUS VOLTAGE"}// 16
// G15P02_LIST {"0 mA", "4 mA"}// 17

// G17P01_LIST {"RESET TO ZERO", "RESTORE"}// 6
// G17P02_LIST {"NOT SEL",  "DI1P", "DI2P", "DI3P", "DI4P", "DI5P",}// 29
// G17P03_LIST {"NOT SEL",  "DI1", "DI2", "DI3", "DI4", "DI5",}// 33
// G17P04_LIST {"NO",  "YES"}// 38
// G17P05_LIST {"LEVEL",  "FRONT"}// 30
// G17P06_LIST {"LOCAL",  "LOCAL/REMOTE"}// 31
// G17P07_LIST {"STOP",   "OPERATION", "OPER & REFERENCE"}// 32

// G21P01_LIST {"SCALAR", "VECTOR", "DTC", "EMF CONTROL"}                                      // 10
// G21P02_LIST {"SPEED CONTROL", "TORQUE CONTROL"}                                             // 11
// G21P03_LIST {"COST", "RAMP"}                                                                // 12
// G21P04_LIST {"2kHz", "3kHz", "4kHz", "5kHz", "6kHz", "8kHz", "10kHz", "12kHz", "16kHz"}     // 13

// G22P01_LIST {"ACC/DEC 1", "ACC/DEC 2","DI1", "DI2", "DI3", "DI4", "DI5"}// 7
// G22P06_LIST {"LINEAR", "S-CURVE"}// 39
// G22P07_LIST {"S01-CURVE", "S02-CURVE","S03-CURVE"}// 40

// G30P01_LIST {"NO ACTION", "FAULT", "STOP", "CONST SPEED 8"}// 37

// G31P04_LIST {"NO", "YES"}// 4



#define LIST_BASE_NUM 42
#define LIST_BASE \
/*id*/  {\
/*0*/  G10P01_LIST,\
/*1*/  G10P03_LIST,\
/*2*/  G10P04_LIST,\
/*3*/  G10P05_LIST,\
/*4*/  G31P04_LIST,\
/*5*/  G40P14_LIST,\
/*6*/  G17P01_LIST,\
/*7*/  G22P01_LIST,\
/*8*/  G11P01_LIST,\
/*9*/  G11P03_LIST,\
/*10*/ G21P01_LIST,\
/*11*/ G21P02_LIST,\
/*12*/ G21P03_LIST,\
/*13*/ G21P04_LIST,\
/*14*/ G13P01_LIST,\
/*15*/ G14P01_LIST,\
/*16*/ G15P01_LIST,\
/*17*/ G15P02_LIST,\
/*18*/ G40P05_LIST,\
/*19*/ G40P06_LIST,\
/*20*/ G40P07_LIST,\
/*21*/ G10P06_LIST,\
/*22*/ G10P07_LIST,\
/*23*/ ARCHIVE_LIST,\
/*24*/ G52P02_LIST,\
/*25*/ G52P03_LIST,\
/*26*/ G52P04_LIST,\
/*27*/ G52P08_LIST,\
/*28*/ G15P01_LIST,\
/*29*/ G17P02_LIST,\
/*30*/ G17P05_LIST,\
/*31*/ G17P06_LIST,\
/*32*/ G17P07_LIST,\
/*33*/ G17P03_LIST,\
/*34*/ G12P01_LIST,\
/*35*/ MONITOR_LIST,\
/*36*/ G13P17_LIST,\
/*37*/ G30P01_LIST,\
/*38*/ G17P04_LIST,\
/*39*/ G22P06_LIST,\
/*40*/ G22P07_LIST,\
/*41*/ G99P01_LIST,\
}

#endif
