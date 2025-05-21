/*
 * usb_user.h
 *
 *  Created on: 21 мая 2018 г.
 *      Author: kislyakov
 */

#ifndef USB_USER_H_
#define USB_USER_H_

#include "usbh_def.h"

#define RXBUFLENGTH 48	//16 // чтобы вычитывать хвосты.
//64 - максимальный прием за один пакет. Значительно больше длинны пакета, чтобы вычитывать данные,
//и буфер на отправку в Блоке управления не засирался в случае каких-то сбоев USB

#define TXBUFLENGTH 6



void MX_USB_HOST_Process();
USBH_StatusTypeDef USBDataRequest(void);
USBH_StatusTypeDef USBDataRead(void);

#endif /* USB_USER_H_ */
