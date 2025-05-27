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
tControlWord ControlWord;
tControlWord1 ControlWord1;

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
	true,
	true
};

//--------------------------------------------------------------------


//--------------------------Группа 11---------------------------------
/*char listG11P01[2][MAX_NAME_LENGTH] =  {"REF1 Hz", "REF2 %"};
tParam G11_P01 = {"01 ЗАДАНИЕ С ПУЛЬТА", 	0x0B00, 0, 0, 1, PAR_IS_LIST, 1, listG11P01[0], true, false, UNITS_VOID};

char listG11P02[5][MAX_NAME_LENGTH] =  {"AI1", "AI2", "AI3",  "KEYPAD", "FIELDBUS"};
tParam G11_P02 = {"02 ПУ1 ЗАДАНИЕ", 	0x0B01, 0, 0, 4, PAR_IS_LIST, 1, listG11P02[0], true, false, UNITS_VOID};

tParam G11_P03 = {"03 ПУ1 МИНИМУМ", 	0x0B02, 0, 0, 500, PAR_IS_INT, 10, NULL, true, false, UNITS_HZ};

tParam G11_P04 = {"04 ПУ1 МАКСИМУМ", 	0x0B03, 500, 0, 1000, PAR_IS_INT, 10, NULL, true, false, UNITS_HZ};

tParam G11_P05 = {"05 ПУ2 ЗАДАНИЕ", 	0x0B04, 0, 0, 4, PAR_IS_LIST, 1, listG11P02[0], true, false, UNITS_VOID};

tParam G11_P06 = {"06 ПУ2 МИНИМУМ", 	0x0B05, 0, 0, 1000, PAR_IS_INT, 10, NULL, true, false, UNITS_PROC};

tParam G11_P07 = {"07 ПУ2 МАКСИМУМ", 	0x0B06, 1000, 0, 2000, PAR_IS_INT, 10, NULL, true, false, UNITS_PROC};
*/

//--------------------------------------------------------------------






//--------------------------------------------------------------------
/*
* InitParameters - инициализация параметров
*/
void InitParameters(void)
{

	//--------------------------Группа 10---------------------------------

	//--------------------------------------------------------------------

}
//--------------------------------------------------------------------



