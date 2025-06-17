//
// Included Files
//
#include "parameters.h"
//
// Function Prototypes
//


//
// Global Variables
//
tControlWord ControlWord, ControlWordOld;
tControlWord1 ControlWord1;

int16_t Reference, ReferenceOld;

tStatusWord StatusWord;
tStatusWord1 StatusWord1;
tFaultWord FaultWord;
tFaultWord1 FaultWord1;
tAlarmWord AlarmWord;

//
// Local Variables
//


//--------------------------Группа 10---------------------------------
// Списки параметров (одной строкой с \0 разделителями)
static const char listG10P01[] = "NOT SEL\0DI1\0DI1, DI2\0DI1P, DI2P\0DI1P, DI2P, DI3\0DI1P, DI2P, DI3P\0DI6\0DI6, DI5\0KEYPAD\0FIELDBUS\0";
static const char listG10P03[] = "FORWARD\0REVERSE\0REQUEST";
static const char listG10P04[] =  "NO\0YES\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";
static const char listG10P05[] =  "ПУ 1\0ПУ 2\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";
static const char listG10P06[] =  "REMOTE\0LOCAL";
static const char listG10P07[] =  "HAND/AUTO\0PID";

// Параметры группы 10
static const tParam group10_params[] = {
	{"01 ПУ1 СТАРТ/СТОП",	 0x0A00, 0, 9, PAR_IS_LIST, 1, listG10P01, true, false, UNITS_VOID},
	{"02 ПУ2 СТАРТ/СТОП", 	 0x0A01, 0, 9, PAR_IS_LIST, 1, listG10P01, true, false, UNITS_VOID},
	{"03 НАПРАВЛЕНИЕ", 		 0x0A02, 0, 2, PAR_IS_LIST, 1, listG10P03, true, false, UNITS_VOID},
	{"04 РАЗРЕШЕНИЕ РАБОТЫ", 0x0A03, 0, 7, PAR_IS_LIST, 1, listG10P04, true, false, UNITS_VOID},
	{"05 ВЫБОР ПУ1/ПУ2", 	 0x0A04, 0, 7, PAR_IS_LIST, 1, listG10P05, true, false, UNITS_VOID},
	{"06 РЕЖИМ УПРАВЛЕНИЯ",  0x0A05, 0, 1, PAR_IS_LIST, 1, listG10P06, true, false, UNITS_VOID},
	{"07 РЕЖИМ РАБОТЫ", 	 0x0A06, 0, 1, PAR_IS_LIST, 1, listG10P07, true, false, UNITS_VOID}
};

