#include "display.h"
#include "usertasks.h"
#include "ST7565.h"
#include "stm32f1xx_hal.h"
#include "ftoa.h"
#include "usbh_def.h"
#include "usb_user.h"
#include "usb_host.h"
#include "groups.h"
#include "menu.h"
//#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern uint8_t rx_buffer[RXBUFLENGTH];
extern uint8_t tx_buffer[TXBUFLENGTH];

extern uint16_t rxMessageCount;
extern uint16_t txMessageCount;

extern uint16_t USBTransmitted;
extern uint16_t USBReceived;

extern I2C_HandleTypeDef hi2c1;

extern ApplicationTypeDef Appli_state;

#define JUSTI_LEFT 0
#define JUSTI_RIGHT 1

#define SCROLL_VALUE 0
#define SCROLL_LIST 1

const char Menu[MENU_ITEMS_NUMBER][MAX_STRING] = MENU_ITEMS;
const char Memory[MEMORY_ITEMS_NUMBER][MAX_STRING] = MEMORY_ITEMS;

//parameters
const group_t groups[GROUPS_NUMBER] = ALL_GROUPS;
//const group_t saved_groups[GROUPS_NUMBER] = SAVED_GROUPS;

const char listBase[LIST_BASE_NUM][MAX_LIST_NUMB][MAX_STRING] = LIST_BASE;

group_t fastParams = GROUP_FAST;
group_t ArchiveGroup = ARCHIVE_GROUP;
group_t monitorParams = MONITOR_GROUP;


 param_t keyRef_hz = G64P03;
 param_t keyRef_prc = G64P03_PRC;

 // АРХИВ
 arch_param_t arch_data[ARCHIVE_DATA_NUM] = ARCHIVE_DATA_ALL;

 // EEPROM
 static uint8_t EE_BufPage[EEPROM_PAGE_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

 // Display
 Menu_t Display;

 level_t* Mode;
 level_t* Main_menu;
 level_t* Groups_list;
 level_t* Fast_list;
 level_t* Monitor_list;
 level_t* Memory_list;
 level_t* Archive_list;
 level_t* Group[GROUPS_NUMBER];
 level_t* Monitor[MONIT_ITEMS_NUMBER];
 level_t* EventPar[EVENTS_PARAM_NUMBER];



 statusWord_t statusWord;
 controlWord_t controlWord;
 controlWord_priv_t controlWord_priv = {.word = 0x2}; // Инициализация вспомогательного контрольного слова

ee_error_t error;

alarmTime_t formatTime;
alarmTime_t formatTime_A;
alarmTime_t formatTime_R;
alarmTime_t formatTime_P;

uint16_t run_enable;

bool editMode = false;
uint16_t indexDecoupled = 0;//индекс перемещения по разрядам числа в NumberEdit
uint16_t lenMax;//максимальная длинна числа в NumberEdit
int cursorTime;// при мигании признак отображения курсора



//Function declaration
uint16_t KeypadRefRead(uint16_t dataAdress);							// Чтение параметра из памяти EEPROM
uint16_t KeypadRefWrite(uint16_t dataAdress, int dataValue);            // Запись параметра в EEPROM


// int32_t to string (null terminated). Buf size need 12 bytes max
void my_itoa(int32_t value, char *str) {
	uint32_t pvalue;
	int len;
	bool subz = (value < 0);
	if (subz) pvalue = -value;
		else pvalue = value;

	if (pvalue < 10) len = 1;
	else if (pvalue < 100) len = 2;
	else if (pvalue < 1000) len = 3;
	else if (pvalue < 10000) len = 4;
	else if (pvalue < 100000) len = 5;
	else if (pvalue < 1000000) len = 6;
	else if (pvalue < 10000000) len = 7;
	else if (pvalue < 100000000) len = 8;
	else if (pvalue < 1000000000) len = 9;
		else len = 10;
	if (subz) len++;
	str += len;
	*str = 0;
  do {
    *--str = pvalue % 10 + '0';
    pvalue /= 10;
  } while (pvalue != 0);
	if (subz) *--str = '-';
}

uint8_t crc8(uint8_t *addr, uint8_t len)
{
    uint8_t crc = 0;

    while (len--)
    {
        uint8_t inbyte = *addr++;
        for (uint8_t i = 8; i; i--)
        {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix)
                crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}



void OutDiagInfo(int type) {
	int i;

    ST7565_clear();
    if (type == 0) ST7565_drawstring(DISP_LEFT_BOUND - FONT_GAP * 0, 0, "No connect after USBReEnum. RESET!");
    		else ST7565_drawstring(DISP_LEFT_BOUND - FONT_GAP * 0, 0, "40 times no response. RESET!");
	char AsciiBuf[8];
	for (i = 0; i < 6; i++) {
		my_itoa(tx_buffer[i], AsciiBuf);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, i+2, AsciiBuf);
		my_itoa(rx_buffer[i], AsciiBuf);
		ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 4, i+2, AsciiBuf);
	}
	my_itoa(USBTransmitted, AsciiBuf);
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 8, 2, AsciiBuf);
	my_itoa(USBReceived, AsciiBuf);
	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 8, 3, AsciiBuf);
	ST7565_display();
	vTaskDelay(5000);
    NVIC_SystemReset();
}

int16_t USBRequest(uint8_t CMD, uint16_t dataAdress, uint16_t dataValue) {
    uint16_t noResponseCount = 0;
    uint16_t i;
    while (1) {
    	// каждый раз заполняю буфер на отправку, так как иногда USB библиотека его портит
        tx_buffer[0] = CMD;// 0 - Read param, 1 - Write param
        tx_buffer[1] = dataAdress;//Lo
        tx_buffer[2] = dataAdress >> 8;
        tx_buffer[3] = dataValue;//Lo;
        tx_buffer[4] = dataValue >> 8;
        tx_buffer[5] = crc8(tx_buffer, 5); // добавляем к посылке чексумму, алгоритм crc8

    	rx_buffer[0] = 0xFE;// порчу буфер принятых данных, чтобы если в следующий раз ЧРП не ответит, не обработь случайно предыдущие данные

    	noResponseCount++;//заранее увеличиваю
    	if (USBDataRequest() == USBH_OK) {
            for (i = 0; i < ANSWER_DELAY; i++) {
            	if (USBTransmitted) break;
                vTaskDelay(1);
            }
            if (USBTransmitted) {
				USBDataRead();// вычитываю больше чем надо, на случай, если в приеме остались какие-то хвосты
				for (i = 0; i < ANSWER_DELAY; i++) {// бывает отказ чтения, поэтому запрашиваю чтение много раз
					if (USBReceived) break;// бывает сразу при USBDataRequest происходит сразу и передача и прием, и нет неоходимости ещё раз запрашивать прием
					vTaskDelay(1);
				}

				if (USBReceived &&
					(rx_buffer[1] == tx_buffer[1]) &&
					(rx_buffer[2] == tx_buffer[2]) &&
					(crc8(rx_buffer, 5)) == (rx_buffer[5])) {// проверяем чексумму принятого сообщения, если не совпадает - шлем запрос заново

					uint16_t data = (rx_buffer[4] << (8)) | (rx_buffer[3]);
					return data;
				}
            }
        }

    	if (noResponseCount > 2) {
            ST7565_clear();
            ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 20, 3, USB_NO_CONNECT_MSG);
            ST7565_display();
            if (Appli_state == APPLICATION_READY) {
				Appli_state = APPLICATION_DISCONNECT;// сбрасываю перед ReEnum, так как уже в ReEnum происходит конект, и после его состояние портить нельзя
				MX_USB_HOST_ReEnumerate();// без этого не начинает работать со включенной оптимизацией. Без оптимизации сюда при включении не заходит.
				for (i = 0; i < 1000; i++) {// подождать для соединения. Обычно хватает 100
					vTaskDelay(1);
					if (Appli_state == APPLICATION_READY) break;//USB готов к работе
				}
	            if (Appli_state != APPLICATION_READY) {
//	            	OutDiagInfo(0);
	                ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 1, 5, "Fault ReEnum RESET!");
	                ST7565_display();
	                vTaskDelay(500);
	            	NVIC_SystemReset();
	            }
            }
            if (noResponseCount > 40) {
//            	OutDiagInfo(1);
                ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 2, 5, "40 no resp RESET!");
                ST7565_display();
                vTaskDelay(500);
                NVIC_SystemReset();
            }
        }
    }
}

//data request and write USB
int16_t IntDataRequest(uint16_t dataAdress) {
    // Monitor селекторы считываю из EEPROM по адресам 1,2,3
    uint8_t buf;

    if ((dataAdress >= 0xF000) && (dataAdress <= 0xF002)) {
    	vTaskDelay(EEPROM_DELAY);
    	if ((HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, dataAdress - 0xF000 + 1, I2C_MEMADD_SIZE_16BIT, &buf, 1, EEPROM_TIME_OUT) != HAL_OK) || (buf == 255)) {
            // вернуть значение по умолчанию
            switch (dataAdress) {
    		case 0xF000: return var_Freq;
    		case 0xF001: return var_Udc;
    		default: return var_Tigbt;
    		}
    	}
    	vTaskDelay(EEPROM_DELAY);
    	return buf;
    }
    return USBRequest(0 /*0 - Read */, dataAdress, 0);
}

