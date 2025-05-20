#ifndef MENU_H_
#define MENU_H_

#define SW_ADR                  0x4200
#define FW_ADR                  0x4202
#define AW_ADR                  0x4204

#define CW_ADR                  0x4000
#define CW_PRIV_ADR             0x000A
#define FREQ_REF_ADR            0x4002

#define LAST_EVENT              0x0300


#define VERSION_VFD_ADR         0x4400
#define FREQ_EST	            0x4401
#define FREQ_REF                0x440E

#define PU1_SOURCE_ADR          0x0A00
#define PU2_SOURCE_ADR          0x0A01
#define PU1_PU2_SELECT_ADR      0x0A04
#define LOC_REM_ADR             0x0A05

#define SEL_PRC_RPM_ADR         0x0B00
#define SEL_EXT_ADR             0x0B09

#define KEY_REF_MODE_ADR        0x1100
#define BUTTON_STOP_MODE_ADR    0x1104

#define MIN_SPEED_ADR           0x2000
#define MAX_SPEED_ADR           0x2001

#define MODBUSADR_ADR           0x3200

#define RESTOR_DEF              0x0007


#define TIME_PWR_ADR            0x0128
#define TIME_ALL_ADR            0x012B
#define TIME_MOT_ADR            0x012E

#define START_STOP       0
#define DIRECT           1
#define ALARM_RES        2

#define MODE_MONITOR     1
#define MODE_CONTROL     2
#define MODE_PREFERENS   3


#define USB_NO_CONNECT_MSG "NO CONNECT TO KEYPAD"

typedef union
{
    struct
    {
        unsigned ready          :1;
        unsigned start_stop     :1;
        unsigned direction      :1;
        unsigned fault          :1;
        unsigned alarm          :1;
        unsigned reserv1        :1;
        unsigned reserv2        :1;
        unsigned reserv3        :1;

    } bits;

    uint8_t word;

} statusWord_t;

typedef union
{
    struct
    {
        unsigned start_stop     :1;
        unsigned direction      :1;
        unsigned fault_res      :1;
        unsigned alarm          :1;
        unsigned atref          :1;
        unsigned rstToFactory   :1;
        unsigned reserv4        :1;
        unsigned reserv5        :1;

    } bits;

    uint8_t word;

} controlWord_t;


typedef union
{
    struct
    {
        unsigned stop_flag      :1;
        unsigned run_enable     :1;
        unsigned reserv0        :1;
        unsigned reserv1        :1;
        unsigned reserv2        :1;
        unsigned reserv3        :1;
        unsigned reserv4        :1;
        unsigned reserv5        :1;

    } bits;

    uint8_t word;

} controlWord_priv_t;


#define MENU_ITEMS_NUMBER 7

#define MENU_ITEM_1 "œ¿–¿Ã≈“–€"
#define MENU_ITEM_2 "¿–’»¬ —Œ¡€“»…"
#define MENU_ITEM_3 "¡€—“–€… —“¿–“"
#define MENU_ITEM_4 "‘”Õ ÷»»  Œœ»–Œ¬¿Õ»ﬂ"
#define MENU_ITEM_5 "¬≈–—»ﬂ œ–Œ√–¿ÃÃ€"
#define MENU_ITEM_6 "ÃŒÕ»“Œ–»Õ√"
#define MENU_ITEM_7 "œŒ ¿«¿Õ»ﬂ “¿…Ã≈–Œ¬"

#define MEMORY_ITEMS_NUMBER 3

#define MEMORY_ITEM_1 "«¿√–”«»“‹ ¬ XD500"
#define MEMORY_ITEM_2 " Œœ»–Œ¬¿“‹ ¬ œ”À‹“"
#define MEMORY_ITEM_3 "«¿¬Œƒ— »≈ Õ¿—“–Œ… »"

#define MEMORY_ITEMS \
{\
    MEMORY_ITEM_1,\
    MEMORY_ITEM_2,\
    MEMORY_ITEM_3,\
}


#define MENU_ITEMS \
{\
MENU_ITEM_1,\
MENU_ITEM_2,\
MENU_ITEM_3,\
MENU_ITEM_4,\
MENU_ITEM_5,\
MENU_ITEM_6,\
MENU_ITEM_7,\
}



#define MONITOR_VIEW_NUMBER 3


#define MONITOR_VIEW1_TEXT    "ÃŒÕ»“Œ–»Õ√ 1"
#define MONITOR_VIEW2_TEXT    "ÃŒÕ»“Œ–»Õ√ 2"
#define MONITOR_VIEW3_TEXT    "ÃŒÕ»“Œ–»Õ√ 3"


#define ALL_MONITOR_VIEW_TEXT \
{\
MONITOR_VIEW1_TEXT,\
MONITOR_VIEW2_TEXT,\
MONITOR_VIEW3_TEXT,\
}


#define MONIT_ITEMS_NUMBER 5

#define MONIT_1_TXT "◊¿—“Œ“¿, √ˆ"
#define MONIT_2_TXT "œŒ—“ Õ¿œ–ﬂ∆, ¬"
#define MONIT_3_TXT "¬€’ Õ¿œ–ﬂ∆, ¬"
#define MONIT_4_TXT "¬€’ “Œ , ¿"
//#define MONIT_5_TXT "ƒ»— – ¬’Œƒ€"
#define MONIT_6_TXT "“≈ÃœE– IGBT,ß—"

#define MONIT_1_ADR 0x0103
#define MONIT_2_ADR 0x0104
#define MONIT_3_ADR 0x0105
#define MONIT_4_ADR 0x0106
//#define MONIT_5_ADR 0x010B
#define MONIT_6_ADR 0x0107

#define MONIT_1_MLP 1
#define MONIT_2_MLP 1
#define MONIT_3_MLP 1
#define MONIT_4_MLP 10
//#define MONIT_5_ADR 0x010B
#define MONIT_6_MLP 1

#define MONITOR_ITEMS \
{\
MONIT_1_TXT,\
MONIT_2_TXT,\
MONIT_3_TXT,\
MONIT_4_TXT,\
MONIT_6_TXT,\
}

#define MONITOR_ADR \
{\
MONIT_1_ADR,\
MONIT_2_ADR,\
MONIT_3_ADR,\
MONIT_4_ADR,\
MONIT_6_ADR,\
}

#define MONITOR_MLP \
{\
MONIT_1_MLP,\
MONIT_2_MLP,\
MONIT_3_MLP,\
MONIT_4_MLP,\
MONIT_6_MLP,\
}






#endif
