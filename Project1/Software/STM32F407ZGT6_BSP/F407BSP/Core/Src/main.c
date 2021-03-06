/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  /* USER CODE END Header */
  /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "math.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*******************/
#include "hmc5883l.h"
#include "mpu9250.h"
#include "encoder.h"
#include "pwmout.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int fputc(int ch, FILE* f)
{
  uint32_t temp = ch;
  HAL_UART_Transmit(&huart1, (uint8_t*)&temp, 1, 1000);
  return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t MODE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PWM_Control_Init(void);
void Encoder_Init(void);
void Calculate_Init(void);

void Task_problem(void);
void Task_scan_button(void);
void Task_Idle(void);

void rotate_board(void);
void stable_board(void);
void pid_stable_board(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//??????????????
void Calculate_Init(void)
{
  PID_param_init();
}

//??????????????
void test_TIM_Encoder(void)
{
  uint32_t enc1 = 0;
  enc1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim3)); //??????????????
  printf("%d", enc1);
}

//????????????
void test_mpu9250(void)
{
  int16_t my_test_buffer[9] = { 0 };

  MPU9250_ReadAccelerateData();
  MPU9250_ReadGYROData();
  MPU9250_ReadMagneticData();
  MPU9250_Output(my_test_buffer);

  printf("%d,%d,%d\r\n", my_test_buffer[0], my_test_buffer[3], my_test_buffer[6]);
}

//????????
void test_mixed(void)
{
  PWM_Rotate(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3));
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
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  MPU9250_Init(); //??????????
  PWM_Control_Init(); //??????????????
  Encoder_Init(); //????????????
  Calculate_Init(); //??????????
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    Task_scan_button();
	  test_mpu9250();
    //Task_problem();
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
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Task_problem(void)
{
  if (MODE == 1) //????????????
  {
    rotate_board();
  }
  else if (MODE == 2) //????????1
  {
    stable_board();
  }
  else if (MODE == 3) //????????2
  {
    pid_stable_board();
  }
  else //MODE == 0
  {
    Task_Idle();
  }

  switch (MODE)
  {
  case 1:
    printf("rotate\r\n");
    break;
  case 2:
    printf("coin_mode_a\r\n");
    break;
  case 3:
    printf("coin_mode_b\r\n");
    break;
  case 0:
    break;
  default:
    break;
  }
}

void Task_scan_button(void)
{
  if ((HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == RESET) || (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == RESET))
  {
    HAL_Delay(5);
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == RESET)
    {
      if (MODE == 3)
      {
        MODE = 0;
      }
      else
      {
        MODE++;
      }
    }
    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == RESET)
    {
      if (MODE == 0)
      {
        MODE = 3;
      }
      else
      {
        MODE--;
      }
    }
  }
}

//??????????????
void Task_Idle(void)
{
  PWM_Control_Stop();
}

/**
  ??????
3-5??????????
??????????????????????????????
????????????????45??
*/
void rotate_board(void)
{
  uint8_t count;
  volatile uint8_t last;
  while (count == 5)
  {
    if (Encoder_GetDirection() != last) //??????????????????????????
    {
      count++;
      last = Encoder_GetDirection();
      PWM_Rotate(360);
    }
  }
}

/**
  ??????
??????????????
????????????????????????????
*/
void stable_board(void)
{
  PWM_Rotate(Encoder_Get_Angle());
}

/**
  ??????
????PID??????????????????????
??????????????????????????
*/
void pid_stable_board(void)
{
  PWM_Rotate(PID_realize(Encoder_Get_Angle()));
}


/**
    ????????1
????????????????
????????????4/9 theta
????????????-????
????????????-????
*/
void laser_pointer_rotate(void)
{
  float THETA = Encoder_Get_Angle();
  if (Encoder_GetDirection() < 0)
  {
    PWM_Rotate(THETA += atan((100 - 100 * cos(THETA)) / (150 - 100 * sin(THETA))));
  }
  else
  {
    PWM_Rotate(THETA += atan((100 - 100 * cos(THETA)) / (150 + 100 * sin(THETA))));
  }
}
/*
volatile float PID_Realize(volatile float laser_pointer_rotate)
{
  pid.target_val = laser_pointer_rotate;
  pid.err = pid.target_val - pid.actual_val;
  volatile float incrementAngle = pid.Kp * (pid.err - pid.err_next) + pid.Ki * pid.err + pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);
  pid.actual_val += incrementAngle;
  pid.err_last = pid.err_next;
  pid.err_next = pid.err;
  return pid.actual_val;
}
void pid_laser_pointer_rotate(void)
{

  volatile float theta = 0;
  uint8_t flag = 0;
  if (flag == 0) //????????????
  {
    pid_laser_pointer_rotate();
  }
  else
  {
    pid_laser_pointer_rotate();
  }

}


*/












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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/