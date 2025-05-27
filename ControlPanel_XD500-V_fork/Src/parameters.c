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


//--------------------------Ãğóïïà 10---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG10P01[] = "NOT SEL\0DI1\0DI1, DI2\0DI1P, DI2P\0DI1P, DI2P, DI3\0DI1P, DI2P, DI3P\0DI6\0DI6, DI5\0KEYPAD\0FIELDBUS\0";
static const char listG10P03[] = "FORWARD\0REVERSE\0REQUEST";
static const char listG10P04[] =  "NO\0YES\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";
static const char listG10P05[] =  "ÏÓ 1\0ÏÓ 2\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";
static const char listG10P06[] =  "REMOTE\0LOCAL";
static const char listG10P07[] =  "HAND/AUTO\0PID";

// Ïàğàìåòğû ãğóïïû 10
static const tParam group10_params[] = {
	{"01 ÏÓ1 ÑÒÀĞÒ/ÑÒÎÏ",	 0x0A00, 0, 9, PAR_IS_LIST, 1, listG10P01, true, false, UNITS_VOID},
	{"02 ÏÓ2 ÑÒÀĞÒ/ÑÒÎÏ", 	 0x0A01, 0, 9, PAR_IS_LIST, 1, listG10P01, true, false, UNITS_VOID},
	{"03 ÍÀÏĞÀÂËÅÍÈÅ", 		 0x0A02, 0, 2, PAR_IS_LIST, 1, listG10P03, true, false, UNITS_VOID},
	{"04 ĞÀÇĞÅØÅÍÈÅ ĞÀÁÎÒÛ", 0x0A03, 0, 7, PAR_IS_LIST, 1, listG10P04, true, false, UNITS_VOID},
	{"05 ÂÛÁÎĞ ÏÓ1/ÏÓ2", 	 0x0A04, 0, 7, PAR_IS_LIST, 1, listG10P05, true, false, UNITS_VOID},
	{"06 ĞÅÆÈÌ ÓÏĞÀÂËÅÍÈß",  0x0A05, 0, 1, PAR_IS_LIST, 1, listG10P06, true, false, UNITS_VOID},
	{"07 ĞÅÆÈÌ ĞÀÁÎÒÛ", 	 0x0A06, 0, 1, PAR_IS_LIST, 1, listG10P07, true, false, UNITS_VOID}
};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group10 = {
    "10 ÏÓÑÊ/ÑÒÎÏ/ÍÀÏĞÀÂË",
    group10_params,
    sizeof(group10_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------


//--------------------------Ãğóïïà 11---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG11P01[] =  "REF1 Hz\0REF2 %";
static const char listG11P02[] =  "AI1\0AI2\0AI3\0KEYPAD\0FIELDBUS";

// Ïàğàìåòğû ãğóïïû 11
static const tParam group11_params[] = {
	{"01 ÇÀÄÀÍÈÅ Ñ ÏÓËÜÒÀ",  0x0B00, 0, 1,    PAR_IS_LIST, 1,  listG11P01, true, false, UNITS_VOID},
	{"02 ÏÓ1 ÇÀÄÀÍÈÅ", 	     0x0B01, 0, 4,    PAR_IS_LIST, 1,  listG11P02, true, false, UNITS_VOID},
	{"03 ÏÓ1 ÌÈÍÈÌÓÌ", 	     0x0B02, 0, 500,  PAR_IS_INT,  10, NULL,       true, false, UNITS_HZ},
	{"04 ÏÓ1 ÌÀÊÑÈÌÓÌ", 	 0x0B03, 0, 1000, PAR_IS_INT,  10, NULL,       true, false, UNITS_HZ},
	{"05 ÏÓ2 ÇÀÄÀÍÈÅ", 	     0x0B04, 0, 4,    PAR_IS_LIST, 1,  listG11P02, true, false, UNITS_VOID},
	{"06 ÏÓ2 ÌÈÍÈÌÓÌ", 	     0x0B05, 0, 1000, PAR_IS_INT,  10, NULL,       true, false, UNITS_PROC},
	{"07 ÏÓ2 ÌÀÊÑÈÌÓÌ", 	 0x0B06, 0, 2000, PAR_IS_INT,  10, NULL,       true, false, UNITS_PROC}
};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group11 = {
    "11 ÂÛÁÎĞ ÇÀÄÀÍÈß",
    group11_params,
    sizeof(group11_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------


const tGroup* const AllGroups[] = {&Group10, &Group11};
const uint16_t AllGroupsCnt = sizeof(AllGroups)/sizeof(tGroup*);

const tGroup* const MenuGroups[] = {&Group10, &Group11};
const uint16_t MenuGroupsCnt = sizeof(MenuGroups)/sizeof(tGroup*);


//--------------------------------------------------------------------
/*
* GetListItem - ôóíêöèÿ ïîëó÷åíèÿ ıëåìåíòà ñïèñêà ïî èíäåêñó.
*/
const char* GetListItem(const tParam* param, uint8_t index)
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
// Ïğèìåğ èñïîëüçîâàíèÿ:
const tParam* param = &Group10.params[0]; // Ïàğàìåòğ ñî ñïèñêîì
const char* listItem = GetListItem(param, 2); // Ïîëó÷àåì 3-é ıëåìåíò (èíäåêñ 2)

*/

//--------------------------------------------------------------------