uint16_t uIntDataRequest(uint16_t dataAdress) {
	return (uint16_t) IntDataRequest(dataAdress);
}

uint16_t IntDataWrite(uint16_t dataAdress, int dataValue)
{
/*
	uint8_t wbuf;
	uint8_t rbuf;

    // Monitor селекторы сохраняю в EEPROM по адресам 1,2,3
    if ((dataAdress >= 0xF000) && (dataAdress <= 0xF002))
    	{
        wbuf = dataValue;// беру только младший байт, больше не надо
    	vTaskDelay(EEPROM_DELAY);
    	if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADR, dataAdress - 0xF000 + 1, I2C_MEMADD_SIZE_16BIT, &wbuf, 1, EEPROM_TIME_OUT) == HAL_OK)
    		{
        	vTaskDelay(EEPROM_DELAY);
    		if (HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, dataAdress - 0xF000 + 1, I2C_MEMADD_SIZE_16BIT, &rbuf, 1, EEPROM_TIME_OUT) == HAL_OK)
        		{
            	vTaskDelay(EEPROM_DELAY);
            	if (rbuf != wbuf)
            		{
                    ST7565_clear();
                    ST7565_drawstring(DISP_RIGHT_BOUND, 4, "ERROR VERIFICATION");
                    ST7565_drawstring(DISP_RIGHT_BOUND, 5, "WRITE/READ EEPROM");
                    ST7565_display();
                    vTaskDelay(5000);
            		}
        		}
        	else
        		{
            	vTaskDelay(EEPROM_DELAY);
                ST7565_clear();
                ST7565_drawstring(DISP_RIGHT_BOUND, 4, "ERROR READ EEPROM");
                ST7565_display();
                vTaskDelay(5000);
        		}
    		}
    	else
    		{
        	vTaskDelay(EEPROM_DELAY);
            ST7565_clear();
            ST7565_drawstring(DISP_RIGHT_BOUND, 4, "ERROR WRITE EEPROM");
            ST7565_display();
            vTaskDelay(5000);
    		}
    	return wbuf;
    	}
*/
    return USBRequest(1 /*1 - Write */, dataAdress, dataValue);
}

//key functions
uint8_t KeyChekSem(SemaphoreHandle_t semaphore)
{
    uint8_t res;

    if (semaphore != NULL)
    	{
        if (xSemaphoreTake(semaphore, (portTickType)0) == pdTRUE) {res = 1;}
        else {res = 0;}
    	}
    else {res = 0;}

    return res;
}

uint8_t PointerUp(level_t * obj, SemaphoreHandle_t semaphore)
{
    uint8_t keyPush = KeyChekSem(semaphore);
    if (keyPush)
    {
        if (obj->Pointer < obj->ValueMax)
        {
            obj->Pointer++;
        } else
        {
            obj->Pointer = obj->ValueMin + 1;
        }
    }
    return keyPush;
}

uint8_t PointerDown(level_t * obj, SemaphoreHandle_t semaphore)
{
    uint8_t keyPush = KeyChekSem(semaphore);
    if (keyPush)
    {
        if (obj->Pointer > obj->ValueMin + 1)
        {
            obj->Pointer--;
        } else
        {
            obj->Pointer = obj->ValueMax;
        }
    }
    return keyPush;
}

bool ValUp(Menu_t * obj, SemaphoreHandle_t semaphore) {
	bool Pressed = KeyChekSem(semaphore);
    if (Pressed) {
        if (obj->idx.Value < obj->idx.ValueMax) {
            obj->idx.Value++;
            obj->main.idx.Pointer = 1;
            obj->main.idx.Value = 0;
        }
        else obj->idx.Value = obj->idx.ValueMin;
    }
    return Pressed;
}

void ReturnBack(level_t* obj)
{
    obj->Value = obj->ValueMin;
}

void ValReset(level_t * obj, SemaphoreHandle_t semaphore)
{
    if (KeyChekSem(semaphore))
    {
        ReturnBack(obj);
    }
}

uint8_t ValSelect(level_t * obj, SemaphoreHandle_t semaphore)
{
    uint8_t keyPush = KeyChekSem(semaphore);
    if (keyPush)
    {
        if (obj->Value < obj->ValueMax)
        {
            obj->Value = obj->Pointer;
        } else
        {
            obj->Value = obj->ValueMin;
        }
    }
    return keyPush;
}

uint16_t SetValue(int inVal, int* tosetVal, SemaphoreHandle_t semaphore)
{
    uint8_t keyPush = KeyChekSem(semaphore);
    if (keyPush)
    {
        *tosetVal = inVal;
    }
    return keyPush;
}

uint16_t Write(int inVal, uint16_t dataAdress, SemaphoreHandle_t semaphore)
{
    uint16_t result = KeyChekSem(semaphore);
    if (result)
    {
        IntDataWrite(dataAdress, inVal);
    }
    return result;
}

// ToDO:
// Функцию CW_Command нужно переделать таким образом,
// чтобы регистр controlWord отправлялся только один раз
// при выполнении функции DisplayStatic()

uint16_t CW_Command(uint8_t key_func, int data, controlWord_t* cw, SemaphoreHandle_t semaphore)
{

    uint16_t result = KeyChekSem(semaphore);


    if (result)
    {
        switch (key_func)
        {

        case START_STOP:
        	if (statusWord.bits.ready == true)
        	{
        		cw->bits.start_stop = data;
        		controlWord_priv.bits.stop_flag = true;

        	}

            if (semaphore == xButtonStopSemaphore)
            	{
//                controlWord_priv.bits.stop_flag = !controlWord_priv.bits.stop_flag;
//                IntDataWrite(CW_PRIV_ADR, controlWord_priv.word);
            	}
        break;

     /*
        case START_STOP:

        	bit_run = 1;
        	bit_stop = 1;
        	STOP = bit_stop & SW.bit.fault;

        	START = RS_Trigger_run(bit_run, STOP);
        	cw->bits.start_stop = START;

        break;
*/
        case DIRECT:
        	cw->bits.direction = data;
        break;

        /*
        case ALARM_RES:
            cw->bits.start_stop = 0;
            cw->bits.fault_res = data;
//            cw->bits.direction ^= 1;// так как перед AlarmEsc срабатывает просто Esc, то переключаем направление ещё раз обратно
        break;
        */

        case ALARM_RES:
            cw->bits.start_stop = 0;
            cw->bits.fault_res = data;
       //            cw->bits.direction ^= 1;// так как перед AlarmEsc срабатывает просто Esc, то переключаем направление ещё раз обратно
        break;



        default:
        break;
        }
    }

    if (statusWord.bits.fault == 0) {cw->bits.fault_res = 0;}         // Сброс бита fault_res
    if (statusWord.bits.ready == 0) {cw->bits.start_stop = 0;}        // Сброс бита fault_res

 /*
    if (controlWord_priv.bits.stop_flag == true)
    	{
    	if (statusWord.bits.start_stop == true) {controlWord_priv.bits.stop_flag = false;}
    	}
    else
    	{
    	if (statusWord.bits.start_stop == false) {cw->bits.start_stop = false;}
    	}
 */

//    if ((statusWord.bits.start_stop == 0) & (controlWord_priv.bits.stop_flag == 0)){cw->bits.start_stop = 0;} // Cброс бита start_stop

    IntDataWrite(CW_ADR, cw->word);

    if (cw -> bits.rstToFactory == true) {cw -> bits.rstToFactory = false;}

    return result;
}

//visualisation and control functions

//void ListView(int * valMax, int * pointer, int * value, const char* text, uint16_t size, uint16_t option)
void ListView(int valMax, int * pointer, int * value, const char* text, uint16_t size, uint16_t option)
{
    int i; 			// счетчик строк дисплея (принимает значения 2, 4, 6)
	int k; 			// используется для вывода трех последовательных значений из списка (принимает значения 0, 1, 2)
	int l;
	int max, pnt;
    if (option == NAVIG_DISP_STRUCT)
    {
        pnt = *pointer - 1;
 //       max = *valMax - 1;
        max = valMax - 1;
    }
    else
    {
        pnt = *pointer;
 //       max = *valMax;
        max = valMax;
    }

    int pntDiv = (unsigned int) ((pnt) / 3);
    int pntMod = (unsigned int) ((pnt) % 3);

    k = 0;
    for (i = 2; i < 8; i = i + 2)
    {
        l = pntDiv * 3 + k++;
   //     l = pnt + k++;
        if (l <= max)
        {
            ST7565_drawstring(2, i, (text + l * size));

   // печатает символ "<" напротив установленного значения параметра
   // возможно это вообще не нужно
            if (option == NAVIG_PARAMETR)
            {
                if (l == *value)
                {
                    ST7565_drawstring(120, i, "<");
                }
            }


        }
    }

    ST7565_inv_fillrect(0, 15 + (16 * pntMod), 128, 9, 1);
}

