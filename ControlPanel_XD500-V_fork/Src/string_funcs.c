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

