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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "lcd.h"
#include "key.h"
#include "led.h"
#include "pulse.h"
#include "pwm.h"
#include "pot.h"
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

Sys sys = {
    .page = 0,
    .mode = 0,//low
    .mode_turn_flag = 0,
    .lock_duty_flag = 0,
    
    .R = 1,
    .K = 1,
    .N = 0,//change time 
    .ML = 0,// max low v
    .MH = 0,// max high v
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Ui_Proc(){
    char buff[20] = "";
    switch (sys.page)
    {
    	case 0:
            LCD_DisplayStringLine(Line1, (u8*)"        DATA");
            
            sprintf(buff, "     M = %c", sys.mode == 0? 'L':'H');
            LCD_DisplayStringLine(Line3, (u8*)buff);
            
            sprintf(buff, "     P = %-10d", pwm.duty);
            LCD_DisplayStringLine(Line4, (u8*)buff);
            
            sprintf(buff, "     V = %-8.1f", pulse.v);
            LCD_DisplayStringLine(Line5, (u8*)buff);
        
//            sprintf(buff, "   ARR = %-8d", htim2.Instance->ARR );
//            LCD_DisplayStringLine(Line6, (u8*)buff);
//            sprintf(buff, "   CCR = %-8d", htim2.Instance->CCR2 );
//            LCD_DisplayStringLine(Line7, (u8*)buff);
//            sprintf(buff, "   CNT = %-8d", htim2.Instance->CNT );
//            LCD_DisplayStringLine(Line8, (u8*)buff);
        
    		break;
    	case 1:
            LCD_DisplayStringLine(Line1, (u8*)"        PARA");
            
            sprintf(buff, "     R = %-10d", sys.R);
            LCD_DisplayStringLine(Line3, (u8*)buff);
            
            sprintf(buff, "     K = %-10d", sys.K);
            LCD_DisplayStringLine(Line4, (u8*)buff);
    		break;
    	default:
            LCD_DisplayStringLine(Line1, (u8*)"        RECD");
            
            sprintf(buff, "     N = %d", sys.N );
            LCD_DisplayStringLine(Line3, (u8*)buff);
            
            sprintf(buff, "     MH = %-5.1f", sys.MH);
            LCD_DisplayStringLine(Line4, (u8*)buff);
            
            sprintf(buff, "     ML = %-5.1f", sys.ML);
            LCD_DisplayStringLine(Line5, (u8*)buff);
    		break;
    }
}

void Key_Proc(){
    Key_Scan();
    uint8_t key1 = Key_GetFlag(1);
    uint8_t key2 = Key_GetFlag(2);
    uint8_t key3 = Key_GetFlag(3);
    uint8_t key4 = Key_GetFlag(4);
    static uint8_t line;
    
    switch (sys.page)
    {
    	case 0:
            if(key2 && sys.mode_turn_flag == 0){
                sys.mode_turn_flag = 1;
            }
            if(key4 == 1){
                sys.lock_duty_flag = 0;
            }
            if(key4 == 2){
                sys.lock_duty_flag = 1;
            }
    		break;
    	case 1:
            if(key2){
                line ^= 1;
            }
            if(key3){
                switch (line)
                {
                	case 0:
                        if(++sys.R == 11) sys.R = 1;
                		break;
                	case 1:
                        if(++sys.K == 11) sys.K = 1;
                		break;
                }
            }
            if(key4){
                switch (line)
                {
                	case 0:
                        if(--sys.R == 0) sys.R = 10;
                		break;
                	case 1:
                        if(--sys.K == 0) sys.K = 10;
                		break;
                }
            }
    		break;
    	default:
    		break;
    }
    if(key1){
        LCD_Clear(Black);
        line = 0;
        pulse.R = sys.R;
        pulse.K = sys.K;
        if(++sys.page == 3) sys.page = 0;
    }
}

void Pluse_Proc(){
    if(sys.pulse_timer < 10) return;
    sys.pulse_timer = 0;
    
    Pulse_Updata();
    if(sys.mode){
        sys.MH = sys.MH > pulse.v ? sys.MH:pulse.v; 
    }else{ 
        sys.ML = sys.ML > pulse.v ? sys.ML:pulse.v; 
    }
}

void Pot_Proc(){
    if(sys.lock_duty_flag == 1 ) return;
    Pot_Updata();
    if(pot.volt <= 1) pwm.duty = 10;
    else if(pot.volt >= 3) pwm.duty = 85;
    else pwm.duty = 10 + (pot.volt - 1) * ((85 - 10) / (3 - 1));
}

void Pwm_Proc(){
    //100 times, 50ms one time, change 40Hz every time.
    Pwm_Updata();
    
    if(sys.mode_timer < 50) return;
    sys.mode_timer = 0;
    
    if(sys.mode_turn_flag){
        switch (sys.mode)
        {
        	case 0:
                pwm.freq += 40;
                if(pwm.freq == 8000){
                    sys.mode_turn_flag = 0;
                    sys.mode = 1;
                    sys.N++;
                }
        		break;
        	case 1:
                pwm.freq -= 40;
                if(pwm.freq == 4000){
                    sys.mode_turn_flag = 0;
                    sys.mode = 0;
                    sys.N++;
                }
        		break;
        }
    }
}
void Led_Proc(){
    if(sys.page == 0) led.state |= 0x01;
    else led.state &= ~0x01;
    if(sys.mode_turn_flag && sys.led_timer >= 100) {
        sys.led_timer = 0;
        
        if( led.state & (0x01 << 1) )led.state &= ~(0x01 << 1);
        else led.state |= (0x01 << 1);
    } else{
        led.state &= ~(0x01 << 1);
    }
    if(sys.lock_duty_flag) led.state |= (0x01 << 2);
    else led.state &= ~(0x01 << 2);
    Led_Set();
}
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
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    LCD_Init();
    LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
    LCD_Clear(Black);
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      Pot_Proc();
      Pluse_Proc();
      
      Key_Proc();
      Pwm_Proc();
      
      Ui_Proc();
      Led_Proc();
//      HAL_Delay(20);
//      static char s[30];
//      static int cnt = 0;
//      sprintf(s,"%.2f   %.2f",pulse.freq,pulse.v);
//      LCD_DisplayStringLine(Line0,(u8*)s);
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