void NumberValView(int32_t value, int bindec , int factor, uint8_t x, uint8_t str, uint8_t justi) {
    char dataAscii[17];
    uint8_t len;
    uint16_t uvalue = value;
    float res = ((float) value / factor);

    int fact;
    if (factor == 1) fact = 0;
    else if (factor == 10) fact = 1;
    else if (factor == 100) fact = 2;
    else if (factor == 1000) fact = 3;
    else fact = 4;

    if (bindec == BIN) {
		for (int i = 0; i < factor; i++) dataAscii[factor - 1 - i] = '0' + ((uvalue >> i) & 1);
		dataAscii[factor] = 0;
    }
    else ftoa(res, dataAscii, fact);

    len = strlen(dataAscii);

    if (justi == JUSTI_LEFT) ST7565_drawstring(x, str, dataAscii);
    else ST7565_drawstring(x - FONT_GAP * len, str, dataAscii);
}
//navigations and parametr change functions

uint16_t NavigateMenu(level_t * from_obj, level_t * to_obj, const char* list, uint16_t size, uint16_t option, bool WorkEsc)
{
    uint16_t status;
    //TextView(to_obj, list, size);
   // ListView(&to_obj->ValueMax, &to_obj->Pointer, &to_obj->Value, list, size, option);
    ListView(to_obj->ValueMax, &to_obj->Pointer, &to_obj->Value, list, size, option);

    PointerUp(to_obj, xButtonDownSemaphore);
    PointerDown(to_obj, xButtonUpSemaphore);
    status = ValSelect(to_obj, xButtonEnterSemaphore);
    if (WorkEsc) ValReset(from_obj, xButtonResetSemaphore);//запретить выход выше по Esc в 3-м меню (ПАРАМЕТРЫ, АРХИМ и т.д.)
    return status;
}

void TimeRequest (uint16_t adr, alarmTime_t* timeData)
{
    timeData->sec = uIntDataRequest(adr) % 60;
    timeData->min = uIntDataRequest(adr) / 60;
    timeData->hour = uIntDataRequest(adr + 1);
    timeData->day = uIntDataRequest(adr + 2);
}

void TimeView (alarmTime_t* timeData, uint16_t str,  uint16_t left_bound)
{
    NumberValView(formatTime.day,DEC, 1, left_bound+FONT_GAP *16, str, JUSTI_RIGHT);
    ST7565_drawstring(left_bound + FONT_GAP * 16, str, "д");
    //4
    NumberValView(formatTime.hour,DEC, 1, left_bound+FONT_GAP * 10, str, JUSTI_RIGHT);
    ST7565_drawstring(left_bound + FONT_GAP * 10, str, "ч");
    //8
    NumberValView(formatTime.min,DEC, 1, left_bound+FONT_GAP * 6, str, JUSTI_RIGHT);
    ST7565_drawstring(left_bound + FONT_GAP * 6, str, "м");
    //12
    NumberValView(formatTime.sec,DEC, 1, left_bound+FONT_GAP * 2, str, JUSTI_RIGHT);
    ST7565_drawstring(left_bound + FONT_GAP * 2, str, "с");
}

bool CheckMinMax(param_t param)
{
    if ( param.value >= param.min && param.value <= param.max)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void NavigateParam(level_t* from_obj, level_t* to_obj, uint16_t* paramNumberPointer, group_t* groupData) {
    PointerUp(to_obj, xButtonDownSemaphore);
    PointerDown(to_obj, xButtonUpSemaphore);

    ValReset(from_obj, xButtonResetSemaphore);

    switch (groupData->param[*paramNumberPointer].type) {
    case PAR_LIST:
    case PAR_MLIST:
        ValSelect(to_obj, xButtonEnterSemaphore);
        int i = IntDataRequest(groupData->param[*paramNumberPointer].adr);
        groupData->param[*paramNumberPointer].value = i;
        groupData->param[*paramNumberPointer].pointer = i;//чтобы когда заходишь внутрь параметра, был выбран текущий элемент списка

        if (CheckMinMax(groupData->param[*paramNumberPointer]) ) ST7565_drawstring(DISP_LEFT_BOUND, 6, listBase[groupData->param[*paramNumberPointer].listBase_id][groupData->param[*paramNumberPointer].value]);
        else ST7565_drawstring(DISP_LEFT_BOUND, 6, "НИЗВЕСТ ЗНАЧ");
        break;

    case PAR_INT:
        ValSelect(to_obj, xButtonEnterSemaphore);// value = pointer
        groupData->param[*paramNumberPointer].value = IntDataRequest(groupData->param[*paramNumberPointer].adr);
        //отображаем
        NumberValView(groupData->param[*paramNumberPointer].value, DEC,groupData->param[*paramNumberPointer].factor, DISP_LEFT_BOUND + FONT_GAP * NUM_MAX_LEN, 6, JUSTI_RIGHT);
        //выводим единицы измерения
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * (NUM_MAX_LEN + 1), 6, groupData->param[*paramNumberPointer].units);
        break;

    case PAR_UINT:
        ValSelect(to_obj, xButtonEnterSemaphore);// value = pointer
        groupData->param[*paramNumberPointer].value = uIntDataRequest(groupData->param[*paramNumberPointer].adr);
        //отображаем
        NumberValView(groupData->param[*paramNumberPointer].value, DEC, groupData->param[*paramNumberPointer].factor, DISP_LEFT_BOUND + FONT_GAP * NUM_MAX_LEN, 6, JUSTI_RIGHT);
        //выводим единицы измерения
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * (NUM_MAX_LEN + 1), 6, groupData->param[*paramNumberPointer].units);
        break;

    case PAR_ARCHIVE:
        ValSelect(to_obj, xButtonEnterSemaphore);
        groupData->param[*paramNumberPointer].value = uIntDataRequest(groupData->param[*paramNumberPointer].adr);

        switch (groupData->param[*paramNumberPointer].value) {
        case 252:
            ST7565_drawstring(DISP_LEFT_BOUND, 4, listBase[groupData->param[*paramNumberPointer].listBase_id][14]);
            break;

        case 253:
            ST7565_drawstring(DISP_LEFT_BOUND, 4, listBase[groupData->param[*paramNumberPointer].listBase_id][15]);
            break;

        case 254:
            ST7565_drawstring(DISP_LEFT_BOUND, 4, listBase[groupData->param[*paramNumberPointer].listBase_id][16]);
            break;

        case 255:
            ST7565_drawstring(DISP_LEFT_BOUND, 4, listBase[groupData->param[*paramNumberPointer].listBase_id][17]);
            break;

        default:
            ST7565_drawstring(DISP_LEFT_BOUND, 4, listBase[groupData->param[*paramNumberPointer].listBase_id][groupData->param[*paramNumberPointer].value]);
            break;
        }
        TimeRequest (groupData->param[*paramNumberPointer].adr + 6, &formatTime);
        TimeView (&formatTime, 6,  0);
        break;

    case PAR_BLIST:
        KeyChekSem(xButtonEnterSemaphore);
        ST7565_drawstring(DISP_LEFT_BOUND, 6, listBase[groupData->param[*paramNumberPointer].listBase_id][groupData->param[*paramNumberPointer].value]);
        break;

    default:
        ST7565_drawstring(DISP_LEFT_BOUND, 6, "не изв тип параметра");
        break;
    }
}

bool ScrollValue(int valMax, int valMin, int *pointer, int type) {
	bool UpPressed = KeyChekSem(xButtonUpSemaphore);
	bool DownPressed = KeyChekSem(xButtonDownSemaphore);
	if (type == SCROLL_VALUE){// упирается в мин и макс, для значений
        if (DownPressed) {
        	if (*pointer > valMin) *pointer -= 1;
        	else *pointer = valMin;
        }
        if (UpPressed) {
        	if (*pointer < valMax) *pointer += 1;
        	else *pointer = valMax;
        }
	}
	else {// циклическое скроллирование, для списков
        if (*pointer < valMax) {
            if (*pointer > valMin) {
            	if (DownPressed) *pointer += 1;
            	if (UpPressed) *pointer -= 1;
            }
            else {
            	if (DownPressed) *pointer += 1;
            	if (UpPressed) *pointer = valMax;
            }
        }
        else {
        	if (DownPressed) *pointer = valMin;
        	if (UpPressed) *pointer -= 1;
        }
	}
    return UpPressed || DownPressed;
}

int16_t MyAbs(int16_t i)
{
	if (i < 0) {return -i;}
	{return i;}
}

int16_t MyMax(int16_t i1, int16_t i2)
{
	if (i1 > i2) {return i1;}
	else {return i2;}
}

