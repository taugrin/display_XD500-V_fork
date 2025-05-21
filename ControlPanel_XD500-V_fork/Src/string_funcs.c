//
// Included Files
//
#include "string_funcs.h"

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
* my_itoa - int32_t to string (null terminated). Buf size need 12 bytes max
*/
void my_itoa(int32_t value, char *str)
{
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
//--------------------------------------------------------------------

/*
 * ѕреобразует число в 16-ричную строку с фиксированной длиной
 * @param value - число дл€ преобразовани€
 * @param str - указатель на буфер вывода (должен быть достаточного размера)
 * @param digits - количество символов в выходной строке
 * @return ”казатель на строку (str)
 */
char* uint16_to_hex_str(uint16_t value, char* str, uint8_t digits)
{
    const char hex_chars[] = "0123456789ABCDEF";

    // «аполн€ем строку справа налево
    for(uint8_t i = 0; i < digits; i++) {
        str[digits - 1 - i] = hex_chars[(value >> (4 * i)) & 0xF];
    }

    str[digits] = '\0'; // ƒобавл€ем нуль-терминатор
    return str;
}
//--------------------------------------------------------------------