// Группа параметров
const tGroup Group10 = {
    "10 ПУСК/СТОП/НАПРАВЛ",
    group10_params,
    sizeof(group10_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------


//--------------------------Группа 11---------------------------------
// Списки параметров (одной строкой с \0 разделителями)
static const char listG11P01[] =  "REF1 Hz\0REF2 %";
static const char listG11P02[] =  "AI1\0AI2\0AI3\0KEYPAD\0FIELDBUS";

// Параметры группы 11
static const tParam group11_params[] = {
	{"01 ЗАДАНИЕ С ПУЛЬТА",  0x0B00, 0, 1,    PAR_IS_LIST, 1,  listG11P01, true, false, UNITS_VOID},
	{"02 ПУ1 ЗАДАНИЕ", 	     0x0B01, 0, 4,    PAR_IS_LIST, 1,  listG11P02, true, false, UNITS_VOID},
	{"03 ПУ1 МИНИМУМ", 	     0x0B02, 0, 500,  PAR_IS_UINT,  10, NULL,       true, false, UNITS_HZ},
	{"04 ПУ1 МАКСИМУМ", 	 0x0B03, 0, 1000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_HZ},
	{"05 ПУ2 ЗАДАНИЕ", 	     0x0B04, 0, 4,    PAR_IS_LIST, 1,  listG11P02, true, false, UNITS_VOID},
	{"06 ПУ2 МИНИМУМ", 	     0x0B05, 0, 1000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_PROC},
	{"07 ПУ2 МАКСИМУМ", 	 0x0B06, 0, 2000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_PROC}
};

// Группа параметров
const tGroup Group11 = {
    "11 ВЫБОР ЗАДАНИЯ",
    group11_params,
    sizeof(group11_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Группа 12---------------------------------
// Списки параметров (одной строкой с \0 разделителями)
static const char listG12P01[] =  "NOT SEL\0DI1\0DI2\0DI3\0DI_12\0DI_34\0DI_56\0DI_123\0DI_456";

// Параметры группы 12
static const tParam group12_params[] = {
	{"01 ВЫБОР ПОСТ СКОР",  0x0C00, 			    0,    8,  PAR_IS_LIST, 1,  listG12P01, true, false, UNITS_VOID},
	{"02 ПОСТ СКОР 1", 	    0x0C01, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"03 ПОСТ СКОР 2", 	    0x0C02, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"04 ПОСТ СКОР 3", 	    0x0C03, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"05 ПОСТ СКОР 4", 	    0x0C04, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"06 ПОСТ СКОР 5", 	    0x0C05, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"07 ПОСТ СКОР 6", 	    0x0C06, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"08 ПОСТ СКОР 7", 	    0x0C07, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"09 ПОСТ СКОР 8", 	    0x0C08, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},

};

// Группа параметров
const tGroup Group12 = {
    "12 ПОСТ СКОРОСТИ",
    group12_params,
    sizeof(group12_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Группа 64---------------------------------
// Списки параметров (одной строкой с \0 разделителями)


// Параметры группы 64
static const tParam group64_params[] = {
	{"01 CONTROL WORD",  	0x4000, 			     0, 65535,  PAR_IS_UINT,   1, NULL, true, true, UNITS_VOID},
	{"02 CONTROL WORD1",  	0x4001,   			     0, 65535,  PAR_IS_UINT,   1, NULL, true, true, UNITS_VOID},
	{"03 REFERENCE",  		0x4002, (uint16_t)(-32767), 32767,  PAR_IS_INT,  100, NULL, true, true, UNITS_HZ}

};

// Группа параметров
const tGroup Group64 = {
    "64 ЗАДАНИЕ ПУЛЬТ",
    group64_params,
    sizeof(group64_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------


//--------------------------Мониторинг--------------------------------
// Списки параметров (одной строкой с \0 разделителями)


// Параметры Мониторинга
const tParam Monitor[] = {
	{"DC BUS VOLT",    	  0x4407,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"VOLTAGE",  	  	  0x4406,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"CURRENT", 	      0x4403,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_AMP},
	{"FREQUENCY", 	      0x4401,  (uint16_t)(-1000),  1000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_HZ},
	{"POWER", 		  	  0x4405, 		 	       0, 10000,  PAR_IS_INT, 100, NULL, false, false,  UNITS_KWT},
	{"IGBT TEMP",    	  0x4408,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_CELSIUM},
	{"SW",    	  		  0x4200,                  0, 65535,  PAR_IS_BIN,  16, NULL, false, false,  UNITS_VOID},
	{"SW HEX",    	  	  0x4200,                  0, 65535,  PAR_IS_HEX,   4, NULL, false, false,  UNITS_VOID},
	{"SW1",    	  		  0x4201,                  0, 65535,  PAR_IS_BIN,  16, NULL, false, false,  UNITS_VOID},
	{"SW1 HEX",    	  	  0x4201,                  0, 65535,  PAR_IS_HEX,   4, NULL, false, false,  UNITS_VOID},
	{"FW",    	  		  0x4202,                  0, 65535,  PAR_IS_BIN,  16, NULL, false, false,  UNITS_VOID},
	{"FW HEX",    	  	  0x4202,                  0, 65535,  PAR_IS_HEX,   4, NULL, false, false,  UNITS_VOID},
	{"FW1",    	  		  0x4203,                  0, 65535,  PAR_IS_BIN,  16, NULL, false, false,  UNITS_VOID},
	{"FW1 HEX",    	  	  0x4203,                  0, 65535,  PAR_IS_HEX,   4, NULL, false, false,  UNITS_VOID},
	{"AW",    	  		  0x4204,                  0, 65535,  PAR_IS_BIN,  16, NULL, false, false,  UNITS_VOID},
	{"AW HEX",    	  	  0x4204,                  0, 65535,  PAR_IS_HEX,   4, NULL, false, false,  UNITS_VOID}

};

const uint16_t MonitorCnt = sizeof(Monitor)/sizeof(tParam);

uint8_t MonitorSelect[3] = {2, 0, 5}; // значения Монитор 1, Монитор 2, Монитор 3
uint8_t MonitorNum = 0; // индекс выбранного мониторинга (Монитор 1, Монитор 2, Монитор 3)
uint8_t MonitorVal = 0;

//--------------------------------------------------------------------


//--------------------------Архив событий-----------------------------
// Списки параметров (одной строкой с \0 разделителями)


// Параметры Архива событий
/*const tParam Monitor[] = {
	{"DC BUS VOLT",    	  0x4407,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"VOLTAGE",  	  	  0x4406,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"CURRENT", 	      0x4403,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_AMP},
	{"FREQUENCY", 	      0x4401,  (uint16_t)(-1000),  1000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_HZ},
	{"POWER", 		  	  0x4405, 		 	       0, 10000,  PAR_IS_INT, 100, NULL, false, false,  UNITS_KWT},
	{"IGBT TEMP",    	  0x4408,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_CELSIUM}

};

const uint16_t MonitorCnt = sizeof(Monitor)/sizeof(tParam);*/

uint8_t EventNum = 0; // индекс просматриваемого события

//--------------------------------------------------------------------



const tGroup* const AllGroups[] = {&Group10, &Group11, &Group12, &Group64};
const uint16_t AllGroupsCnt = sizeof(AllGroups)/sizeof(tGroup*);

const tGroup* const MenuGroups[] = {&Group10, &Group11, &Group12};
const uint16_t MenuGroupsCnt = sizeof(MenuGroups)/sizeof(tGroup*);

const tParam* const FastSettings[] = {
	&group10_params[0], &group10_params[2],
	&group11_params[0], &group11_params[1],
	&group12_params[1], &group12_params[3]
};
const uint16_t FastSettingsCnt = sizeof(FastSettings)/sizeof(tParam*);
uint8_t FastSettingsNum = 0; // индекс выбранного параметра быстрых настроек

//--------------------------------------------------------------------
/*
* GetParamListItem - функция получения элемента списка по индексу.
*/
const char* GetParamListItem(const tParam* param, uint8_t index)
{
    if(param->type != PAR_IS_LIST) return NULL;

    const char* item = param->listItems;
    uint8_t current = 0;

    while(*item && current < index) {
        item += strlen(item) + 1;
        current++;
    }

    return *item ? item : NULL;
}

/*
// Пример использования:
const tParam* param = &Group10.params[0]; // Параметр со списком
const char* listItem = GetParamListItem(param, 2); // Получаем 3-й элемент (индекс 2)

*/

//--------------------------------------------------------------------



