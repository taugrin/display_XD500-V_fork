#include <groups_old.h>
#include <menu_old.h>
#include "cmsis_os.h"


#define VERSION "01.00.09"

#define KEY_FUNC  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)==GPIO_PIN_RESET
#define KEY_UP  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)==GPIO_PIN_RESET
#define KEY_DOWN    HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)==GPIO_PIN_RESET
#define KEY_ENTER HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET
#define KEY_RESET HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)==GPIO_PIN_RESET
#define KEY_RUN   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14)==GPIO_PIN_RESET
#define KEY_STOP  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15)==GPIO_PIN_RESET

#define LED_READY GPIOB, GPIO_PIN_5
#define LED_RUN GPIOB, GPIO_PIN_14
#define LED_FAULT GPIOB, GPIO_PIN_2

#define EEPROM_ADR 0xA0
#define EEPROM_PAGE_SIZE 15
#define EEPROM_TIME_OUT 100
#define EEPROM_DELAY 8
#define EEPROM_NOTE_SIZE 2
#define EEPROM_NOTE_IN_PAGE 7
#define EEPROM_TRY_READ 3
#define EEPROM_CRC_SIZE 1
#define EEPROM_DATA_PAGE 1
#define EEPROM_SERVICE_PAGE 0
#define EEPROM_CONFIG_EXISTS 0xAA

//#define EEPROM_READ_ADR 0xA1


typedef union
{
    uint16_t data;

    uint8_t byte[2];

} ee_note_t;


typedef struct
{
    uint16_t code_first_write;
    uint16_t firmware_version;
    uint16_t error;
    uint16_t reserved1;
    uint16_t reserved2;
    uint16_t reserved3;
    uint16_t reserved4;
    uint8_t  reserved5;
    uint8_t  crc;


} ee_service_words_t;


typedef union
{

    ee_service_words_t words;
    uint8_t page[16];

} ee_service_t;


typedef enum
{
    upload,
    download,

} ee_cmd_t;

typedef enum
{
    first,
    last,
    transit,

} ee_bound_flag_t;


typedef enum
{
    no_error,
    cmd_not_found,
    crc_not_correct,
    memory_is_empty,
    vfd_is_started

} ee_error_t;


//#define MAX_DATA 4

#define FONT_SIZE 5
#define CHAR_GAP  1
#define FONT_GAP (FONT_SIZE+1*CHAR_GAP)

#define NUM_MAX_LEN 6

#define DISP_RIGHT_BOUND 127
#define DISP_LEFT_BOUND 2
#define DISP_CENTR 64

#define BIN 1
#define DEC 0


#define UNITS_GAP 2
#define NUMBER_EDIT_X 35
#define NUMBER_EDIT_STR 4

#define NAVIG_PARAMETR 0
#define NAVIG_DISP_STRUCT 1
#define ANSWER_DELAY 30

typedef struct
{
    int Value;
    int Pointer;
    int ValueMax;
    int ValueMin;


} level_t;


typedef struct
{
    level_t idx;
    level_t group[GROUPS_NUMBER];

} Param_t;

typedef struct
{
    level_t idx;
    level_t item[MONIT_ITEMS_NUMBER];

} Monitor_t;

typedef struct
{
    level_t idx;
    //level_t event[EVENTS_PARAM_NUMBER];

} Archive_t;

typedef struct
{
    level_t idx;
    //level_t event[EVENTS_PARAM_NUMBER];

} Memory_t;

typedef struct
{

     Param_t fastParam;
     Param_t Param;
     Monitor_t Monitor;
     Archive_t Archive;
     Memory_t Memory;
     level_t idx;

} main_t;

typedef struct
{


    main_t main;
    level_t idx;

} Menu_t;

typedef struct
{
    uint16_t sec;
    uint16_t min;
    uint16_t hour;
    uint16_t day;
} alarmTime_t;

typedef enum
{
    left,
    right,


} arch_pos_t;


typedef struct
{
 int16_t adr;
 int16_t fact;
 int16_t bindec;
 char name [6];
 uint16_t data;
 arch_pos_t pos;
 uint8_t str;

} arch_param_t;



#define ARCHIVE_DATA1 {0,1, DEC,"CODE", 9,    left,  2}
#define ARCHIVE_DATA2 {1,1, BIN,"SWORD",7,    right, 2}
#define ARCHIVE_DATA3 {2,10,DEC,"FREQ", 1001, left,  6}
#define ARCHIVE_DATA4 {3,10,DEC,"CURR", 5600, left,  4}
#define ARCHIVE_DATA5 {4,10,DEC,"UDC",  6600, right, 4}
#define ARCHIVE_DATA6 {5,10,DEC,"TIGBT",1887, right, 6}

#define ARCHIVE_DATA_ALL {ARCHIVE_DATA1,ARCHIVE_DATA2,ARCHIVE_DATA3,ARCHIVE_DATA4,ARCHIVE_DATA5,ARCHIVE_DATA6}
#define ARCHIVE_DATA_NUM 6

typedef union
{
    uint32_t i;
    float f;

} fl_data_t;

typedef enum
{
    var_RO_status = 0,	// 0131
    var_DI_status,		// 0132
    var_AI1_scale,		// 0133
    var_AI2_scale,		// 0134
    var_AIT_scale,		// 0135
    var_Udc,			// 0104
    var_rms_Vab,		// 0105
    var_rms_Iab,		// 0106
    var_FreqRef,		// 0102
    var_Freq,			// 0103
    var_Power,			// 0136
    var_Tigbt,			// 0107
    var_Encoder_Speed,	// 0137
    var_Stator_Flux_Magn,//0138
    var_TechPidCtrl_In,	// 0139
    var_TechPidCtrl_Fdb,// 013A
    var_TechPidCtrl_Error,//013B
    var_TechPidCtrl_Out,// 013C
    var_Control_word,	// 0001
    var_Status_word,	// 0101
    var_Fault_word,	// 0109
    var_Alarm_word,	// 013D
} MonitorSelect;




//uint8_t KeyChekSem(SemaphoreHandle_t semaphore);

int16_t IntDataRequest(uint16_t dataAdress);

void MenuInit(Menu_t * obj);

void DisplayStaticOld();

void RefInit();