bool NumberEdit(param_t * parametr, int16_t xPos, uint16_t numStr)
{

	int num;

	if (parametr->adr == FREQ_REF_ADR)
		{
		if (IntDataRequest(SEL_PRC_RPM_ADR) == 0) {parametr = &keyRef_hz;}
			else {parametr = &keyRef_prc;}
		}

	int min = parametr->min;
	int max = parametr->max;
	int factor = parametr->factor;
	int pointer = parametr->value;

	// выводим название, единицы измерения, мин и макс значения

	if (xPos >= 0)
		{
		ST7565_drawstring(xPos + FONT_GAP * 3, numStr - 2, &(parametr->name[3]));
		//выводим единицы измерения
		ST7565_drawstring(xPos + FONT_GAP * (lenMax + 1), numStr, parametr->units);
		//отображаем min
		ST7565_drawstring(DISP_LEFT_BOUND, 7, " мин");
		NumberValView(min, DEC, factor, DISP_LEFT_BOUND, 6, JUSTI_LEFT);
		//отображаем max
		ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 4, 7, "макс");
		NumberValView(max, DEC, factor, DISP_RIGHT_BOUND, 6, JUSTI_RIGHT);
		}

	 // Режим редактирования параметра
    if (editMode) {
    	// вывожу минус в самой левой части
    	if (pointer < 0) {ST7565_drawstring(xPos, numStr, "-");}
    	// заполняю нулями
    	int i;
    	for (i = 1; i < lenMax-1; i++) {ST7565_drawstring(xPos + FONT_GAP * i, numStr, "0");}
    	// вывожу положительное значение с плавающей запятой
		NumberValView(MyAbs(pointer), DEC, factor, xPos + FONT_GAP * lenMax, numStr, JUSTI_RIGHT);
	    int fact;
	    int pointLen = 1;
	    if (factor == 1) {fact = 0;	pointLen = 0;}

	    else if (factor == 10) fact = 1;
	    else if (factor == 100) fact = 2;
	    else if (factor == 1000) fact = 3;
	    else fact = 4;

        //переключаемся между разрядами
        if (KeyChekSem(xButtonFuncSemaphore))
        	{
        	indexDecoupled++;
            if (indexDecoupled >= lenMax - pointLen) indexDecoupled = 0;
        	}
        //отображаем курсор
        cursorTime++;
        if (cursorTime >= 5) cursorTime = 0;
        if (cursorTime < 2) {
        	if ((fact > 0) & (indexDecoupled >= fact)) ST7565_drawstring(xPos + FONT_GAP * (lenMax - indexDecoupled - 2), numStr, "_");
        		else ST7565_drawstring(xPos + FONT_GAP * (lenMax - indexDecoupled - 1), numStr, "_");
        }
        //меняю значения разрядов
        int delta;
	    if (indexDecoupled == 0) delta = 1;
	    else if (indexDecoupled == 1) delta = 10;
	    else if (indexDecoupled == 2) delta = 100;
	    else if (indexDecoupled == 3) delta = 1000;
	    else delta = 10000;
        if ((indexDecoupled == lenMax - pointLen -1)) {
            if (KeyChekSem(xButtonUpSemaphore) | KeyChekSem(xButtonDownSemaphore)) pointer = -pointer;
        }
        else {
			if (KeyChekSem(xButtonDownSemaphore)) {
				if (pointer - delta >= min) pointer -= delta;
				else pointer = min;
			}
			if (KeyChekSem(xButtonUpSemaphore)) {
				if (pointer + delta < max) pointer += delta;
				else pointer = max;
			}
        }

/*
        //проверяем выход за пределы
        if (pointer > max) parametr->pointer = max;
        else if (pointer < min) parametr->pointer = min;
        else parametr->pointer = pointer;


        //отправляем значение по Enter
        if (SetValue(parametr->pointer, &parametr->value, xButtonEnterSemaphore))
        	{
            IntDataWrite(parametr->adr, parametr->value);
            editMode = false;
        	}
*/

        //проверяем выход за пределы

        if (pointer > max) parametr->value = max;
        else if (pointer < min) parametr->value = min;
        else parametr->value = pointer;
        num = parametr->value;

            if (KeyChekSem(xButtonEnterSemaphore))
            {
            	if (IntDataRequest(SEL_PRC_RPM_ADR) == 0)
            		{
            		IntDataWrite(parametr->adr, num);						//Отправляем по USB (Hz)
            		KeypadRefWrite(5, num);						            //Запись в EEPROM   (Hz)
            		parametr = &keyRef_prc;
            		parametr->value = 2.0*num;								//Сохраняем параметр в процентах (Hz ->%)
            		}
            	else
            		{
            		IntDataWrite(parametr->adr, num/2.0);                   //Отправляем по USB (% -> Hz)
            	    KeypadRefWrite(5, num/2.0);						        //Запись в EEPROM   (% -> Hz)
            	    parametr = &keyRef_hz;
            	    parametr->value = num/2.0;								//Сохраняем параметр в Герцах    (% ->Hz)
            		}
            	editMode = false;
            }


        //выходим из edit mode по Reset
        if (KeyChekSem(xButtonResetSemaphore)) {editMode = false;}

    }
    // Режим просмотра параметра
    else {

		if (xPos >= 0) {
			// parametr->pointer = parametr->value;
			//максимальное количество разрядов
		    char lenMinAscii[12];
		    char lenMaxAscii[12];
			my_itoa(MyAbs(min), lenMinAscii);
			my_itoa(MyAbs(max), lenMaxAscii);
			lenMax = MyMax(strlen(lenMinAscii), strlen(lenMaxAscii)) + 1;// +1 для знака
		    if (factor > 1) lenMax++;// ещё точка будет место занимать

			//отображаем значение параметра


			NumberValView(parametr->value, DEC, factor, xPos + FONT_GAP * lenMax, numStr, JUSTI_RIGHT);

			if ((parametr->adr != FREQ_REF_ADR) | KeyChekSem(xButtonEnterSemaphore)) {
				//сразу вхожу в edit mode
				indexDecoupled = 0;
				cursorTime = 5;
				editMode = true;
			}
		}
	}
	return !editMode;
}

void ParametrEdit(param_t* parametr, level_t* from_obj) {
    switch (parametr->type) {
    case PAR_LIST:
    case PAR_MLIST:
        parametr->value = IntDataRequest(parametr->adr);
        ScrollValue(parametr->max, parametr->min, &(parametr->pointer), SCROLL_LIST);
 //       ListView(&(parametr->max), &(parametr->pointer), &(parametr->value), &listBase[parametr->listBase_id][0][0], MAX_STRING, NAVIG_PARAMETR);
        ListView(parametr->max, &(parametr->pointer), &(parametr->value), &listBase[parametr->listBase_id][0][0], MAX_STRING, NAVIG_PARAMETR);
        if (SetValue(parametr->pointer, &(parametr->value), xButtonEnterSemaphore))
        	{
			IntDataWrite(parametr->adr, parametr->value);
            ReturnBack(from_obj);
        	}
        ValReset(from_obj, xButtonResetSemaphore);
        break;

    case PAR_INT:
    case PAR_UINT:
        if (NumberEdit(parametr, NUMBER_EDIT_X, NUMBER_EDIT_STR)) ReturnBack(from_obj);// from_obj->value = from_obj->min
        break;

    case PAR_BLIST:
        ValReset(from_obj, xButtonResetSemaphore);
        break;

    default:
        ST7565_drawstring(2, 4, "НЕИЗВЕСТНЫЙ ТИП ПАРАМТРА");
        break;
    }
}

//monitoring mode

void MessegeCounter()
{
    char dataAscii[12];
    my_itoa(txMessageCount, dataAscii);
    ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 21, 7, "ОТПР:");
    ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 16, 7, dataAscii);

    my_itoa(rxMessageCount, dataAscii);
    ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 10, 7, "ПРИН:");
    ST7565_drawstring(DISP_RIGHT_BOUND - FONT_GAP * 5, 7, dataAscii);
}

