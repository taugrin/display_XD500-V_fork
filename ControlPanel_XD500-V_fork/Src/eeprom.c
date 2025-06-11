//
// Included Files
//
#include "eeprom.h"

//
// Function Prototypes
//


//
// Global Variables
//


//
// Local Variables
//


//--------------------------------------------------------------------
/*
* readMonitorSel - чтение из eeprom выбора переменной для мониторинга
* @param num - номер мониторинга от 0 до 2
*/
uint8_t readMonitorSel(uint8_t num)
{
	uint16_t dataAdress;
	uint8_t data;
	HAL_StatusTypeDef readRes;

	if (num <= 2) {dataAdress = MONITOR_SEL_BEGIN_ADR + num;}
	else {dataAdress = MONITOR_SEL_BEGIN_ADR + 2;}

	readRes = HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, dataAdress, I2C_MEMADD_SIZE_16BIT, &data, 1, EEPROM_TIME_OUT);

	if (readRes == HAL_OK)
	{
		return data;
	}
	else
	{
		return 0xFF;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* writeMonitorSel - запись в eeprom выбранной переменной для мониторинга
* @param num - номер мониторинга от 0 до 2
* @param data - номер переменной мониторинга
*/
bool writeMonitorSel(uint8_t num, uint8_t *pData)
{
	uint16_t dataAdress;
	HAL_StatusTypeDef writeRes;

	if (num <= 2) {dataAdress = MONITOR_SEL_BEGIN_ADR + num;}
	else {dataAdress = MONITOR_SEL_BEGIN_ADR + 2;}

	writeRes = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADR, dataAdress, I2C_MEMADD_SIZE_16BIT, pData, 1, EEPROM_TIME_OUT);

	if (writeRes == HAL_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* readDisplayRef - чтение из eeprom задания задания пульта
*/
uint16_t readDisplayRef(void)
{
	uint16_t dataAdress, result;
	uint8_t data[2];
	HAL_StatusTypeDef readRes;

	dataAdress = DISPLAY_REF_ADR;

	readRes = HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADR, dataAdress, I2C_MEMADD_SIZE_16BIT, data, 2, EEPROM_TIME_OUT);

	if (readRes == HAL_OK)
	{
		result = (data[1] << 8) | (data[0]);
		return result;
	}
	else
	{
		result = 0xFFFF;
		return result;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* writeDisplayRef - запись в eeprom задания пульта
*/
bool writeDisplayRef(uint16_t data)
{
	uint16_t dataAdress;
	HAL_StatusTypeDef writeRes;
	uint8_t pData[2];

	dataAdress = DISPLAY_REF_ADR;

	pData[0] = data & 0xFF;
	pData[1] = (data >> 8) & 0xFF;

	writeRes = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADR, dataAdress, I2C_MEMADD_SIZE_16BIT, pData, 2, EEPROM_TIME_OUT);

	if (writeRes == HAL_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------

