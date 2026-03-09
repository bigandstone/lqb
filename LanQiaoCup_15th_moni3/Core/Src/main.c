/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "key.h"
#include "led.h"
#include "pot.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef enum {
    GOODS,
    STANDARD,
    PASS
}PAGE;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t page = GOODS;

uint8_t pass_R37        = 0;//R37 --- val2 ; R38 --- val1
uint8_t pass_not_R37    = 0;
float   percent_R37     = 0;
float   high_R37        = 0;
float   low_R37         = 0;
uint8_t high_R37_10     = 22;
uint8_t low_R37_10      = 12;


uint8_t pass_R38        = 0;
uint8_t pass_not_R38    = 0;
float   percent_R38     = 0;
float   high_R38        = 0;
float   low_R38         = 0;
uint8_t high_R38_10     = 30;
uint8_t low_R38_10      = 14;

uint16_t led_timer1 = 0;
uint16_t led_timer2 = 0;

char receive_buff[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UI_Proc();
void KEY_Proc();
void POT_Proc();
void LED_Proc();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    
//    HAL_UART_Receive_DMA(&huart1, (uint8_t*)receive_buff, 3);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)receive_buff, 3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      POT_Proc();
      KEY_Proc();
      UI_Proc();     
      LED_Proc();
//      static char s[20];
//      static uint16_t monitor;
//      sprintf(s, "%8d", para);
//      LCD_DisplayStringLine(Line8, (u8*)s);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void UI_Proc(){
    static char buff[20];
    switch (page)
    {
    	case GOODS:
            LCD_DisplayStringLine(Line1, (u8*)"       GOODS        ");
        
            sprintf(buff, "     R37:%.2fV     ", pot.val_2);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     R38:%.2fV     ", pot.val_1);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    	case STANDARD:
            LCD_DisplayStringLine(Line1, (u8*)"       STANDARD     ");
        
            sprintf(buff, "     SR37:%.1f-%.1f",low_R37,high_R37);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     SR38:%.1f-%.1f",low_R38,high_R38);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    	case PASS:
            LCD_DisplayStringLine(Line1, (u8*)"       PASS         ");
        
            sprintf(buff, "     PR37:%.1f%%     ",percent_R37 * 100);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     PR38:%.1f%%     ",percent_R38 * 100);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    }
}
void KEY_Proc(){
    static uint8_t flag = 0;
    static uint8_t para = 0;
    
    KEY_Updata();
    flag = KEY_GetFlag();
    
    if(flag == 1) {
        LCD_Clear(Black);
        if(++page == 3) page = GOODS; 
        para = 0;
    }
    switch (page)
    {
    	case GOODS:
            if(flag == 2) {
                if(pot.val_2 <= high_R37 && pot.val_2 >= low_R37) ++pass_R37;
                else ++pass_not_R37;
            }
            if(flag == 3) {
                if(pot.val_1 <= high_R38 && pot.val_1 >= low_R38) ++pass_R38;
                else ++pass_not_R38;
            }
    		break;
    	case STANDARD:
            if(flag == 2) {
                if(++para == 4) para = 0;
            }
            if(flag == 3) {
                switch (para) {
                	case 0: //R37 high
                        if(high_R37_10 >= 30) high_R37_10 = 22;
                        else high_R37_10 += 2;
                		break;
                	case 1: //R37 low
                        if(low_R37_10 >= 20) low_R37_10 = 12;
                        else low_R37_10 += 2;
                		break;
                	case 2: //R38 hihg
                        if(high_R38_10 >= 30) high_R38_10 = 22;
                        else high_R38_10 += 2;
                		break;
                	case 3: //R38 low
                        if(low_R38_10 >= 20) low_R38_10 = 12;
                        else low_R38_10 += 2;
                		break;
                }
            }
            if(flag == 4) {
                switch (para) {
                	case 0: //R37 high
                        if(high_R37_10 <= 22) high_R37_10 = 30;
                        else high_R37_10 -= 2;
                		break;
                	case 1: //R37 low
                        if(low_R37_10 <= 12) low_R37_10 = 20;
                        else low_R37_10 -= 2;
                		break;
                	case 2: //R38 hihg
                        if(high_R38_10 <= 22) high_R38_10 = 30;
                        else high_R38_10 -= 2;
                		break;
                	case 3: //R38 low
                        if(low_R38_10 <= 12) low_R38_10 = 20;
                        else low_R38_10 -= 2;
                		break;
                }
            }
    		break;
    	case PASS:
            if(flag == 4) {
                pass_R37        = 0;
                pass_not_R37    = 0;
                pass_R38        = 0;
                pass_not_R38    = 0;
            }
    		break;
    }
}
void POT_Proc(){
    POT_Updata();
    
    percent_R37 = pass_R37 / (float)(pass_not_R37 + pass_R37);
    percent_R38 = pass_R38 / (float)(pass_not_R38 + pass_R38);
    
    low_R37 = low_R37_10 / 10.0;
    low_R38 = low_R38_10 / 10.0;
    
    high_R38 = high_R38_10 / 10.0;
    high_R37 = high_R37_10 / 10.0;
    
    if(pass_not_R37 + pass_R37 == 0) {
        percent_R37 = 0;
    }
    if(pass_not_R38 + pass_R38 == 0) {
        percent_R38 = 0;
    }
}
void LED_Proc(){
    static uint8_t isPass_R37;
    static uint8_t last_pass_R37;
    static uint8_t isPass_R38;
    static uint8_t last_pass_R38;
    
    isPass_R37 = last_pass_R37 == pass_R37? 0:1;
    isPass_R38 = last_pass_R38 == pass_R38? 0:1;
    
    if(isPass_R37) {
        LED_Set(1);
        led_timer1 = 0;
    }
    if(led_timer1 >= 1000) LED_Reset(1);
    
    if(isPass_R38) {
        LED_Set(2);
        led_timer2 = 0;
    }
    if(led_timer2 >= 1000) LED_Reset(2);
    
    switch (page)
    {
    	case GOODS:
            LED_Set(3);
            LED_Reset(4);
            LED_Reset(5);
    		break;
    	case STANDARD:
            LED_Reset(3);
            LED_Set(4);
            LED_Reset(5);
    		break;
    	case PASS:
            LED_Reset(3);
            LED_Reset(4);
            LED_Set(5);
    		break;
    }
    
    last_pass_R37 = pass_R37;
    last_pass_R38 = pass_R38;
    LED_Updata();
}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//    if(huart == &huart1) {
//        static char transmit_buff[50];
//        
//        if(strcmp(receive_buff, "R37") == 0) {
//            sprintf(transmit_buff,"R37:%d,%d,%.1f%%\r\n",pass_not_R37 + pass_R37, pass_R37, percent_R37 * 100);
//            HAL_UART_Transmit(&huart1, (uint8_t*)transmit_buff, strlen(transmit_buff), HAL_MAX_DELAY);
//        }
//        else if(strcmp(receive_buff, "R38") == 0) {
//            sprintf(transmit_buff,"R38:%d,%d,%.1f%%\r\n",pass_not_R38 + pass_R38, pass_R38, percent_R38 * 100);
//            HAL_UART_Transmit(&huart1, (uint8_t*)transmit_buff, strlen(transmit_buff), HAL_MAX_DELAY);
//        }
//        HAL_UART_Receive_DMA(&huart1, (uint8_t*)receive_buff, 3);
//    }
//    
//}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart == &huart1) {
        static char transmit_buff[50];
        
        if(strcmp(receive_buff, "R37") == 0) {
            sprintf(transmit_buff,"R37:%d,%d,%.1f%%\r\n",pass_not_R37 + pass_R37, pass_R37, percent_R37 * 100);
            HAL_UART_Transmit(&huart1, (uint8_t*)transmit_buff, strlen(transmit_buff), HAL_MAX_DELAY);
        }
        else if(strcmp(receive_buff, "R38") == 0) {
            sprintf(transmit_buff,"R38:%d,%d,%.1f%%\r\n",pass_not_R38 + pass_R38, pass_R38, percent_R38 * 100);
            HAL_UART_Transmit(&huart1, (uint8_t*)transmit_buff, strlen(transmit_buff), HAL_MAX_DELAY);
        }
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t*)receive_buff, 3);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