void StatusMonitor()
{
    char dataAscii[12];
    char modbusADR[12];
    uint16_t fault_word;
    uint16_t alarm_word;
    char fault_num;
    char eventAscii[2];


    ST7565_drawline(0, 10, 128, 10, 1);

    statusWord.word = uIntDataRequest(SW_ADR);

    //Вывод номера преобразователя//
    my_itoa(IntDataRequest(MODBUSADR_ADR), modbusADR);
    ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 0, 0,  modbusADR);

    //Вывод LOCAL/REMOTE//
    if (IntDataRequest(LOC_REM_ADR) == 0) ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 3, 0, "R");
    else ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 3, 0, "L");

    //Вывод сигнала Fault
    if (statusWord.bits.fault)
    {
        fault_word = IntDataRequest(FW_ADR);

        if ((fault_word & 0x0001) != 0){fault_num = 1;}
        if ((fault_word & 0x0002) != 0){fault_num = 2;}
        if ((fault_word & 0x0004) != 0){fault_num = 3;}
        if ((fault_word & 0x0008) != 0){fault_num = 4;}
        if ((fault_word & 0x0010) != 0){fault_num = 5;}
        if ((fault_word & 0x0020) != 0){fault_num = 6;}
        if ((fault_word & 0x0040) != 0){fault_num = 7;}
        if ((fault_word & 0x0080) != 0){fault_num = 8;}
        if ((fault_word & 0x0100) != 0){fault_num = 9;}
        if ((fault_word & 0x0200) != 0){fault_num = 10;}
        if ((fault_word & 0x0400) != 0){fault_num = 11;}
        if ((fault_word & 0x0800) != 0){fault_num = 12;}
        if ((fault_word & 0x1000) != 0){fault_num = 13;}
        if ((fault_word & 0x2000) != 0){fault_num = 14;}
        if ((fault_word & 0x4000) != 0){fault_num = 15;}
        if ((fault_word & 0x8000) != 0){fault_num = 16;}

        my_itoa(fault_num, eventAscii);

        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 5, 0, "E");//FAULT_WORD
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 6, 0, eventAscii);

        //ToDO: доделать вывод кода аварии
        //Вывод HEX кода FAULT_WORD

        //NumberValView(fault_word, HEX, 4/*кол-во символов*/, DISP_LEFT_BOUND + FONT_GAP * 7, 0, 0);
    }

    // Старая реализация сигнала Fault в скалярном XD500
    /*event = IntDataRequest(LAST_EVENT);
    if ((event != 255) & (event != 254) & (event != 0) & (statusWord.bits.fault)) {
        my_itoa(event, eventAscii);
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 5, 0, "E");
        ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 6, 0, eventAscii);
    }*/

    //Вывод сигнала Alarm
    if (statusWord.bits.alarm)
    	{

        alarm_word = IntDataRequest(AW_ADR);

        if ((alarm_word & 0x0001) != 0){fault_num = 1;}
        if ((alarm_word & 0x0002) != 0){fault_num = 2;}
        if ((alarm_word & 0x0004) != 0){fault_num = 3;}
        if ((alarm_word & 0x0008) != 0){fault_num = 4;}
        if ((alarm_word & 0x0010) != 0){fault_num = 5;}
        if ((alarm_word & 0x0020) != 0){fault_num = 6;}
        if ((alarm_word & 0x0040) != 0){fault_num = 7;}
        if ((alarm_word & 0x0080) != 0){fault_num = 8;}
        if ((alarm_word & 0x0100) != 0){fault_num = 9;}
        if ((alarm_word & 0x0200) != 0){fault_num = 10;}
        if ((alarm_word & 0x0400) != 0){fault_num = 11;}
        if ((alarm_word & 0x0800) != 0){fault_num = 12;}
        if ((alarm_word & 0x1000) != 0){fault_num = 13;}
        if ((alarm_word & 0x2000) != 0){fault_num = 14;}
        if ((alarm_word & 0x4000) != 0){fault_num = 15;}
        if ((alarm_word & 0x8000) != 0){fault_num = 16;}

        my_itoa(fault_num, eventAscii);
    	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 8, 0, "A");
    	ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 9, 0, eventAscii);
    	}

    //Вывод частоты
    //my_itoa(IntDataRequest(FREQ_EST)/10.0, dataAscii);
    my_itoa(IntDataRequest(FREQ_REF)/10.0, dataAscii);
    ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 15, 0, dataAscii);
    ST7565_drawstring(0 + FONT_GAP * 19, 0, HZ);

    // Вывод направления вращения
    if (statusWord.bits.direction) ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 12, 0, "<-");
    else ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP * 12, 0, "->");

    if (statusWord.bits.fault) controlWord.bits.fault_res = 0;
}

void MonitorView (uint16_t mon_num)
{
    uint16_t monitor_sel = 0xFFFF;
    uint16_t data_adr;
    int factor;

    monitor_sel = uIntDataRequest(groups[MONITOR_GROUP_NUM].param[mon_num].adr);
    switch (monitor_sel) {
    case var_RO_status:	// 0131
    	data_adr = 0x0131;
    	factor = 3;// для этого параметра, означает количество бинарных разрядов
		break;
    case var_DI_status:	// 0132
    	data_adr = 0x0132;
    	factor = 5;// для этого параметра, означает количество бинарных разрядов
		break;
    case var_AI1_scale:	// 0133
    	data_adr = 0x0133;
    	factor = 10;
		break;
    case var_AI2_scale:	// 0134
    	data_adr = 0x0134;
    	factor = 10;
		break;
    case var_AIT_scale:	// 0135
    	data_adr = 0x0135;
    	factor = 10;
		break;
    case var_Udc:		// 0104
    	data_adr = 0x4407;
    	factor = 10;
		break;
    case var_rms_Vab:	// 0105
    	data_adr = 0x4406;
    	factor = 10;
		break;
    case var_rms_Iab:	// 0106
    	data_adr = 0x4403;
    	factor = 10;
		break;
    case var_FreqRef:	// 0102
    	data_adr = FREQ_REF_ADR;
    	factor = 100;
		break;
    case var_Freq:		// 0103
    	data_adr = 0x4401;
    	factor = 10;
		break;
    case var_Power:		// 0136
    	data_adr = 0x4405;
    	factor = 100;
		break;
    case var_Tigbt:		// 0107
    	data_adr = 0x4408;
    	factor = 10;
		break;
    case var_Encoder_Speed:// 0137
    	data_adr = 0x4402;
    	factor = 1;
		break;
    case var_Stator_Flux_Magn://0138
    	data_adr = 0x4409;
    	factor = 100;
		break;
    case var_TechPidCtrl_In:// 0139
    	data_adr = 0x0139;
    	factor = 10;
		break;
    case var_TechPidCtrl_Fdb:// 013A
    	data_adr = 0x013A;
    	factor = 10;
		break;
    case var_TechPidCtrl_Error://013B
    	data_adr = 0x013B;
    	factor = 10;
		break;
    case var_TechPidCtrl_Out:// 013C
    	data_adr = 0x013C;
    	factor = 10;
		break;
    case var_Control_word:// 0001
    	data_adr = CW_ADR;
    	factor = 16; //3;// для этого параметра, означает количество бинарных разрядов
		break;
    case var_Status_word:// 0101
    	data_adr = SW_ADR;
    	factor = 16; //6;// для этого параметра, означает количество бинарных разрядов
		break;
    case var_Fault_word:// 0109
    	data_adr = FW_ADR;
    	factor = 16; // для этого параметра, означает количество бинарных разрядов
		break;
    case var_Alarm_word:// 013D
    	data_adr = 0x013D;
    	factor = 16; //3;// для этого параметра, означает количество бинарных разрядов
		break;
    default:
    	data_adr = 0xFFFF;
    	factor = 1;
		break;
    }

    if (data_adr == 0xFFFF) {ST7565_drawstring(2, (2 + 2 * mon_num), "НЕИЗВ ПАРАМ");}
    else if (monitor_sel == var_Control_word) {ST7565_drawstring(2, (2 + 2 * mon_num), "CW");}
    else if (monitor_sel == var_Status_word) {ST7565_drawstring(2, (2 + 2 * mon_num), "SW");}
    else if (monitor_sel == var_Fault_word) {ST7565_drawstring(2, (2 + 2 * mon_num), "FW");}
    else if (monitor_sel == var_Alarm_word) {ST7565_drawstring(2, (2 + 2 * mon_num), "AW");}
    else {ST7565_drawstring(2, (2 + 2 * mon_num), listBase[MONITOR_LIST_BASE_NUM][monitor_sel]);}

    if ((monitor_sel == var_RO_status) || (monitor_sel == var_DI_status))
    {
    	NumberValView(IntDataRequest(data_adr), BIN, factor, 96, (2 + 2 * mon_num), JUSTI_LEFT);
    }
    else if ((monitor_sel == var_Control_word) || (monitor_sel == var_Status_word) || \
    		(monitor_sel == var_Fault_word) || (monitor_sel == var_Alarm_word))
    {
    	NumberValView(IntDataRequest(data_adr), BIN, factor, 127, (2 + 2 * mon_num), JUSTI_RIGHT);
    }
    else
    {
    	NumberValView(IntDataRequest(data_adr), DEC, factor, 96, (2 + 2 * mon_num), JUSTI_LEFT);
    }
}

//in developed screen
void InDevelop(Menu_t * obj, SemaphoreHandle_t semaphore)
{
    ST7565_fillrect(0, 0, 128, 64, 0);
    ST7565_drawstring(0, 1, "ЭТА СТРАНИЦА");
    ST7565_drawstring(0, 2, "В РАЗРАБОТКЕ...");
    ST7565_drawstring(0, 3, "НАЖМИТЕ КНОПКУ");
    ST7565_drawstring(0, 4, "RESET ДЛЯ");
    ST7565_drawstring(0, 5, "СБРОСА МЕНЮ");

    if (KeyChekSem(semaphore))
    {
        MenuInit(obj);
    }
}

//Archive

