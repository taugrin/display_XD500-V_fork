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
//ToDo: list_test нужен только на время отладки. Иначе компилятор выкидывает переменные
tParam Gxx_Pxx = {"xx НАЗВАНИЕ", 		0xEEEE, 0, 0, 9, 2, 1, listG10_P01[0], true, false};


//--------------------------Группа 10---------------------------------
/*const*/ char listG10_P01[10][MAX_NAME_LENGTH] = {"NOT SEL", "DI1", "DI1, DI2", "DI1P, DI2P", "DI1P, DI2P, DI3",
		"DI1P, DI2P, DI3P", "DI6", "DI6, DI5", "KEYPAD", "FIELDBUS"};
tParam G10_P01 = {"01 ПУ1 СТАРТ/СТОП", 	0x0A00, 0, 0, 9, PAR_IS_LIST, 1, listG10_P01[0], true, false, UNITS_VOID};

tParam G10_P02 = {"02 ПУ2 СТАРТ/СТОП", 	0x0A01, 0, 0, 9, PAR_IS_LIST, 1, listG10_P01[0], true, false, UNITS_VOID};

/*const*/ char listG10_P03[3][MAX_NAME_LENGTH] = {"FORWARD", "REVERSE", "REQUEST"};
tParam G10_P03 = {"03 НАПРАВЛЕНИЕ", 	0x0A02, 0, 0, 2, PAR_IS_LIST, 1, listG10_P03[0], true, false, UNITS_VOID};

char listG10P04[8][MAX_NAME_LENGTH] =  {"NO", "YES", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"};
tParam G10_P04 = {"04 РАЗРЕШЕНИЕ РАБОТЫ", 	0x0A03, 0, 0, 7, PAR_IS_LIST, 1, listG10P04[0], true, false, UNITS_VOID};

char listG10P05[8][MAX_NAME_LENGTH] =  {"ПУ 1", "ПУ 2", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6"};
tParam G10_P05 = {"04 РАЗРЕШЕНИЕ РАБОТЫ", 	0x0A04, 0, 0, 7, PAR_IS_LIST, 1, listG10P05[0], true, false, UNITS_VOID};

char listG10P06[2][MAX_NAME_LENGTH] =  {"REMOTE", "LOCAL"};
tParam G10_P06 = {"04 РАЗРЕШЕНИЕ РАБОТЫ", 	0x0A05, 0, 0, 1, PAR_IS_LIST, 1, listG10P06[0], true, false, UNITS_VOID};

char listG10P07[2][MAX_NAME_LENGTH] =  {"HAND/AUTO", "PID"};
tParam G10_P07 = {"04 РАЗРЕШЕНИЕ РАБОТЫ", 	0x0A06, 0, 0, 1, PAR_IS_LIST, 1, listG10P07[0], true, false, UNITS_VOID};
//--------------------------------------------------------------------


//--------------------------Группа 11---------------------------------
char listG11P01[2][MAX_NAME_LENGTH] =  {"REF1 Hz", "REF2 %"};
tParam G11_P01 = {"01 ЗАДАНИЕ С ПУЛЬТА", 	0x0B00, 0, 0, 1, PAR_IS_LIST, 1, listG11P01[0], true, false, UNITS_VOID};




//--------------------------------------------------------------------






//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------



