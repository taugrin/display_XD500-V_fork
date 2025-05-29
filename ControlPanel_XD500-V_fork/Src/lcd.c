#include "lcd.h"
#include "dwt_stm32_delay.h"


//uint8_t data;
//uint8_t cmd;

uint16_t GpioDataPins = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;




void lcdDataWriteOld(uint8_t data)
{
    //RESET DATA BUS
    HAL_GPIO_WritePin(GPIOC, GpioDataPins, GPIO_PIN_RESET);

    //A0 DOWN
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    //DWT_Delay_us(1);

    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //WR UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

    //A0 UP
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    //DWT_Delay_us(1);

    //CS DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    //WR DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

    //DWT_Delay_us(1);

    //CMD BUS SET
    HAL_GPIO_WritePin(GPIOC, GpioDataPins & data, GPIO_PIN_SET);
    //DWT_Delay_us(1);

    //WR UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //DWT_Delay_us(1);

    //A0 DOWN
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    //DWT_Delay_us(1);
}

void lcdDataWrite(uint8_t data)
{
	/*
    // —брос шины данных
    GPIOC->BSRR = GpioDataPins << 16;

    GPIOC->BSRR = GPIO_PIN_8 << 16;  // A0 низкий
    GPIOB->BSRR = GPIO_PIN_12;       // CS высокий
    GPIOB->BSRR = GPIO_PIN_10;       // WR высокий

    GPIOC->BSRR = GPIO_PIN_8;        // A0 высокий
    GPIOB->BSRR = GPIO_PIN_12 << 16; // CS низкий
    GPIOB->BSRR = GPIO_PIN_10 << 16; // WR низкий

    // ”становка данных
    GPIOC->BSRR = data & 0xFF;

    GPIOB->BSRR = GPIO_PIN_10;       // WR высокий
    GPIOB->BSRR = GPIO_PIN_12;       // CS высокий
    GPIOC->BSRR = GPIO_PIN_8 << 16;  // A0 низкий
	*/

    // —брос шины данных и установка A0 в низкий уровень
    GPIOC->BSRR = (GpioDataPins | GPIO_PIN_8) << 16;

    // ”становка CS и WR в высокий уровень (если они еще не установлены)
    GPIOB->BSRR = GPIO_PIN_12 | GPIO_PIN_10;

    // ”становка A0 в высокий и CS в низкий
    GPIOC->BSRR = GPIO_PIN_8;
    GPIOB->BSRR = GPIO_PIN_12 << 16;

    // ”становка данных и формирование строба записи
    GPIOC->BSRR = data & 0xFF;
    GPIOB->BSRR = GPIO_PIN_10 << 16; // WR низкий
    GPIOB->BSRR = GPIO_PIN_10;       // WR высокий

    // «авершение цикла записи
    GPIOB->BSRR = GPIO_PIN_12;       // CS высокий
    GPIOC->BSRR = GPIO_PIN_8 << 16;  // A0 низкий
}

uint8_t  lcdReadStatus()
{
    uint16_t result;
    GPIO_InitTypeDef GPIO_InitStruct;

    //INIT PARALLEL BUS FOR READ
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //A0 UP
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    DWT_Delay_us(5);
    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //RD UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    DWT_Delay_us(5);

    //A0 DOWN
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    DWT_Delay_us(5);
    //CS DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    DWT_Delay_us(5);
    //RD DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    DWT_Delay_us(5);

    result = GPIOC->IDR;


    //RD UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    DWT_Delay_us(5);
    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    DWT_Delay_us(5);



    //INIT PARALLEL BUS FOR WRITE
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    return result;
}

void lcdCmd(uint8_t cmd)
{
    //RESET DATA BUS
    HAL_GPIO_WritePin(GPIOC, GpioDataPins, GPIO_PIN_RESET);


    //A0 UP
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    //DWT_Delay_us(1);
    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //WR UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

    //A0 DOWN
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    //DWT_Delay_us(1);

    //CS DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    //WR DOWN
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

    //DWT_Delay_us(1);

    //CMD BUS SET
    HAL_GPIO_WritePin(GPIOC, GpioDataPins & cmd, GPIO_PIN_SET);
    //DWT_Delay_us(1);

    //WR UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    //CS UP
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    //DWT_Delay_us(1);

    //A0 UP
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    //DWT_Delay_us(1);
}

void lcdInit()
{

    //RESET LCD (Activate reset)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_Delay(500);

    //Prepare GPIO Initial State
    //PS BIT
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    //DC (A0) BIT
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    //WR BIT
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    //RD BIT
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    //OLED LED
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    //CS BIT (Deselect controller)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    //SET LCD
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_Delay(5);


    lcdCmd(cADC_NORMAL);            // Normal segment order
    HAL_Delay(2);

    lcdCmd(cCOM_NORMAL);            // Normal common order
    HAL_Delay(2);

    lcdCmd(cDISP_NORMAL);           // Normal common order
    HAL_Delay(2);

    lcdCmd(cBIAS_9);                // Set 1/9 bias
    HAL_Delay(2);

    lcdCmd(cPOWER_CONTROL | 4);
    HAL_Delay(50);

    lcdCmd(cPOWER_CONTROL | 6);
    HAL_Delay(50);

    lcdCmd(cPOWER_CONTROL | 7);     // Power boost, regulator, and follower all on
    HAL_Delay(50);

    lcdCmd(cBOOSTRATIO);            // Enter boost ratio set mode, and then...
    lcdCmd(0);                      //   set boost ratio to 2x/3x/4x
    HAL_Delay(5);

//
//    lcdCmd(cRESISTOR_RATIO | 7);
//    HAL_Delay(5);
//

    lcdCmd(cVOLUME);                // Volume register set (LCD voltage, Vo). Next byte is...
    lcdCmd(NORM_VOLUME);          //    the "volume", 0..63 (0x00..0x3f).
    HAL_Delay(55);

    lcdCmd(cDISP_START_LINE);            // Reset startline
    HAL_Delay(2);

    lcdCmd(cDISPLAY_ON);

    HAL_Delay(2);
}

void lcdByteAllpx(uint8_t byte)
{
  uint8_t p, c;

  for(p = 0; p < 8; p++)
  {

      lcdCmd(cPAGE | p);
    for(c = 0; c < 128; c++)
    {
        lcdCmd(cCOL_LS | (c & 0xf));
        lcdCmd(cCOL_MS | ((c >> 4) & 0xf));
        lcdDataWrite(byte);
    }
  }
}