void ArchiveView(param_t* parametr ,level_t* obj)
{
    ValReset(obj, xButtonResetSemaphore);
    KeyChekSem(xButtonEnterSemaphore);
    KeyChekSem(xButtonDownSemaphore);
    KeyChekSem(xButtonUpSemaphore);

    for (int16_t num = 0; num < ARCHIVE_DATA_NUM; num++)
    {
        arch_data[num].data = uIntDataRequest(parametr->adr + arch_data[num].adr);

        switch (arch_data[num].pos)
        {
        case left:
            ST7565_drawstring(DISP_LEFT_BOUND, arch_data[num].str, arch_data[num].name);
            NumberValView(arch_data[num].data, arch_data[num].bindec, arch_data[num].fact,
                    DISP_LEFT_BOUND + FONT_GAP * 4 + 3, arch_data[num].str, JUSTI_LEFT);
            break;

        case right:
            ST7565_drawstring(DISP_CENTR, arch_data[num].str, arch_data[num].name);
            NumberValView(arch_data[num].data, arch_data[num].bindec, arch_data[num].fact,
                    DISP_CENTR + FONT_GAP * 5 + 3, arch_data[num].str, JUSTI_LEFT);
            break;

        default:
            break;
        }
    }
}

//eeprom functions

void EE_WritePage(uint8_t page_adr) {
    uint8_t i;
    uint16_t hal_error;
	EE_BufPage[EEPROM_PAGE_SIZE - 1] = crc8(EE_BufPage, EEPROM_PAGE_SIZE - EEPROM_CRC_SIZE - 1);
	for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
		vTaskDelay(EEPROM_DELAY);
//		hal_error = HAL_I2C_Mem_Write(&hi2c1, (uint16_t) EEPROM_ADR, page_adr * EEPROM_PAGE_SIZE, I2C_MEMADD_SIZE_16BIT, EE_BufPage, EEPROM_PAGE_SIZE, EEPROM_TIME_OUT);
		hal_error = HAL_I2C_Mem_Write(&hi2c1, (uint16_t) EEPROM_ADR, page_adr * EEPROM_PAGE_SIZE + i, I2C_MEMADD_SIZE_16BIT, &EE_BufPage[i], 1, EEPROM_TIME_OUT);
		vTaskDelay(EEPROM_DELAY);
		if (hal_error != HAL_OK) {
			ST7565_drawstring(DISP_LEFT_BOUND, 4, "   ОШИБКА EEPROM");
			ST7565_display();
			vTaskDelay(5000);
		}
	}
}

ee_error_t EE_ReadPage(uint8_t page_adr) {
    uint8_t try_cnt = 0;
    uint8_t i;
    uint16_t hal_error;

    for (try_cnt = 0; try_cnt < EEPROM_TRY_READ; try_cnt++) {
    	EE_BufPage[EEPROM_PAGE_SIZE - 1] = 0xFF;//чтобы ничего не считав не получить хороший crc8
    	for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
			vTaskDelay(EEPROM_DELAY);
//			hal_error = HAL_I2C_Mem_Read(&hi2c1, (uint16_t) EEPROM_ADR, page_adr * EEPROM_PAGE_SIZE, I2C_MEMADD_SIZE_16BIT, EE_BufPage, EEPROM_PAGE_SIZE, EEPROM_TIME_OUT);
			hal_error = HAL_I2C_Mem_Read(&hi2c1, (uint16_t) EEPROM_ADR, page_adr * EEPROM_PAGE_SIZE + i, I2C_MEMADD_SIZE_16BIT, &EE_BufPage[i], 1, EEPROM_TIME_OUT);
			vTaskDelay(EEPROM_DELAY);
			if (hal_error != HAL_OK) {
				ST7565_drawstring(DISP_LEFT_BOUND, 4, "   ОШИБКА EEPROM");
				ST7565_display();
				vTaskDelay(5000);
				return crc_not_correct;
			}
    	}
        if (crc8(EE_BufPage, EEPROM_PAGE_SIZE - EEPROM_CRC_SIZE - 1) == EE_BufPage[EEPROM_PAGE_SIZE - 1]) return no_error;
    }
    return crc_not_correct;
}

ee_error_t EE_PageNoteConv(ee_note_t* note, uint8_t* note_cnt, ee_cmd_t cmd) {
    ee_error_t error = no_error;

    for (uint8_t byte_cnt = 0; byte_cnt < EEPROM_NOTE_SIZE; byte_cnt++) {
        switch (cmd) {
        case upload:
            note->byte[byte_cnt] = EE_BufPage[*note_cnt * EEPROM_NOTE_SIZE + byte_cnt];
            break;

        case download:
        	EE_BufPage[*note_cnt * EEPROM_NOTE_SIZE + byte_cnt] = note->byte[byte_cnt];
            break;

        default:
            error = cmd_not_found;
            break;
        }
    }
    (*note_cnt)++;
    return error;
}

ee_error_t EE_PushPullNote(ee_note_t* note, uint8_t* note_cnt, uint8_t* page_cnt, ee_cmd_t cmd, ee_bound_flag_t bound) {
    ee_error_t error = no_error;

    if ((cmd == upload) && (bound == first)) {
        error = EE_ReadPage(*page_cnt);
        if (error != no_error) return error;
    }

    if (*note_cnt < (EEPROM_NOTE_IN_PAGE)) {
        error = EE_PageNoteConv(note, note_cnt, cmd);
        if ((cmd == download) && (bound == last))
        	EE_WritePage(*page_cnt);
    }
    else {
        switch (cmd) {
        case download:
            EE_WritePage(*page_cnt);
            (*page_cnt)++;
            *note_cnt = 0;
            error = EE_PageNoteConv(note, note_cnt, cmd);
            if ((bound == last))
            	EE_WritePage(*page_cnt);
            break;

        case upload:
            (*page_cnt)++;
            *note_cnt = 0;
            error = EE_ReadPage(*page_cnt);
            if (error != no_error) return error;
            error = EE_PageNoteConv(note, note_cnt, cmd);
            break;

        default:
            error = cmd_not_found;
            break;
        }
    }
    return error;
}

ee_error_t EE_ProcessVisual(ee_cmd_t cmd, uint8_t group_cnt) {
    ee_error_t error = no_error;
    switch (cmd) {
    case download:
        ST7565_drawstring(DISP_LEFT_BOUND, 3, "КОПИРОВАНИЕ В ПУЛЬТ");
        break;
    case upload:
        ST7565_drawstring(DISP_LEFT_BOUND, 3, "ЗАГРУЗКА В XD500");
        break;

    default:
        error = cmd_not_found;
        break;
    }
    //Bar version
    ST7565_fillrect(5    , 36    , 7 * GROUPS_NUMBER    , 20, 1);
    ST7565_fillrect(5 + 2, 36 + 2, 7 * GROUPS_NUMBER - 4, 16, 0);
    ST7565_fillrect(5 + 2, 36 + 2, 7 * group_cnt              , 16, 1);
    ST7565_display();
    return error;
}

//ee_service_t serv_words = {.page = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

void BlockKeys()
{
    //keys blocking
    KeyChekSem(xButtonRunSemaphore);
    KeyChekSem(xButtonEnterSemaphore);
    KeyChekSem(xButtonFuncSemaphore);
    KeyChekSem(xButtonUpSemaphore);
    KeyChekSem(xButtonDownSemaphore);
    KeyChekSem(xButtonResetAlarmsSemaphore);
}







ee_error_t EE_MemoryComand(ee_cmd_t comand)
{
    uint8_t note_cnt = 0;
    uint8_t page_cnt = EEPROM_DATA_PAGE;
    uint8_t par_cnt = 0;
    uint8_t par_numb = 0;
    uint8_t group_cnt = 0;
    ee_error_t error = no_error;
    ee_bound_flag_t bound_flag = transit;
    ee_note_t note = { .data = 0 };

    controlWord_priv.bits.run_enable = 0;
    IntDataWrite(CW_PRIV_ADR, controlWord_priv.word);

    for (group_cnt = 0; group_cnt < GROUPS_NUMBER; group_cnt++)
    if (groups[group_cnt].save_to_eeprom) {
        par_numb = groups[group_cnt].maxParam;
        for (par_cnt = 0; par_cnt < par_numb; par_cnt++) {
            //Определение пограничности элемента - первый последний или промежуточный
            bound_flag = ((((group_cnt + 1) == GROUPS_NUMBER)) && ((par_cnt + 1) == par_numb)) ? last :
                         (((group_cnt == 0) && (par_cnt == 0))) ? first : transit;

            switch (comand) {
            case download:
                // чтение из Блока управления
                note.data = uIntDataRequest(groups[group_cnt].param[par_cnt].adr);
                //запись в EEPROM
                error = EE_PushPullNote(&note, &note_cnt, &page_cnt, download, bound_flag);
                //Визуализация процесса
                if (error == no_error) EE_ProcessVisual(download, group_cnt);
                break;

            case upload:
                //чтение из EEPROM
                error = EE_PushPullNote(&note, &note_cnt, &page_cnt, upload, bound_flag);
                //Визуализация процесса
                if (error == no_error) EE_ProcessVisual(upload, group_cnt);
                else break;
                // запись в Блок управления
                IntDataWrite(groups[group_cnt].param[par_cnt].adr, note.data);
                break;

            default:
                error = cmd_not_found;
                break;
            }
            if (error != no_error) break;
        }
        if (error != no_error) break;
    }
    controlWord_priv.bits.run_enable = 1;
    IntDataWrite(CW_PRIV_ADR, controlWord_priv.word);
    if (error != no_error) {
        ST7565_drawstring(DISP_LEFT_BOUND, 4, "   ОШИБКА EEPROM");
        ST7565_display();
        vTaskDelay(5000);
        ReturnBack(&Display.main.Memory.idx);
    }
    else MenuInit(&Display);

    return error;
}

