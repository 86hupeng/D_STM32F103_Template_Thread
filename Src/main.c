/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "User_usart.h"
#include "w5500_conf.h"
#include "socket.h"
#include "w5500.h"
#include "tcp_demo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int Crc16(volatile uint8_t *ptr, int count);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

void mainP(uint8_t * muchp_addr);
uint8_t ABC[4]={0X12 ,0X34 ,0X56 ,0X78 };
uint8_t *ABP = ABC;
uint8_t gucha_macrx_6[6];
uint32_t gui_mac_data=0;
uint16_t gun_mac_data=0;
uint8_t guch_mac_data=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t sent_uart1[]="this is a test for uart1\r\n";
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern SPI_HandleTypeDef hspi1;
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
 // vFN_SPI1GPIO_Init();
  vFN_W5500GPIO_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_CAN_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  
  HAL_UART_Receive_DMA(&huart1,gT_rx1buf.ucha_rx_80,80);//接收DMA串口1，空闲中断方式接�?
  W5500_SPI1CS_ON_GPIO_LOW;
  
  HAL_SPI_Transmit(&hspi1, gT_rx1buf.ucha_rx_80, 1, 10);//发�??1个SPI字节，消除CLK默认低电平的错误
  W5500_SPI1CS_OFF_GPIO_HIGH;
  
  vFN_W5500_Set_IP();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        
        HAL_Delay(100);
        vFN_W5500_TCP_Client();
//        vFN_UserTx1Analy_Pro();
//       W5500_SPI1CS_ON_GPIO_LOW;  
//       HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)Cgch_sys_time.Cchp_data, gT_rx1buf.ucha_rx_80,Cgch_sys_time.Cuch_count, 1000);
//       W5500_SPI1CS_OFF_GPIO_HIGH;   
//        mainP(ABC);
//        HAL_Delay(500);
//        mainP(ABP);
        //vFN_W5500setSHAR(gucha_mac_6);//设置MAC地址
//        vFN_W5500_Write1Baye_IO(SHAR0,0X01);
//        vFN_W5500_Write1Baye_IO(SHAR0,0X01);
//        unFN_W5500ReadBuf_IO(SHAR0,gucha_macrx_6,6);
//        vFN_W5500_Write2Baye_IO(SHAR0,0X0102);
//        unFN_W5500ReadBuf_IO(SHAR0,gucha_macrx_6,6);
//        vFN_W5500_Write4Baye_IO(SHAR0,0X01020304);
//        unFN_W5500ReadBuf_IO(SHAR0,gucha_macrx_6,6);
//        gui_mac_data = uiFN_W5500_Read4Baye_IO(SHAR0);
//        vFN_W5500getSHAR(gucha_macrx_6);
//        guch_mac_data = uchFN_W5500_Read1Baye_IO(SHAR0);
//        unFN_W5500ReadBuf_IO(SHAR0,gucha_macrx_6,6);
//        gun_mac_data = unFN_W5500_Read2Baye_IO(SHAR0);
//        unFN_W5500ReadBuf_IO(SHAR0,gucha_macrx_6,6);

        
        
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
