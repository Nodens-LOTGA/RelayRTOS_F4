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
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = { .name = "adcTask", .priority = osPriorityBelowNormal2, .stack_size = 256 };
osThreadId_t relayStateControlTaskHandle;
const osThreadAttr_t relayStateControlTask_attributes = { .name = "relSCTask", .priority = osPriorityNormal2, .stack_size = 400 };
osThreadId_t fftTaskHandle;
const osThreadAttr_t fftTask_attributes = { .name = "fftTask", .priority = osPriorityBelowNormal1, .stack_size = 256 };
osThreadId_t relayTaskHandle;
const osThreadAttr_t relayTask_attributes = { .name = "relTask", .priority = osPriorityNormal, .stack_size = 304 };
osThreadId_t buttonsTaskHandle;
const osThreadAttr_t buttonsTask_attributes = { .name = "btnsTask", .priority = osPriorityBelowNormal, .stack_size = 288 };
osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = { .name = "displTask", .priority = osPriorityBelowNormal, .stack_size = 800 };
osThreadId_t rtcTaskHandle;
const osThreadAttr_t rtcTask_attributes = { .name = "rtcTask", .priority = osPriorityLow, .stack_size = 256 };
osThreadId_t flashTaskHandle;
const osThreadAttr_t flashTask_attributes = { .name = "flashTask", .priority = osPriorityNormal1, .stack_size = 384 };
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN 4 */

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

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  adcTaskHandle = osThreadNew(StartAdcCaptureTask, NULL, &adcTask_attributes);
  relayStateControlTaskHandle = osThreadNew(StartRelayStateControlTask, NULL, &relayStateControlTask_attributes);
  fftTaskHandle = osThreadNew(StartFftTask, NULL, &fftTask_attributes);
  relayTaskHandle = osThreadNew(StartRelayTask, NULL, &relayTask_attributes);
  buttonsTaskHandle = osThreadNew(StartButtonsTask, NULL, &buttonsTask_attributes);
  displayTaskHandle = osThreadNew(StartDisplayTask, NULL, &displayTask_attributes);
  rtcTaskHandle = osThreadNew(StartRtcTask, NULL, &rtcTask_attributes);
  flashTaskHandle = osThreadNew(StartFlashTask, NULL, &flashTask_attributes);
  /* USER CODE END RTOS_THREADS */

}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