ee_error_t EE_Main(ee_cmd_t comand) {
    ee_error_t error = no_error;

    ValReset(&Display.main.idx, xButtonResetSemaphore);
    if (!statusWord.bits.start_stop) {
    	uint8_t priznak;
    	if (comand == upload) {
			//Проверка не пустая ли память
        	vTaskDelay(EEPROM_DELAY);
        	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, 0, I2C_MEMADD_SIZE_16BIT, &priznak, 1, EEPROM_TIME_OUT);//читаю старое значение
        	vTaskDelay(EEPROM_DELAY);
			if (priznak == EEPROM_CONFIG_EXISTS) error = EE_MemoryComand(upload);
			else {
				ST7565_drawstring(DISP_LEFT_BOUND, 3, "  НЕТ  СОХРАНЕННОЙ");
				ST7565_drawstring(DISP_LEFT_BOUND, 4, "    КОНФИГУРАЦИИ");
				ST7565_drawstring(DISP_LEFT_BOUND, 5, "  ДЛЯ ЗАПИСИ В БЛОК");
				ST7565_display();
				vTaskDelay(5000);
				ReturnBack(&Display.main.Memory.idx);
				return memory_is_empty;
			}
    	}
    	else {// download
            error = EE_MemoryComand(download);
            if (error == no_error) {
            	// пишу, что в памяти есть конфигурация
            	if (priznak != EEPROM_CONFIG_EXISTS) {// если там ещё нет никакой конфигурации, пишу, что теперь есть
            		priznak = EEPROM_CONFIG_EXISTS;
            		HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADR, 0, I2C_MEMADD_SIZE_16BIT, &priznak, 1, EEPROM_TIME_OUT);
            		vTaskDelay(EEPROM_DELAY);
            	}
            }
    	}
    }
    else {
        ST7565_drawstring(DISP_LEFT_BOUND, 3, " НЕЛЬЗЯ ЗАГР/ВЫГРУЖ");
        ST7565_drawstring(DISP_LEFT_BOUND, 4, "КОНФИГУРАЦИЮ НА ВКЛ");
        ST7565_drawstring(DISP_LEFT_BOUND, 5, "  ПРЕОБРАЗОВАТЕЛЕ");
        ST7565_display();
        vTaskDelay(5000);
        ReturnBack(&Display.main.Memory.idx);
    	return vfd_is_started;
    }

    BlockKeys();
    return error;
}

void MenuInit(Menu_t * obj)
{
//    obj->idx.Pointer = 1;
//    obj->idx.Value = 1;
    obj->idx.ValueMax = 3;
    obj->idx.ValueMin = 1;
    if (obj->idx.Value < 1) obj->idx.Value = 1;// выполняется в первую инициализацию, когда Value == 0

    Mode = &(obj->idx);

    obj->main.idx.Pointer = 1;
    obj->main.idx.Value = 0;
    obj->main.idx.ValueMax = MENU_ITEMS_NUMBER;
    obj->main.idx.ValueMin = 0;

    Main_menu = &(obj->main.idx);

    obj->main.Memory.idx.Pointer = 1;
    obj->main.Memory.idx.Value = 0;
    obj->main.Memory.idx.ValueMax = MEMORY_ITEMS_NUMBER;
    obj->main.Memory.idx.ValueMin = 0;

    Memory_list = &(obj->main.Memory.idx);

    obj->main.Monitor.idx.Pointer = 1;
    obj->main.Monitor.idx.Value = 0;
    obj->main.Monitor.idx.ValueMax = MONITORS_NUMBER;
    obj->main.Monitor.idx.ValueMin = 0;

    Monitor_list = &(obj->main.Monitor.idx);

    obj->main.Param.idx.Pointer = 1;
    obj->main.Param.idx.Value = 0;
    obj->main.Param.idx.ValueMax = GROUPS_NUMBER - 1; //Последняя группа не отображается
    obj->main.Param.idx.ValueMin = 0;

    Groups_list = &(obj->main.Param.idx);

    for (int i = 0; i < GROUPS_NUMBER - 1; i++)
    {
        obj->main.Param.group[i].Pointer = 1;
        obj->main.Param.group[i].Value = 0;
        obj->main.Param.group[i].ValueMax = groups[i].maxParam;
        obj->main.Param.group[i].ValueMin = 0;

        Group[i] = &(obj->main.Param.group[i]);
    }

    obj->main.Archive.idx.Pointer = 1;
    obj->main.Archive.idx.Value = 0;
    obj->main.Archive.idx.ValueMax = EVEVENTS_NUMBER;
    obj->main.Archive.idx.ValueMin = 0;

    Archive_list = &(obj->main.Archive.idx);

    obj->main.fastParam.idx.Pointer = 1;
    obj->main.fastParam.idx.Value = 0;
    obj->main.fastParam.idx.ValueMax = GROUP_FAST_PAR_NUMB;
    obj->main.fastParam.idx.ValueMin = 0;

    Fast_list = &(obj->main.fastParam.idx);

}

//main diplay function

void VfdVerrsionConvert (char* vers_in, char* vers_out)
{
    vers_out[0] = '0';
    vers_out[1] = vers_in[0];
    vers_out[2] = '.';
    vers_out[3] = vers_in[1];
    vers_out[4] = vers_in[2];
    vers_out[5] = '.';
    vers_out[6] = vers_in[3];
    vers_out[7] = vers_in[4];
    vers_out[8] = '\0';

}

void KeyCheckStartStop () {
	uint16_t pu, pu1_source, pu2_source, loc_rem, button_stop_mode;
    loc_rem = uIntDataRequest(LOC_REM_ADR);

//  в этой функции есть ошибка. Если пост управления выбирается по
//  сигналу на дискретном входе, то команда старт/стоп не сбрасывается
//  ни при какой конфигурации команд старт/стоп.

    if (loc_rem == 0) {
        pu = uIntDataRequest(PU1_PU2_SELECT_ADR);
        button_stop_mode = uIntDataRequest(BUTTON_STOP_MODE_ADR);
		switch (pu) {
		case 0:
	        pu1_source = uIntDataRequest(PU1_SOURCE_ADR);
			if (pu1_source != 8) // 10.1 EXT1_Start_Stop != keypad
			{
				KeyChekSem(xButtonRunSemaphore); // сброс нажатия кнопки RUN

				if (button_stop_mode == 0) // 17.5 Stop_button_mode = Local
				{
					KeyChekSem(xButtonStopSemaphore); // сброс нажатия кнопки STOP
				}
			}
			break;
		case 1:
	        pu2_source = uIntDataRequest(PU2_SOURCE_ADR);
			if (pu2_source != 8) // 10.2 EXT2_Start_Stop != keypad
			{
				KeyChekSem(xButtonRunSemaphore); // сброс нажатия кнопки RUN

				if (button_stop_mode == 0) // 17.5 Stop_button_mode = Local
				{
					KeyChekSem(xButtonStopSemaphore); // сброс нажатия кнопки STOP
				}
			}
			break;
		default:
			break;
		}
    }
}

uint16_t KeypadRefWrite(uint16_t dataAdress, int dataValue)
{
	uint8_t wbuf[2];
	uint16_t tmp;
	wbuf[0] = dataValue & 0x00FF;
	tmp = dataValue & 0xFF00;
	tmp = tmp >> 8;
	wbuf[1] = tmp;
	uint16_t Adress = 100 * EEPROM_PAGE_SIZE + dataAdress;

//	HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADR, Adress, I2C_MEMADD_SIZE_16BIT, &wbuf[0], 2, EEPROM_TIME_OUT);

//	HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, Adress, I2C_MEMADD_SIZE_16BIT, &wbuf[0], 2, EEPROM_TIME_OUT);

/*
	tmp = wbuf[1];
	tmp = tmp << 8;
	tmp = tmp & 0xFF00;
	tmp = tmp + wbuf[0];
	IntDataWrite(0x0C05, tmp);
	IntDataWrite(0x0C05, dataValue);
*/
	vTaskDelay(500);
	return (dataValue);
}

uint16_t KeypadRefRead(uint16_t dataAdress)
{
	uint8_t wbuf[2];
	uint16_t tmp;

	uint16_t Adress = 100 * EEPROM_PAGE_SIZE + dataAdress;

//	HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, Adress, I2C_MEMADD_SIZE_16BIT, &wbuf[0], 2, EEPROM_TIME_OUT);

	tmp = wbuf[1];
	tmp = tmp << 8;
	tmp = tmp & 0xFF00;
	tmp = tmp + wbuf[0];
	vTaskDelay(50);
	return (tmp);
}



