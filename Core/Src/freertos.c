/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tasks.h"
#include "display.h"
#include "colors.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for initTask */
osThreadId_t initTaskHandle;
const osThreadAttr_t initTask_attributes = {
  .name = "initTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .priority = (osPriority_t) osPriorityBelowNormal2,
  .stack_size = 128 * 4
};
/* Definitions for relSCTask */
osThreadId_t relSCTaskHandle;
const osThreadAttr_t relSCTask_attributes = {
  .name = "relSCTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for fftTask */
osThreadId_t fftTaskHandle;
const osThreadAttr_t fftTask_attributes = {
  .name = "fftTask",
  .priority = (osPriority_t) osPriorityBelowNormal1,
  .stack_size = 128 * 4
};
/* Definitions for relTask */
osThreadId_t relTaskHandle;
const osThreadAttr_t relTask_attributes = {
  .name = "relTask",
  .priority = (osPriority_t) osPriorityNormal2,
  .stack_size = 128 * 4
};
/* Definitions for btnsTask */
osThreadId_t btnsTaskHandle;
const osThreadAttr_t btnsTask_attributes = {
  .name = "btnsTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 128 * 4
};
/* Definitions for displTask */
osThreadId_t displTaskHandle;
const osThreadAttr_t displTask_attributes = {
  .name = "displTask",
  .priority = (osPriority_t) osPriorityBelowNormal,
  .stack_size = 256 * 4
};
/* Definitions for rtcTask */
osThreadId_t rtcTaskHandle;
const osThreadAttr_t rtcTask_attributes = {
  .name = "rtcTask",
  .priority = (osPriority_t) osPriorityLow1,
  .stack_size = 128 * 4
};
/* Definitions for flashTask */
osThreadId_t flashTaskHandle;
const osThreadAttr_t flashTask_attributes = {
  .name = "flashTask",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitTask(void *argument);
extern void StartAdcCaptureTask(void *argument);
extern void StartRelayStateControlTask(void *argument);
extern void StartFftTask(void *argument);
extern void StartRelayTask(void *argument);
extern void StartButtonsTask(void *argument);
extern void StartDisplayTask(void *argument);
extern void StartRtcTask(void *argument);
extern void StartFlashTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
  static TaskHandle_t lastOfTask = NULL;
  static wchar_t wcharBuf[configMAX_TASK_NAME_LEN];
  static uint8_t strLength;
  static uint8_t lastPos = 0;
  if (lastOfTask != xTask) {
    lastOfTask = xTask;
    strLength = mbstowcs(wcharBuf, (const char *)pcTaskName, configMAX_TASK_NAME_LEN);
    if (lastPos >= DISPLAY_WIDTH)
      lastPos = 0;
    SetCSLow();
    lastPos += DrawUTF8String(lastPos, 86, wcharBuf, strLength, 1, 0, redColor, &arialBlack_8ptFontInfo, TextAlignLeft) + 1;
    SetCSHigh();
  }
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of initTask */
  initTaskHandle = osThreadNew(StartInitTask, NULL, &initTask_attributes);

  /* creation of adcTask */
  adcTaskHandle = osThreadNew(StartAdcCaptureTask, NULL, &adcTask_attributes);

  /* creation of relSCTask */
  relSCTaskHandle = osThreadNew(StartRelayStateControlTask, NULL, &relSCTask_attributes);

  /* creation of fftTask */
  fftTaskHandle = osThreadNew(StartFftTask, NULL, &fftTask_attributes);

  /* creation of relTask */
  relTaskHandle = osThreadNew(StartRelayTask, NULL, &relTask_attributes);

  /* creation of btnsTask */
  btnsTaskHandle = osThreadNew(StartButtonsTask, NULL, &btnsTask_attributes);

  /* creation of displTask */
  displTaskHandle = osThreadNew(StartDisplayTask, NULL, &displTask_attributes);

  /* creation of rtcTask */
  rtcTaskHandle = osThreadNew(StartRtcTask, NULL, &rtcTask_attributes);

  /* creation of flashTask */
  flashTaskHandle = osThreadNew(StartFlashTask, NULL, &flashTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartInitTask */
/**
  * @brief  Function implementing the initTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartInitTask */
void StartInitTask(void *argument)
{
  /* USER CODE BEGIN StartInitTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
    vTaskDelete(NULL);
  }
  /* USER CODE END StartInitTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
