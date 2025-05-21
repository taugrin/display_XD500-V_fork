#ifndef USB_FUNCS_H_
#define USB_FUNCS_H_

//
// Included Files
//
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

#include "usb_user.h"


//
// Defines
//
#define USB_ANSWER_DELAY 30

//
// TypeDefs
//


//
// Variables from other files
//
extern uint8_t rx_buffer[RXBUFLENGTH];
extern uint8_t tx_buffer[TXBUFLENGTH];

extern uint16_t rxMessageCount;
extern uint16_t txMessageCount;

extern uint16_t USBTransmitted;
extern uint16_t USBReceived;

//
// Variables for other files
//


//
// Functions for other files
//

void UsbReadData(uint16_t ParamAdr, uint16_t ParamCnt, uint16_t *ParamData);



#endif /* USB_FUNCS_H_ */
