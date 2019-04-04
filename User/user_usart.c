
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "User_usart.h"
#include "gpio.h"
#include <string.h>
		
extern	UART_HandleTypeDef huart1;
extern	UART_HandleTypeDef huart2;
extern	UART_HandleTypeDef huart3;

extern 	TIM_HandleTypeDef htim2;
extern	TIM_HandleTypeDef htim3;
extern	TIM_HandleTypeDef htim4;


/* USER CODE BEGIN 0 */

	 
/* USER CODE END PV */

void  vFN_UserRx1Analysis (char * Date)
{
    
}


void  vFN_UserRx2Analysis (void)
{
    
}


void  vFN_UserRx1FrameEnd_IO (void)
{
    
}

void  vFN_UserRx2FrameEnd_IO (void)
{
    
}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback can be implemented in the user file   */
	
    if( UartHandle->Instance == USART1  )// USART1											
    {																
    }		
    else if( UartHandle->Instance == USART2  )// USART2 										
    {		
    }			
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)  
{
    static int i=0;
    if( huart->Instance == USART1  )// USART1 										
    {
        i++;
    }
    else if( huart->Instance == USART2  )// USART2 													
    {
        i--; 
    }
}

/* USER CODE END 0 */


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
