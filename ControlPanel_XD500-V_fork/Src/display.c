//
// Included Files
//
#include "display.h"

//
// Function Prototypes
//
int8_t CheckKeySem(SemaphoreHandle_t semaphore);

//
// Global Variables
//


//
// Local Variables
//


//--------------------------------------------------------------------
/*
* DisplayStatic - функция обработки связи и отображения данных на экране
*/
void DisplayStatic(void)
{
	if (CheckKeySem(xButtonFuncSemaphore))
	{
		NextScreen();
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
* CheckKey - возвращает значение бинарного семафора кнопки
*/
int8_t CheckKeySem(SemaphoreHandle_t semaphore)
{
    int8_t res;

    if (semaphore != NULL)
	{
        if (xSemaphoreTake(semaphore, (portTickType)0) == pdTRUE) {res = 1;}
        else {res = 0;}
	}
    else {res = -1;}

    return res;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
/*
*
*/

//--------------------------------------------------------------------

