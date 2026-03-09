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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "eeprom.h"
#include "key.h"
#include "led.h"
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

/* USER CODE BEGIN PV */
typedef struct{
    uint8_t buy_num;
    float prise;
    uint8_t num;
}Good;

uint8_t page = 0;//0 == SHOP;1 == PRISE;2 == REP
Good X = { 0, 1.0, 10};
Good Y = { 0, 1.0, 10};
uint8_t eeprom_updata_flag = 0;
uint8_t eeprom_read_flag = 1;
char receive_buff[30];
uint16_t led_timer ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UI_Proc();
void KEY_Proc();
void EEPROM_Proc();
void LED_Proc();
void PWM_Proc();
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
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_Clear(Black);
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    
    I2CInit();
    HAL_UART_Init(&huart1);
    HAL_UART_Receive_IT(&huart1, (uint8_t*)receive_buff, 1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    htim2.Instance->ARR = 500-1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      EEPROM_Proc();
      KEY_Proc();
      UI_Proc();
      LED_Proc();
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
void KEY_Proc(){
    static uint8_t key[5];//index is Key id
    static char buff[20];
    
    KEY_Scan();
    KEY_GetFlag(key);
    
    if(key[1]){
        LCD_Clear(Black);
        if(++page == 3) page = 0;
    }    
    switch (page)
    {
    	case 0:
            if(key[2]) if(++X.buy_num == X.num + 1) X.buy_num = 0;
            if(key[3]) if(++Y.buy_num == Y.num + 1) Y.buy_num = 0;
            if(key[4]) {
                sprintf(buff, "X:%d,Y:%d,Z:%.1f\r\n",X.buy_num, Y.buy_num, X.prise * X.buy_num + Y.prise * Y.buy_num );
                HAL_UART_Transmit(&huart1, (uint8_t*)buff, strlen(buff), HAL_MAX_DELAY);
                
                eeprom_updata_flag = 1;
                
                X.num -= X.buy_num;
                Y.num -= Y.buy_num;
                X.buy_num = 0;
                Y.buy_num = 0;
                
                htim2.Instance->CCR2 = 25;
                led.state |= 0x01;
                led_timer = 0;
            }
    		break;
    	case 1:
            if(key[2]){
                eeprom_updata_flag = 1;
                if(X.prise >= 2.0) X.prise = 1.0;// pay attention to float compare, >= cant change to ==.
                else X.prise += 0.1;
            }
            if(key[3]){
                eeprom_updata_flag = 1;
                if(Y.prise >= 2.0) Y.prise = 1.0;
                else Y.prise += 0.1;
            }
    		break;
    	default:
            if(key[2]){
                eeprom_updata_flag = 1;
                ++X.num;
            }
            if(key[3]){
                eeprom_updata_flag = 1;
                ++Y.num;
            }
    		break;
    }
}
void EEPROM_Proc(){
    if(eeprom_read_flag){
        if(EEPROM_Read(0) == 255) {
            EEPROM_Write(0,10);
            EEPROM_Write(1,10);
            EEPROM_Write(2,10);
            EEPROM_Write(3,10);
        }
        X.num = EEPROM_Read(0);
        Y.num = EEPROM_Read(1);
        X.prise = EEPROM_Read(2) / 10.0;
        Y.prise = EEPROM_Read(3) / 10.0;
        eeprom_read_flag = 0;
    }
    if(eeprom_updata_flag) {
        EEPROM_Write(0,X.num);
        EEPROM_Write(1,Y.num);
        EEPROM_Write(2,(uint8_t)(10 * X.prise));
        EEPROM_Write(3,(uint8_t)(10 * Y.prise));
        eeprom_updata_flag = 0;
    }
}
void UI_Proc(){
    static char buff[30];
    switch (page)
    {
    	case 0:
            LCD_DisplayStringLine(Line1, (u8*) "        DATA");
            sprintf(buff, "     X:%-5d",X.buy_num);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     Y:%-5d",Y.buy_num);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    	case 1:
            LCD_DisplayStringLine(Line1, (u8*) "        PRISE");
            sprintf(buff, "     X:%-5.1f",X.prise);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     Y:%-5.1f",Y.prise);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    	default:
            LCD_DisplayStringLine(Line1, (u8*) "        REP");
            sprintf(buff, "     X:%-5d",X.num);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            sprintf(buff, "     Y:%-5d",Y.num);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    }
}
void LED_Proc(){
    LED_Updata();
    if(led_timer >= 5000){
        led_timer = 0;
        led.state &= ~0x01;
        htim2.Instance->CCR2 = 150;
    }
    if(X.num == 0 && Y.num == 0){
        if(led_timer % 200 < 100)  led.state &= ~(0x01<<1);
        else led.state |= 0x01<<1;
    } else led.state &= ~(0x01<<1);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart == &huart1) {
        if(receive_buff[0] =='?') {
            sprintf(receive_buff, "X:%.1f,Y:%.1f\r\n",X.prise, Y.prise);
            HAL_UART_Transmit(&huart1, (uint8_t*)receive_buff, strlen(receive_buff), HAL_MAX_DELAY);
        }    
        HAL_UART_Receive_IT(&huart1, (uint8_t*)receive_buff, 1);
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
