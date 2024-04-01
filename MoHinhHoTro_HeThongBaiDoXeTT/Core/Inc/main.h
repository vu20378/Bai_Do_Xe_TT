/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_EXIT_INT_Pin GPIO_PIN_13
#define BTN_EXIT_INT_GPIO_Port GPIOC
#define BTN_Menu_Pin GPIO_PIN_14
#define BTN_Menu_GPIO_Port GPIOC
#define BTN_Back_Pin GPIO_PIN_15
#define BTN_Back_GPIO_Port GPIOC
#define SS_Lua_Pin GPIO_PIN_0
#define SS_Lua_GPIO_Port GPIOA
#define SS_Lua_EXTI_IRQn EXTI0_IRQn
#define SS_BanDem_Pin GPIO_PIN_1
#define SS_BanDem_GPIO_Port GPIOA
#define LED_Ban_Dem_Pin GPIO_PIN_3
#define LED_Ban_Dem_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_4
#define SDA_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_12
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_13
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_14
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_15
#define LCD_D4_GPIO_Port GPIOB
#define Servo_Vao_Pin GPIO_PIN_8
#define Servo_Vao_GPIO_Port GPIOA
#define Servo_Ra_Pin GPIO_PIN_9
#define Servo_Ra_GPIO_Port GPIOA
#define LCD_E_Pin GPIO_PIN_10
#define LCD_E_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_11
#define LCD_RW_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_12
#define LCD_RS_GPIO_Port GPIOA
#define LED_NAP_Pin GPIO_PIN_15
#define LED_NAP_GPIO_Port GPIOA
#define ViTri_1_Pin GPIO_PIN_3
#define ViTri_1_GPIO_Port GPIOB
#define ViTri_2_Pin GPIO_PIN_4
#define ViTri_2_GPIO_Port GPIOB
#define ViTri_3_Pin GPIO_PIN_5
#define ViTri_3_GPIO_Port GPIOB
#define ViTri_4_Pin GPIO_PIN_6
#define ViTri_4_GPIO_Port GPIOB
#define ViTri_5_Pin GPIO_PIN_7
#define ViTri_5_GPIO_Port GPIOB
#define ViTri_6_Pin GPIO_PIN_8
#define ViTri_6_GPIO_Port GPIOB
#define Coi_Bao_Pin GPIO_PIN_9
#define Coi_Bao_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
