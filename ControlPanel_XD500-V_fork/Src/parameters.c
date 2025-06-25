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
	{"03 ÏÓ1 ÌÈÍÈÌÓÌ", 	     0x0B02, 0, 500,  PAR_IS_UINT,  10, NULL,       true, false, UNITS_HZ},
	{"04 ÏÓ1 ÌÀÊÑÈÌÓÌ", 	 0x0B03, 0, 1000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_HZ},
	{"05 ÏÓ2 ÇÀÄÀÍÈÅ", 	     0x0B04, 0, 4,    PAR_IS_LIST, 1,  listG11P02, true, false, UNITS_VOID},
	{"06 ÏÓ2 ÌÈÍÈÌÓÌ", 	     0x0B05, 0, 1000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_PROC},
	{"07 ÏÓ2 ÌÀÊÑÈÌÓÌ", 	 0x0B06, 0, 2000, PAR_IS_UINT,  10, NULL,       true, false, UNITS_PROC}
};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group11 = {
    "11 ÂÛÁÎĞ ÇÀÄÀÍÈß",
    group11_params,
    sizeof(group11_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 12---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG12P01[] =  "NOT SEL\0DI1\0DI2\0DI3\0DI_12\0DI_34\0DI_56\0DI_123\0DI_456";

// Ïàğàìåòğû ãğóïïû 12
static const tParam group12_params[] = {
	{"01 ÂÛÁÎĞ ÏÎÑÒ ÑÊÎĞ",  0x0C00, 			    0,    8,  PAR_IS_LIST, 1,  listG12P01, true, false, UNITS_VOID},
	{"02 ÏÎÑÒ ÑÊÎĞ 1", 	    0x0C01, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"03 ÏÎÑÒ ÑÊÎĞ 2", 	    0x0C02, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"04 ÏÎÑÒ ÑÊÎĞ 3", 	    0x0C03, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"05 ÏÎÑÒ ÑÊÎĞ 4", 	    0x0C04, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"06 ÏÎÑÒ ÑÊÎĞ 5", 	    0x0C05, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"07 ÏÎÑÒ ÑÊÎĞ 6", 	    0x0C06, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"08 ÏÎÑÒ ÑÊÎĞ 7", 	    0x0C07, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM},
	{"09 ÏÎÑÒ ÑÊÎĞ 8", 	    0x0C08, (uint16_t)(-6000), 6000,  PAR_IS_INT,  1,  NULL,       true, false, UNITS_RPM}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group12 = {
    "12 ÏÎÑÒ ÑÊÎĞÎÑÒÈ",
    group12_params,
    sizeof(group12_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 13---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG13P01[] =  "ÍÀÏĞßÆÅÍÈÅ\0ÒÎÊ";
static char listG13P02[] =  "0Â/0ìÀ x2Â/4ìÀ \0TUNED VALUE\0TUNE";
static const char listG13P03[] =  "10Â/20ìÀ \0TUNED VALUE\0TUNE";
static char listG13P05[] =  "0ìÀ x4ìÀ \0TUNED VALUE\0TUNE";
static const char listG13P06[] =  "20ìÀ \0TUNED VALUE\0TUNE";
static const char listG13P08[] =  "ÒÎÊ\0ÒÅÌÏÅĞÀÒÓĞÀ";
static char listG13P09[] =  "0ìÀ x4ìÀ \0TUNED VALUE\0TUNE";
static const char listG13P10[] =  "20ìÀ \0TUNED VALUE\0TUNE";

// Ïàğàìåòğû ãğóïïû 13
static const tParam group13_params[] = {
	{"01 ÒÈÏ ÂÕÎÄÀ AI1",  	0x0D00, 			    0,     1,  PAR_IS_LIST, 1,     listG13P01, true, false, UNITS_VOID},
	{"02 ÌÈÍÈÌÓÌ AI1",  	0x0D01, 			    0,     3,  PAR_IS_LIST, 1,     listG13P02, true, false, UNITS_VOID},
	{"03 ÌÀÊÑÈÌÓÌ AI1",  	0x0D02, 			    0,     2,  PAR_IS_LIST, 1,     listG13P03, true, false, UNITS_VOID},
	{"04 T ÔÈËÜÒĞÀ AI1", 	0x0D03, 				1, 65535,  PAR_IS_UINT, 1000,  NULL,       true, false, UNITS_SEC},
	{"05 ÌÈÍÈÌÓÌ AI2",  	0x0D04, 			    0,     3,  PAR_IS_LIST, 1,     listG13P05, true, false, UNITS_VOID},
	{"06 ÌÀÊÑÈÌÓÌ AI2",  	0x0D05, 			    0,     2,  PAR_IS_LIST, 1,     listG13P06, true, false, UNITS_VOID},
	{"07 T ÔÈËÜÒĞÀ AI2", 	0x0D06, 				1, 65535,  PAR_IS_UINT, 1000,  NULL,       true, false, UNITS_SEC},
	{"08 ÒÈÏ ÂÕÎÄÀ AI3",  	0x0D07, 			    0,     1,  PAR_IS_LIST, 1,     listG13P08, true, false, UNITS_VOID},
	{"09 ÌÈÍÈÌÓÌ AI3",  	0x0D08, 			    0,     3,  PAR_IS_LIST, 1,     listG13P09, true, false, UNITS_VOID},
	{"10 ÌÀÊÑÈÌÓÌ AI3",  	0x0D09, 			    0,     2,  PAR_IS_LIST, 1,     listG13P10, true, false, UNITS_VOID},
	{"11 T ÔÈËÜÒĞÀ AI3", 	0x0D0A, 				1, 65535,  PAR_IS_UINT, 1000,  NULL,       true, false, UNITS_SEC}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group13 = {
    "13 ÀÍÀËÎÃÎÂÛÅ ÂÕÎÄÛ",
    group13_params,
    sizeof(group13_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 14---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG14P01[] =  "NOT SEL\0READY\0RUN\0FAULT\0FAULT(-1)\0ALARM";

// Ïàğàìåòğû ãğóïïû 14
static const tParam group14_params[] = {
	{"01 ÒÈÏ ÂûÕÎÄÀ RO1",  	0x0E00, 			    0,     5,  PAR_IS_LIST, 1,     listG14P01, true, false, UNITS_VOID},
	{"02 ÒÈÏ ÂûÕÎÄÀ RO2",  	0x0E01, 			    0,     5,  PAR_IS_LIST, 1,     listG14P01, true, false, UNITS_VOID},
	{"03 ÒÈÏ ÂûÕÎÄÀ RO3",  	0x0E02, 			    0,     5,  PAR_IS_LIST, 1,     listG14P01, true, false, UNITS_VOID}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group14 = {
	"14 ĞÅËÅÉÍÛÅ ÂûÕÎÄÛ",
    group14_params,
    sizeof(group14_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 15---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG15P01[] =  "NOT SEL\0SPEED\0FREQUENCY\0CURRENT\0VOLTAGE\0DC BUS VOLTAGE";
static char listG15P02[] =  "0-20ìÀ x4-20ìÀ";

// Ïàğàìåòğû ãğóïïû 15
static const tParam group15_params[] = {
	{"01 ÒÈÏ ÂûÕÎÄÀ AO1",  	0x0F00, 			    0,     5,  PAR_IS_LIST, 1,     listG15P01, true, false, UNITS_VOID},
	{"02 ÄÈÀÏÀÇÎÍ AO1",  	0x0F01, 			    0,     1,  PAR_IS_LIST, 1,     listG15P02, true, false, UNITS_VOID},
	{"03 T ÔÈËÜÒĞÀ AO1", 	0x0F02, 				1, 10000,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_MSEC},
	{"04 ÒÈÏ ÂûÕÎÄÀ AO2",  	0x0F03, 			    0,     5,  PAR_IS_LIST, 1,     listG15P01, true, false, UNITS_VOID},
	{"05 ÄÈÀÏÀÇÎÍ AO2",  	0x0F04, 			    0,     1,  PAR_IS_LIST, 1,     listG15P02, true, false, UNITS_VOID},
	{"06 T ÔÈËÜÒĞÀ AO2", 	0x0F05, 				1, 10000,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_MSEC},
	{"07 ÑÊÎĞÎÑÒÜ 20ìÀ", 	0x0F06, 				0, 65535,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_RPM},
	{"08 ×ÀÑÒÎÒÀ 20ìÀ", 	0x0F07, 				0, 65535,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_HZ},
	{"09 ÒÎÊ 20ìÀ", 		0x0F08, 				0, 65535,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_AMP},
	{"10 ÍÀÏĞßÆÅÍÈÅ 20ìÀ", 	0x0F09, 				0, 65535,  PAR_IS_UINT, 1,     NULL,       true, false, UNITS_VOLT}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group15 = {
	"15 ÀÍÀËÎÃÎÂÛÅ ÂûÕÎÄÛ",
    group15_params,
    sizeof(group15_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 17---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG17P01[] =  "RESET TO ZERO\0SAVE TO MEMORY";
static const char listG17P02[] =  "LOCAL\0LOCAL/REMOTE";
static const char listG17P03[] =  "STOP\0OPERATION\0OPERATION AND REF";
static const char listG17P04[] =  "NOT SEL\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";

// Ïàğàìåòğû ãğóïïû 17
static const tParam group17_params[] = {
	{"01 ÇÀÄÀÍÈÅ ÏÓËÜÒ",  		0x1100,		0,     1,  PAR_IS_LIST, 1,     listG17P01, true, false, UNITS_VOID},
	{"02 ĞÅÆÈÌ ÊÍÎÏÊÈ ÑÒÎÏ",	0x1101, 	0,     1,  PAR_IS_LIST, 1,     listG17P02, true, false, UNITS_VOID},
	{"03 ÏÅĞÅÕÎÄ ÄÈÑÒ/ÌÅÑÒ",	0x1101, 	0,     2,  PAR_IS_LIST, 1,     listG17P03, true, false, UNITS_VOID},
	{"04 ÏÅĞÅÕÎÄ ÄÈÑÒ/ÌÅÑÒ",	0x1101, 	0,     6,  PAR_IS_LIST, 1,     listG17P04, true, false, UNITS_VOID}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group17 = {
	"17 ÍÀÑÒĞÎÉÊÈ ÓÏĞÀÂË",
    group17_params,
    sizeof(group17_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 20---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG20P07[] =  "OFF\0ON";

// Ïàğàìåòğû ãğóïïû 20
static const tParam group20_params[] = {
	{"01 ÌÈÍ ÑÊÎĞÎÑÒÜ",		0x1400, (uint16_t)(-1000), 1000,  PAR_IS_INT,  10,  	  NULL,	true, false, UNITS_HZ},
	{"02 ÌÀÊÑ ÑÊÎĞÎÑÒÜ",	0x1401, (uint16_t)(-1000), 1000,  PAR_IS_INT,  10,  	  NULL,	true, false, UNITS_HZ},
	{"03 ÌÀÊÑ ÒÎÊ",			0x1402, 				0,  200,  PAR_IS_UINT,  1,  	  NULL,	true, false, UNITS_PROC},
	{"04 ÌÀÊÑ ÌÎÌÅÍÒ",		0x1403, 			   50,  400,  PAR_IS_UINT,  1,  	  NULL,	true, false, UNITS_PROC},
	{"05 ÌÈÍ ÍÀÏĞßÆÅÍÈÅ",	0x1404, 			   70,  100,  PAR_IS_UINT,  1,  	  NULL,	true, false, UNITS_PROC},
	{"06 ÌÀÊÑ ÍÀÏĞßÆÅÍÈÅ",	0x1405, 			  100,  150,  PAR_IS_UINT,  1,  	  NULL,	true, false, UNITS_PROC},
	{"07 ĞÅÃÓËßÒÎĞ Vmin",  	0x1406,					0,    1,  PAR_IS_LIST,  1,  listG20P07, true, false, UNITS_VOID},
	{"08 ĞÅÃÓËßÒÎĞ Vmax",  	0x1407,					0,    1,  PAR_IS_LIST,  1,  listG20P07, true, false, UNITS_VOID},

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group20 = {
	"20 ÏĞÅÄÅËÛ",
    group20_params,
    sizeof(group20_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 22---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG22P01[] =  "ACC/DEC_1\0ACC/DEC_2\0DI1\0DI2\0DI3\0DI4\0DI5";
static const char listG22P06[] =  "LINEAR\0S-CURVE";
static const char listG22P07[] =  "S01-CURVE\0S02-CURVE\0S03-CURVE";

// Ïàğàìåòğû ãğóïïû 22
static const tParam group22_params[] = {
	{"01 ÂÛÁ ÏÀĞÛ ÓÑÊ/ÇÀÌ",  0x1600,		0,     6,  PAR_IS_LIST,   1, listG22P01, true, false, UNITS_VOID},
	{"02 ÂĞÅÌß ÓÑÊÎĞÅÍÈß 1", 0x1601,		1, 60000,  PAR_IS_UINT,  10,   	   NULL, true, false, UNITS_SEC},
	{"03 ÂĞÅÌß ÇÀÌÅÄËÅÍ 1",  0x1602,	    1, 60000,  PAR_IS_UINT,  10,   	   NULL, true, false, UNITS_SEC},
	{"04 ÂĞÅÌß ÓÑÊÎĞÅÍÈß 2", 0x1603,		1, 60000,  PAR_IS_UINT,  10,   	   NULL, true, false, UNITS_SEC},
	{"05 ÂĞÅÌß ÇÀÌÅÄËÅÍ 2",  0x1604,	    1, 60000,  PAR_IS_UINT,  10,   	   NULL, true, false, UNITS_SEC},
	{"06 ÊĞÈÂÀß ĞÀÇÃÎÍÀ",  	 0x1605,		0,     1,  PAR_IS_LIST,   1, listG22P06, true, false, UNITS_VOID},
	{"07 ÔÎĞÌÀ S-ÊĞÈÂÎÉ",  	 0x1606,		0,     2,  PAR_IS_LIST,   1, listG22P07, true, false, UNITS_VOID},
	{"08 ÂĞÅÌß ÇÀÌ ÀÂÀĞ",  	 0x1607,		0,  1000,  PAR_IS_UINT, 100, 	   NULL, true, false, UNITS_SEC},

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group22 = {
	"22 ÓÑÊÎĞ/ÒÎĞÌÎÆÅÍÈÅ",
    group22_params,
    sizeof(group22_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 31---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG31P04[] =  "NO\0YES";

// Ïàğàìåòğû ãğóïïû 31
static const tParam group31_params[] = {
	{"01 ÊÎËÈ× ÏÎÏÛÒÎÊ", 	0x1F00,		0,    10,  PAR_IS_UINT,  1,		NULL, true, false, UNITS_VOID},
	{"02 ÂĞÅÌß ÀÏÂ", 		0x1F01,		0,  6000,  PAR_IS_UINT, 10,   	NULL, true, false, UNITS_SEC},
	{"03 ÇÀÄÅĞÆÊÀ ÀÏÂ", 	0x1F02,		0,  6000,  PAR_IS_UINT, 10,   	NULL, true, false, UNITS_SEC},
	{"04 ÏÅĞÅÃĞÓÇÊÀ", 		0x1F03,		0,     1,  PAR_IS_LIST,   1, listG31P04, true, false, UNITS_VOID},
	{"05 ÌÈÍ ÍÀÏĞßÆÅÍÈÅ", 	0x1F04,		0,     1,  PAR_IS_LIST,   1, listG31P04, true, false, UNITS_VOID},
	{"06 ÌÀÊÑ ÍÀÏĞßÆÅÍ", 	0x1F05,		0,     1,  PAR_IS_LIST,   1, listG31P04, true, false, UNITS_VOID},

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group31 = {
	"31 ÀÂÒÎ ÏÅĞÅÇÀÏÓÑÊ",
    group31_params,
    sizeof(group31_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 40---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)
static const char listG40P05[] =  "NO\0YES";
static const char listG40P06[] =  "ACT1\0ACT1-ACT2\0ACT1+ACT2\0ACT1*ACT2\0ACT1/ACT2\0MIN(ACT1, ACT2)\0MAX(ACT1, ACT2)";
static const char listG40P07[] =  "AI1\0AI2\0AI3\0POWER";
static const char listG40P14[] =  "OFF\0INTERNAL\0DI1\0DI2\0DI3\0DI4\0DI5\0DI6";

// Ïàğàìåòğû ãğóïïû 40
static const tParam group40_params[] = {
    {"01 ÏÈÄ Kp", 		 	0x2800,					1,  1000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_VOID},
    {"02 ÏÈÄ Ki", 	 	 	0x2801,					1,  1000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_VOID},
    {"03 ÏÈÄ Kd", 	 	 	0x2802,					0,  1000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_VOID},
	{"04 ÔÈËÜÒĞ ÄÈÔÔ", 	 	0x2803, 				4, 10000,  PAR_IS_UINT, 100,     	NULL, true, false, UNITS_SEC},
	{"05 ÈÍÂÅĞÒ ÎØÈÁ", 	 	0x2804,					0,     1,  PAR_IS_LIST,   1,  listG40P05, true, false, UNITS_VOID},
	{"06 ÂÛÁÎĞ ÑÈÃÍ ÎÑ", 	0x2805,					0,     6,  PAR_IS_LIST,   1,  listG40P06, true, false, UNITS_VOID},
	{"07 ÂÕÎÄ ÑÈÃÍ ÎÑ1",	0x2806,					0,     3,  PAR_IS_LIST,   1,  listG40P07, true, false, UNITS_VOID},
	{"08 ÂÕÎÄ ÑÈÃÍ ÎÑ2", 	0x2807,					0,     3,  PAR_IS_LIST,   1,  listG40P07, true, false, UNITS_VOID},
	{"09 ÑÈÃÍ ÎÑ1 ÌÈÍ",		0x2808, (uint16_t)(-1000),  1000,   PAR_IS_INT,  10,  	  	NULL, true, false, UNITS_PROC},
	{"10 ÑÈÃÍ ÎÑ1 ÌÀÊÑ",	0x2809, (uint16_t)(-1000),  1000,   PAR_IS_INT,  10,  	  	NULL, true, false, UNITS_PROC},
	{"11 ÑÈÃÍ ÎÑ2 ÌÈÍ",		0x280A, (uint16_t)(-1000),  1000,   PAR_IS_INT,  10,  	  	NULL, true, false, UNITS_PROC},
	{"12 ÑÈÃÍ ÎÑ2 ÌÀÊÑ",	0x280B, (uint16_t)(-1000),  1000,   PAR_IS_INT,  10,  	  	NULL, true, false, UNITS_PROC},
	{"13 ÔÈËÜÒĞ ÑÈÃÍ ÎÑ", 	0x280C, 				0,  1000,  PAR_IS_UINT, 100,     	NULL, true, false, UNITS_SEC},
	{"14 ÂÛÁÎĞ ÔÓÍÊ. ÑÎÍ", 	0x280D,					0,     7,  PAR_IS_LIST,   1,  listG40P14, true, false, UNITS_VOID},
	{"15 ÓĞÎÂÅÍÜ ÀÊÒÈÂ", 	0x280E,					0, 60000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_RPM},
	{"16 ÇÀÄÅĞÆÊÀ ÀÊÒÈÂ", 	0x280F,					0, 36000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_SEC},
	{"17 ÓĞÎÂÅÍÜ ÎÒÊË", 	0x2810,					0,  1000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_PROC},
	{"18 ÇÀÄÅĞÆÊÀ ÎÒÊË", 	0x2811,					0, 36000,  PAR_IS_UINT,  10,		NULL, true, false, UNITS_SEC},

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group40 = {
	"40 ÏÈÄ-ĞÅÃÓËßÒÎĞ",
    group40_params,
    sizeof(group40_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------

//--------------------------Ãğóïïà 64---------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)


// Ïàğàìåòğû ãğóïïû 64
static const tParam group64_params[] = {
	{"01 CONTROL WORD",  	0x4000, 			     0, 65535,  PAR_IS_UINT,   1, NULL, true, true, UNITS_VOID},
	{"02 CONTROL WORD1",  	0x4001,   			     0, 65535,  PAR_IS_UINT,   1, NULL, true, true, UNITS_VOID},
	{"03 REFERENCE",  		0x4002, (uint16_t)(-32767), 32767,  PAR_IS_INT,  100, NULL, true, true, UNITS_HZ}

};

// Ãğóïïà ïàğàìåòğîâ
const tGroup Group64 = {
    "64 ÇÀÄÀÍÈÅ ÏÓËÜÒ",
    group64_params,
    sizeof(group64_params)/sizeof(tParam),
	true
};
//--------------------------------------------------------------------


//--------------------------Ìîíèòîğèíã--------------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)


// Ïàğàìåòğû Ìîíèòîğèíãà
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

uint8_t MonitorSelect[3] = {2, 0, 5}; // çíà÷åíèÿ Ìîíèòîğ 1, Ìîíèòîğ 2, Ìîíèòîğ 3
uint8_t MonitorNum = 0; // èíäåêñ âûáğàííîãî ìîíèòîğèíãà (Ìîíèòîğ 1, Ìîíèòîğ 2, Ìîíèòîğ 3)
uint8_t MonitorVal = 0;

//--------------------------------------------------------------------


//--------------------------Àğõèâ ñîáûòèé-----------------------------
// Ñïèñêè ïàğàìåòğîâ (îäíîé ñòğîêîé ñ \0 ğàçäåëèòåëÿìè)


// Ïàğàìåòğû Àğõèâà ñîáûòèé
/*const tParam Monitor[] = {
	{"DC BUS VOLT",    	  0x4407,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"VOLTAGE",  	  	  0x4406,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_VOLT},
	{"CURRENT", 	      0x4403,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_AMP},
	{"FREQUENCY", 	      0x4401,  (uint16_t)(-1000),  1000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_HZ},
	{"POWER", 		  	  0x4405, 		 	       0, 10000,  PAR_IS_INT, 100, NULL, false, false,  UNITS_KWT},
	{"IGBT TEMP",    	  0x4408,                  0, 10000,  PAR_IS_INT,  10, NULL, false, false,  UNITS_CELSIUM}

};

const uint16_t MonitorCnt = sizeof(Monitor)/sizeof(tParam);*/

uint8_t EventNum = 0; // èíäåêñ ïğîñìàòğèâàåìîãî ñîáûòèÿ

//--------------------------------------------------------------------



const tGroup* const AllGroups[] = {&Group10, &Group11, &Group12, &Group13, &Group14, &Group15, &Group17, &Group20, &Group22,
		&Group31, &Group40, &Group64};
const uint16_t AllGroupsCnt = sizeof(AllGroups)/sizeof(tGroup*);

const tGroup* const MenuGroups[] = {&Group10, &Group11, &Group12, &Group13, &Group14, &Group15, &Group17, &Group20, &Group22,
		&Group31, &Group40};
const uint16_t MenuGroupsCnt = sizeof(MenuGroups)/sizeof(tGroup*);

const tParam* const FastSettings[] = {
	&group10_params[0], &group10_params[2],
	&group11_params[0], &group11_params[1],
	&group12_params[1], &group12_params[3]
};
const uint16_t FastSettingsCnt = sizeof(FastSettings)/sizeof(tParam*);
uint8_t FastSettingsNum = 0; // èíäåêñ âûáğàííîãî ïàğàìåòğà áûñòğûõ íàñòğîåê

//--------------------------------------------------------------------
/*
* GetParamListItem - ôóíêöèÿ ïîëó÷åíèÿ ıëåìåíòà ñïèñêà ïî èíäåêñó.
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
// Ïğèìåğ èñïîëüçîâàíèÿ:
const tParam* param = &Group10.params[0]; // Ïàğàìåòğ ñî ñïèñêîì
const char* listItem = GetParamListItem(param, 2); // Ïîëó÷àåì 3-é ıëåìåíò (èíäåêñ 2)

*/

//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* InitParams - èíèöèàëèçàöèÿ ïàğàìåòğîâ
*/
void InitParams(void)
{
	/* Â íåêòîğûõ ñïèñêàõ âñòğå÷àşòñÿ äğóã çà äğóãîì íóëü-òåğìèíàòîğ,à ïîòîì öèôğà.
	 * İòà ñèòóàöèÿ âîñïğèíèìàåòñÿ íåïğàâèëüíî, ïîòîìó ÷òî âìåñòî \0 çàïèñûâàåòñÿ \02 (èëè äğóãàÿ öèôğà).
	 * Ïîıòîìó äëÿ îáõîäà ıòîé ñèòóàöèè ïğèõîäèòñÿ âìåñòî \0 çàïèñûâàòü "ëåâûé" ñèìâîë, íàïğèìåğ x.
	 * À ïîòîì ìåíÿòü x íà \0.
	 */
	listG13P02[7] = '\0'; listG13P05[4] = '\0'; listG13P09[4] = '\0';

	listG15P02[7] = '\0';

}

//--------------------------------------------------------------------


