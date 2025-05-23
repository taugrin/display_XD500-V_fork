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
tParam Gxx_Pxx = {"xx НАЗВАНИЕ", 		0xEEEE, 0, 0, 9, false, 1, true, 0, true, false};

// Группа 10
tParam G10_P01 = {"01 ПУ1 СТАРТ/СТОП", 	0x0A00, 0, 0, 9, false, 1, true, 0, true, false};
tParam G10_P02 = {"02 ПУ2 СТАРТ/СТОП", 	0x0A01, 0, 0, 9, false, 1, true, 0, true, false};
tParam G10_P03 = {"03 НАПРАВЛЕНИЕ", 	0x0A02, 0, 0, 3, false, 1, true, 1, true, false};



// массив для хранения списковых значений параметров
const char listArray[2][MAX_LIST_CNT][MAX_NAME_LENGTH] =
{
/*0 для G10_P01,02 */ {"NOT SEL", "DI1", "DI1, DI2", "DI1P, DI2P", "DI1P, DI2P, DI3",
"DI1P, DI2P, DI3P", "DI6", "DI6, DI5", "KEYPAD", "FIELDBUS"},

/*1 для G10_P03*/ {"FORWARD", "REVERSE", "REQUEST"}

};


//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------



