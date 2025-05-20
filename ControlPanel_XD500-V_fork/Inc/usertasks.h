#include "cmsis_os.h"

#define TIMER1_PERIOD 4


SemaphoreHandle_t xButtonUpSemaphore;
SemaphoreHandle_t xButtonDownSemaphore;
SemaphoreHandle_t xButtonEnterSemaphore;
SemaphoreHandle_t xButtonFuncSemaphore;
SemaphoreHandle_t xButtonResetSemaphore;
SemaphoreHandle_t xButtonRunSemaphore;
SemaphoreHandle_t xButtonStopSemaphore;
SemaphoreHandle_t xButtonResetAlarmsSemaphore;




void vTaskUSBCDC(void *pvParameters);

void vTaskLED1(void *pvParameters);

void vTaskDisplay (void const * argument);

void vTaskKeysCheck (void *pvParameters);

