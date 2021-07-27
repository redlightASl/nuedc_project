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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//#include "hmc5883l.h"
#include "mpu9250.h"
#include "encoder.h"
#include "pwmout.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int fputc(int ch,FILE *f)
{
	uint32_t temp = ch;
    HAL_UART_Transmit(&huart1, (uint8_t *)&temp, 1, 1000);
    return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define START
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
void pid_laser_pointer(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//基本算法初始化
void Calculate_Init(void)
{
	PID_param_init();
}

//编码器驱动测试
void test_TIM_Encoder(void)
{
	uint32_t enc1 = 0;
	enc1 = (uint32_t)(__HAL_TIM_GET_COUNTER(&htim3)); //获取编码器的值
	printf("%d", enc1);
}

//六轴驱动测试
void test_mpu9250(void)
{
	int16_t my_test_buffer[9] = {0};
	
	MPU9250_ReadAccelerateData();
	MPU9250_ReadGYROData();
	MPU9250_ReadMagneticData();
	MPU9250_Output(my_test_buffer);

	printf("%d,%d,%d\r\n",my_test_buffer[0],my_test_buffer[3],my_test_buffer[6]);
}

//混合测试
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
	MPU9250_Init(); //六轴初始化
	PWM_Control_Init(); //步进电机初始化
	Encoder_Init(); //编码器初始化
	Calculate_Init(); //算法初始化
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
#ifdef START
		Task_scan_button();
		Task_problem();
#else
//	  test_TIM_Encoder();
	  test_mpu9250();
#endif
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
	if(MODE == 1) //摆杆旋转模式
	{
		rotate_board();
	}
	else if(MODE == 2) //硬币模式1
	{
		stable_board();
	}
	else if(MODE == 3) //硬币模式2
	{
		pid_stable_board();
	}
	else //MODE == 0
	{
		Task_Idle();
	}
}

void Task_scan_button(void)
{
	if((HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == RESET) || (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == RESET))
	{
		HAL_Delay(5);
		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == RESET)
		{
			if(MODE == 3)
			{
				MODE = 0;
			}
			else
			{
				MODE++;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == RESET)
		{
			if(MODE == 0)
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

//停止输出，复位
void Task_Idle(void)
{
	PWM_Control_Stop();
}

/**
	第一题
3-5个周期内，
摆杆每摆一个周期，平板旋转一周
偏差绝对值不大于45度
*/
void rotate_board(void)
{
	uint8_t count;
	volatile uint8_t last;
	while(count == 5)
	{
		if(Encoder_GetDirection() != last) //检测到反转，则算作一个周期
		{
			count++;
			last = Encoder_GetDirection();
			PWM_Rotate(360);
		}
	}
}

/**
	第二题
让平板和摆保持垂直
摆角是多少，步进电机就转多少
*/
void stable_board(void)
{
	PWM_Rotate(Encoder_Get_Angle());
}

/**
	第三题
使用PID算法让平板尽量保证水平
先预测摆角再让步进电机转动
*/
void pid_stable_board(void)
{
	//volatile float target_angle = Encoder_Get_Angle();
	//volatile float target_speed = 
	PWM_Rotate(PID_realize(Encoder_Get_Angle()));
}

/**
	发挥部分
经过数学计算得到
应使平板转动4/9 theta
靠近平衡位置-正转
远离平衡位置-反转
*/
void pid_laser_pointer(void)
{
	if(close_to_middle()) //靠近平衡位置
	{
		pid_laser_pointer_rotate(MOV_THETA * Encoder_Get_Angle(), 0);
	}
	else
	{
		pid_laser_pointer_rotate(MOV_THETA * Encoder_Get_Angle(), 1);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
