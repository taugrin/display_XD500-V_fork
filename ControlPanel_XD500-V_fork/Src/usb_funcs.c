//
// Included Files
//
#include "usb_funcs.h"
#include "usb_host.h"
#include "usbh_core.h"

extern USBH_HandleTypeDef hUsbHostFS;  // Дескриптор USB Host (объявлен в usb_host.c)

//
// Function Prototypes
//
uint8_t get_crc8(uint8_t *addr, uint8_t len);

void USBH_Restart(void);

//
// Global Variables
//


//
// Local Variables
//

//--------------------------------------------------------------------
/*
* get_crc8 - определения чек-суммы по алгоритму crc-8
*/
uint8_t get_crc8(uint8_t *addr, uint8_t len)
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
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* UsbReadData - чтение нескольких параметров (регистров) по usb
*/
void UsbReadData(uint16_t ParamAdr, uint16_t ParamCnt, uint16_t *ParamData)
{
	uint16_t noResponseCount = 0;
	uint16_t i, ParamCntTmp, DataByteCnt, n;
	while (1)
	{
		tx_buffer[0] = 0x03; // CMD
		tx_buffer[1] = ParamAdr; // ParamAdr Lo
		tx_buffer[2] = ParamAdr >> 8; //ParamAdr Hi
		if (ParamCnt > 16) {ParamCntTmp = 16;}
		else {ParamCntTmp = ParamCnt;}
		DataByteCnt = ParamCnt*2;
		tx_buffer[3] = ParamCntTmp;
		tx_buffer[4] = 0x00;
		tx_buffer[5] = get_crc8(tx_buffer, 5); // добавляем к посылке чексумму, алгоритм crc8

		rx_buffer[0] = 0xFE;// порчу буфер принятых данных, чтобы если в следующий раз ЧРП не ответит, не обработь случайно предыдущие данные

		noResponseCount++;//заранее увеличиваю
		if (USBDataRequest() == USBH_OK)
		{
			for (i = 0; i < USB_ANSWER_DELAY; i++)
			{
				if (USBTransmitted) break;
				vTaskDelay(1);
			}
			if (USBTransmitted)
			{
				USBDataRead();// вычитываю больше чем надо, на случай, если в приеме остались какие-то хвосты
				for (i = 0; i < USB_ANSWER_DELAY; i++) {// бывает отказ чтения, поэтому запрашиваю чтение много раз
					if (USBReceived) break;// бывает сразу при USBDataRequest происходит сразу и передача и прием, и нет неоходимости ещё раз запрашивать прием
					vTaskDelay(1);
				}

				if (USBReceived &&
					(rx_buffer[1] == tx_buffer[1]) &&
					(rx_buffer[2] == tx_buffer[2]) &&
					(get_crc8(rx_buffer, (DataByteCnt + 4))) == (rx_buffer[(DataByteCnt + 4)])) {// проверяем чексумму принятого сообщения, если не совпадает - шлем запрос заново

					for (n = 0; n < ParamCntTmp; n++)
					{
						ParamData[n] = (rx_buffer[5+n*2] << (8)) | (rx_buffer[4+n*2]);
					}
					return;
				}
			}
		}

		if (noResponseCount > USB_MAX_REQUEST)
		{
			/*ST7565_clear();
			ST7565_drawstring(6, 4, "USB long no response");
			ST7565_drawstring(6, 5, "RESET!");
			ST7565_display();
			vTaskDelay(1000);*/
			//NVIC_SystemReset();
			noResponseCount = 0;
			USBH_Restart();  // Перезапуск USB Host
			//return;
		}

	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* UsbWriteReg - запись одного параметра (регистра) по usb
*/
void UsbWriteReg(uint16_t ParamAdr, uint16_t ParamData)
{
	uint16_t noResponseCount = 0;
	uint16_t i;
	while (1)
	{
		tx_buffer[0] = 0x01; // CMD
		tx_buffer[1] = ParamAdr; // ParamAdr Lo
		tx_buffer[2] = ParamAdr >> 8; //ParamAdr Hi
		tx_buffer[3] = ParamData; // ParamData Lo
		tx_buffer[4] = ParamData >> 8; //ParamData Hi
		tx_buffer[5] = get_crc8(tx_buffer, 5); // добавляем к посылке чексумму, алгоритм crc8

		rx_buffer[0] = 0xFE;// порчу буфер принятых данных, чтобы если в следующий раз ЧРП не ответит, не обработь случайно предыдущие данные

		noResponseCount++;//заранее увеличиваю
		if (USBDataRequest() == USBH_OK)
		{
			for (i = 0; i < USB_ANSWER_DELAY; i++)
			{
				if (USBTransmitted) break;
				vTaskDelay(1);
			}
			if (USBTransmitted)
			{
				USBDataRead();// вычитываю больше чем надо, на случай, если в приеме остались какие-то хвосты
				for (i = 0; i < USB_ANSWER_DELAY; i++) {// бывает отказ чтения, поэтому запрашиваю чтение много раз
					if (USBReceived) break;// бывает сразу при USBDataRequest происходит сразу и передача и прием, и нет неоходимости ещё раз запрашивать прием
					vTaskDelay(1);
				}

				if (USBReceived &&
					(rx_buffer[1] == tx_buffer[1]) &&
					(rx_buffer[2] == tx_buffer[2]) &&
					(get_crc8(rx_buffer, 5)) == (rx_buffer[5])) {// проверяем чексумму принятого сообщения, если не совпадает - шлем запрос заново
					return;
				}
			}
		}

		if (noResponseCount > USB_MAX_REQUEST)
		{
			/*ST7565_clear();
			ST7565_drawstring(6, 4, "USB long no response");
			ST7565_drawstring(6, 5, "RESET!");
			ST7565_display();
			vTaskDelay(1000);*/
			//NVIC_SystemReset();
			noResponseCount = 0;
			USBH_Restart();  // Перезапуск USB Host
			//return;
		}

	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
 * USBH_Restart - полный перезапуск USB Host
 */
void USBH_Restart(void)
{
    // 1. Остановка и деинициализация USB Host
    USBH_Stop(&hUsbHostFS);
    USBH_DeInit(&hUsbHostFS);

    // 2. Задержка для сброса питания (если нужно)
    //HAL_Delay(200);  // 200 мс для гарантированного детекта отключения
    vTaskDelay(50);

    // 3. Повторная инициализация
    MX_USB_HOST_Init();  // Функция, сгенерированная CubeMX

    // 4. Запуск USB Host
    USBH_Start(&hUsbHostFS);
}
/*void USBH_Restart(void) {
    // 1. Остановка USB Host
    USBH_Stop(&hUsbHostFS);

    // 2. Принудительный сброс USB периферии (критично для GD32!)
    __HAL_RCC_USB_OTG_FS_FORCE_RESET();
    HAL_Delay(50);
    __HAL_RCC_USB_OTG_FS_RELEASE_RESET();
    HAL_Delay(10);

    // 3. Деинициализация
    USBH_DeInit(&hUsbHostFS);

    // 4. Очистка всех URBs (если есть доступ)
    //for (int i = 0; i < USBH_MAX_PIPES; i++) {
    //    if (hHCD->hc[i].state != HC_IDLE) {
    //        hHCD->hc[i].state = HC_HALTED;
    //    }
    //}

    // 5. Задержка для сброса линии USB (GD32 требует больше времени!)
    HAL_Delay(300);

    // 6. Повторная инициализация
    MX_USB_HOST_Init();
    USBH_Start(&hUsbHostFS);

    // 7. Проверка успешности
    if (hUsbHostFS.gState == USBH_FAIL) {
        Error_Handler();
    }
}*/
//--------------------------------------------------------------------
