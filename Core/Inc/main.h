/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RELAY_ON_Pin GPIO_PIN_13
#define RELAY_ON_GPIO_Port GPIOC
#define VOLTAGE_Pin GPIO_PIN_0
#define VOLTAGE_GPIO_Port GPIOC
#define CURRENT_Pin GPIO_PIN_1
#define CURRENT_GPIO_Port GPIOC
#define TEMPERATURE_IN_Pin GPIO_PIN_2
#define TEMPERATURE_IN_GPIO_Port GPIOC
#define TEMPERATURE_OUT_Pin GPIO_PIN_3
#define TEMPERATURE_OUT_GPIO_Port GPIOC
#define FREQ_DETECT_Pin GPIO_PIN_0
#define FREQ_DETECT_GPIO_Port GPIOA
#define VOLTAGE_DETECT_Pin GPIO_PIN_1
#define VOLTAGE_DETECT_GPIO_Port GPIOA
#define VOLTAGE_DETECT_EXTI_IRQn EXTI1_IRQn
#define AMPLITUDE_DETECT_Pin GPIO_PIN_2
#define AMPLITUDE_DETECT_GPIO_Port GPIOA
#define AMPLITUDE_DETECT_EXTI_IRQn EXTI2_IRQn
#define DISP_CS_Pin GPIO_PIN_4
#define DISP_CS_GPIO_Port GPIOA
#define DB0_Pin GPIO_PIN_5
#define DB0_GPIO_Port GPIOA
#define DB1_Pin GPIO_PIN_6
#define DB1_GPIO_Port GPIOA
#define DB2_Pin GPIO_PIN_7
#define DB2_GPIO_Port GPIOA
#define LED_PWM_Pin GPIO_PIN_14
#define LED_PWM_GPIO_Port GPIOB
#define BUTTON_3_Pin GPIO_PIN_15
#define BUTTON_3_GPIO_Port GPIOB
#define BUTTON_2_Pin GPIO_PIN_6
#define BUTTON_2_GPIO_Port GPIOC
#define BUTTON_1_Pin GPIO_PIN_7
#define BUTTON_1_GPIO_Port GPIOC
#define DB3_Pin GPIO_PIN_8
#define DB3_GPIO_Port GPIOA
#define DB4_Pin GPIO_PIN_9
#define DB4_GPIO_Port GPIOA
#define DB5_Pin GPIO_PIN_10
#define DB5_GPIO_Port GPIOA
#define DB6_Pin GPIO_PIN_11
#define DB6_GPIO_Port GPIOA
#define DB7_Pin GPIO_PIN_12
#define DB7_GPIO_Port GPIOA
#define DISP_RD_Pin GPIO_PIN_15
#define DISP_RD_GPIO_Port GPIOA
#define DISP_WR_Pin GPIO_PIN_10
#define DISP_WR_GPIO_Port GPIOC
#define DISP_RS_Pin GPIO_PIN_11
#define DISP_RS_GPIO_Port GPIOC
#define DISP_RES_Pin GPIO_PIN_12
#define DISP_RES_GPIO_Port GPIOC
#define FLASH_CS_Pin GPIO_PIN_2
#define FLASH_CS_GPIO_Port GPIOD
#define FLASH_SCK_Pin GPIO_PIN_3
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define EEPROM_SCL_Pin GPIO_PIN_6
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin GPIO_PIN_7
#define EEPROM_SDA_GPIO_Port GPIOB
#define RELAY_OFF_Pin GPIO_PIN_9
#define RELAY_OFF_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define FREERTOS_TIM TIM7
#define LED_TIM TIM12
#define ADC_TIM TIM5
#define FREQUENCY_TIM TIM2
#define LED_TIM_Handle htim12
#define ADC_TIM_Handle htim5
#define FREQUENCY_TIM_Handle htim2
#define MEASURE_ADC_Handle hadc1
#define RTC_Handle hrtc
#define FLASH_SPI_Handle hspi1
#define DB0_Pin_Num 5
#define EEPROM_I2C_Handle hi2c1
#define LED_TIM_Channel TIM_CHANNEL_1
#define LED_TIM_CCR CCR1
#define FREQUENCY_TIM_Channel TIM_CHANNEL_1
#define FREQUENCY_TIM_CCR CCR1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