void DisplayStatic(void) {
    uint16_t groupNumber = Groups_list->Value - 1;
    uint16_t paramNumber = Group[Groups_list->Value - 1]->Value - 1;
    uint16_t paramNumberPointer = Group[Groups_list->Value - 1]->Pointer - 1;
    uint16_t fastParamNumber = Fast_list->Value - 1;
    uint16_t fastParamNumberPointer = Fast_list->Pointer - 1;
    uint16_t eventNumberPointer = Archive_list->Pointer - 1;
    uint16_t monitorNumberPointer = Monitor_list->Pointer - 1;

    static group_t groupLocal;

    char dataAscii[12], vfd_vers[12];

    StatusMonitor();// верхняя строка до черты и светодиоды
    if (!editMode)
    if (ValUp(&Display, xButtonFuncSemaphore)) MenuInit(&Display);

    KeyCheckStartStop();// Сбрасывает кнопки Start-Stop в режиме Remote при выполнении некоторых условий
    CW_Command(START_STOP, 1, &controlWord, xButtonRunSemaphore);
    CW_Command(START_STOP, 0, &controlWord, xButtonStopSemaphore);
    CW_Command(ALARM_RES, 1, &controlWord, xButtonResetAlarmsSemaphore);
//    CW_Command(DIRECT, 0, &controlWord, xButtonUpSemaphore);
//    CW_Command(DIRECT, 1, &controlWord, xButtonDownSemaphore);

    switch (Mode->Value) {
    case MODE_MONITOR:// Главный экран с Мониторингами
        KeyChekSem(xButtonEnterSemaphore);// сброс нажатия Enter
        CW_Command(DIRECT, 0, &controlWord, xButtonUpSemaphore);
        CW_Command(DIRECT, 1, &controlWord, xButtonDownSemaphore);
        for (int i = 0; i < MONITORS_NUMBER; ++i) MonitorView(i);
        break;

    case MODE_CONTROL:// Второй экран с частотой


        if (IntDataRequest(SEL_PRC_RPM_ADR) == 0) NumberEdit(&keyRef_hz, 30, 4);// %
    		else NumberEdit(&keyRef_prc, 30, 4);// rpm

        CW_Command(DIRECT, 0, &controlWord, xButtonUpSemaphore);
        CW_Command(DIRECT, 1, &controlWord, xButtonDownSemaphore);

        break;

    case MODE_PREFERENS:
        switch (Main_menu->Value) {
        case 0:
            NavigateMenu(Mode, Main_menu, Menu[0], MAX_STRING, NAVIG_DISP_STRUCT, true);
            break;

        case 1:// ПАРАМЕТРЫ
            switch (Groups_list->Value) {
            case 0:
                NavigateMenu(Main_menu, Groups_list, groups[0].name, sizeof(groups[0]), NAVIG_DISP_STRUCT, true);//отображать все группы кроме последней
                break;

            default:
                switch (Group[groupNumber]->Value) {
                case 0:// нахожусь в списке параметров и стою на параметре видя его значение
                    groupLocal = groups[groupNumber];

                    NavigateParam(Groups_list, Group[groupNumber], &paramNumberPointer, &groupLocal);

                    ST7565_drawstring(DISP_LEFT_BOUND, 4, groupLocal.param[paramNumberPointer].name);
                    ST7565_drawstring(DISP_LEFT_BOUND, 2, groupLocal.name);
                    break;

                default:// зашёл внутрь параметра
                    ParametrEdit(&(groupLocal.param[paramNumber]), Group[groupNumber]);
                    break;

                }
                break;

            }
            break;

        case 2:// АРХИВ СОБЫТИЙ
            switch (Archive_list->Value) {
            case 0:
                groupLocal = ArchiveGroup;
                NavigateParam(Main_menu, Archive_list, &eventNumberPointer, &groupLocal);
                ST7565_drawstring(DISP_LEFT_BOUND, 2, &groupLocal.param[eventNumberPointer].name[0]);
                break;

            default:
                ArchiveView(&groupLocal.param[eventNumberPointer], Archive_list);
                break;
            }
            break;

        case 3:// БЫСТРЫЙ СТАРТ
            switch (Fast_list->Value) {
            case 0:
                groupLocal = fastParams;
                NavigateParam(Main_menu, Fast_list, &fastParamNumberPointer, &groupLocal);
                ST7565_drawstring(DISP_LEFT_BOUND, 4, &groupLocal.param[fastParamNumberPointer].name[3]);
                ST7565_drawstring(DISP_LEFT_BOUND, 2, groupLocal.name);
                break;

            default:
                ParametrEdit(&(groupLocal.param[fastParamNumber]), Fast_list);
                break;

            }
            break;

        case 4:// ФУНКЦИИ КОПИРОВАНИЯ
            switch (Memory_list->Value) {
            case 0:
                NavigateMenu(Main_menu, Memory_list, Memory[0], MAX_STRING, NAVIG_DISP_STRUCT, true);
                break;

            case 1:// ЗАГРУЗИТЬ В XD500
                error = EE_Main(upload);
                break;

            case 2:// КОПИРОВАТЬ В ПУЛЬТ
                EE_Main(download);
                break;

            case 3:// ЗАВОДСКИЕ НАСТРОЙКИ
                ValReset(Main_menu, xButtonResetSemaphore);
                controlWord.bits.rstToFactory = true;
                IntDataWrite(RESTOR_DEF, 1);

                //настройки Мониторинга
                IntDataWrite(0xF000, 7);//CURRENT
                IntDataWrite(0xF001, 5);// DC BUS VOLT
                IntDataWrite(0xF002, 11);// TEMP IGBT
                MenuInit(&Display);
                break;

            default:
                break;
            }
            break;

        case 5:// ВЕРСИЯ ПРОГРАММЫ
            ST7565_drawstring(DISP_LEFT_BOUND+FONT_GAP, 2, "XDRC500:");
            ST7565_drawstring(DISP_LEFT_BOUND+FONT_GAP*10, 2, VERSION);

            my_itoa(IntDataRequest(VERSION_VFD_ADR), dataAscii);
            VfdVerrsionConvert(dataAscii, vfd_vers);
            ST7565_drawstring(DISP_LEFT_BOUND+FONT_GAP, 4, "XDCU500:");
            ST7565_drawstring(DISP_LEFT_BOUND + FONT_GAP*10, 4,  vfd_vers);

            ValReset(Main_menu, xButtonResetSemaphore);
            break;

        case 6:// МОНИТОРИНГ ПАРАМЕТРЫ
            switch (Monitor_list->Value) {
			case 0:
				groupLocal = monitorParams;
				NavigateParam(Main_menu, Monitor_list, &monitorNumberPointer, &groupLocal);
				ST7565_drawstring(DISP_LEFT_BOUND, 4, &groupLocal.param[monitorNumberPointer].name[0]);
				ST7565_drawstring(DISP_LEFT_BOUND, 2, groupLocal.name);
				break;

			default:
				ParametrEdit(&(groupLocal.param[monitorNumberPointer]), Monitor_list);
				break;
			}
            ValReset(Main_menu, xButtonResetSemaphore);
            break;

        case 7:// ПОКАЗАНИЯ ТАЙМЕРОВ
            ST7565_drawstring(0, 2, "МОТ:");
            TimeRequest (TIME_MOT_ADR, &formatTime);
            TimeView (&formatTime_R, 2,  FONT_GAP*4);

            ST7565_drawstring(0, 4, "ОБЩ:");
            TimeRequest (TIME_ALL_ADR, &formatTime);
            TimeView (&formatTime_A, 4,  FONT_GAP*4);

            ST7565_drawstring(0, 6, "ВКЛ:");
            TimeRequest (TIME_PWR_ADR, &formatTime);
            TimeView (&formatTime_P, 6,  FONT_GAP*4);

            ValReset(Main_menu, xButtonResetSemaphore);
            break;

        default:
            InDevelop(&Display, xButtonResetSemaphore);
            break;
        }
        break;

    default:
        InDevelop(&Display, xButtonResetSemaphore);
        break;

    }
}

void RefInit()
{
	uint16_t dataAdress;
	uint16_t dataValue;
	uint16_t keypadMode;

	param_t * parametr;

//	osDelay(10000);

//	MX_USB_HOST_Init();
//	 while (Appli_state != APPLICATION_READY) vTaskDelay(1);// жду готовности USB

	dataAdress = KEY_REF_MODE_ADR;
	keypadMode = USBRequest(0 /*0 - Read */, dataAdress, 0);

	if (keypadMode == true) {dataValue = KeypadRefRead(5);}         // Читаем задание из EEPROM (В Герцах)
	else {dataValue = 0;}

	parametr = &keyRef_hz;											// Записываем задание в параметр G64P03 (в Герцах)
	parametr->value = dataValue;

	parametr = &keyRef_prc;											// Записываем задание в параметр G64P03_PRC (в Процентах)
	parametr->value = 2.0*dataValue;


// Отправляю заданное значение скорости (в Герцах) по USB  (в XD500-V)
	dataAdress = FREQ_REF_ADR;
	USBRequest(1 /*1 - Write */, dataAdress, dataValue);
}
