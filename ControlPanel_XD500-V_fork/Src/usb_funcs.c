//
// Included Files
//
#include "usb_funcs.h"

//
// Function Prototypes
//
uint8_t get_crc8(uint8_t *addr, uint8_t len);

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
			ST7565_drawstring(10, 5, "USB 50 no resp, RESET!");
			ST7565_display();
			vTaskDelay(500);
			NVIC_SystemReset();
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
			ST7565_drawstring(10, 5, "USB 50 no resp, RESET!");
			ST7565_display();
			vTaskDelay(500);
			NVIC_SystemReset();
		}

	}
}
//--------------------------------------------------------------------
