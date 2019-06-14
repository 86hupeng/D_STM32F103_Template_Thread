
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include <string.h>
#include "norm_task.h"
#include "User_usart.h"
#include "usart.h"

/* USER CODE BEGIN 0 */

extern 	TIM_HandleTypeDef htim2;
extern	TIM_HandleTypeDef htim3;
extern	TIM_HandleTypeDef htim4;

StringData_T  Cgch_sys_time = {11, "sys time:\r\n"};  //系统时间
TxBuff_T  gT_Tx1Buf;
RxBuff_T  gT_rx1buf; 
/* USER CODE END PV */

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :UART1 - TX 数据发送与处理
* 输入参数   :
* 输出参数   :
* 返回值     :
****************************************************************************************/
void  vFN_UserTx1Analy_Pro(void)
{
//    uint8_t i=0,k=0,uch_swap;
//    uint32_t mui_us_count=0;
//    
//    gT_sysrun_mtime.ui_us_count = vFN_SystemReadTimeNow();
//    mui_us_count = gT_sysrun_mtime.ui_us_count;
//    for(i=0;mui_us_count > 0;i++)
//    {
//        gT_Tx1Buff.ucha_txbuff_80[i] = mui_us_count%10;
//        mui_us_count = mui_us_count/10;
//    }

      HAL_UART_Transmit_DMA(&huart1,(uint8_t*)Cgch_sys_time.Cchp_data,Cgch_sys_time.Cuch_count);
}


void  vFN_UserRx2Analy(void)
{
    
}

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :UART1 - TX 数据发送，DMA方式
* 输入参数   :
* 输出参数   :
* 返回值     :
****************************************************************************************/
void  vFN_UserSetTx1Frame_IO(void)
{
    
}

void  vFN_UserRx2FrameEnd_IO(void)
{
    
}







/* USER CODE END 0 */


/* USER CODE BEGIN 1 */
/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :UART接收 + DMA + 空闲中断处理
* 输入参数   :
* 返回值     :
* 备注       :在 USART1_IRQHandler()中被调用
****************************************************************************************/
void vFN_UART_RxDMA_IDLEBack(UART_HandleTypeDef *UartHandle)
{
//	uint32_t temp;
    static int i=0;
    if((__HAL_UART_GET_FLAG(UartHandle,UART_FLAG_IDLE) != RESET))//如果是空闲中断
    { 
        __HAL_UART_CLEAR_IDLEFLAG(UartHandle);//清零空闲中断标志
        __HAL_UART_CLEAR_FLAG(UartHandle, UART_FLAG_RXNE);
        HAL_UART_DMAStop(UartHandle);//停止DMA接收
        
        //判断是那个串口的数据
        if( UartHandle->Instance == USART1  )// USART1											
        {			
            i++;
//            i = UartHandle->Instance->SR;//清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
//            i = UartHandle->Instance->DR;//读取数据寄存器中的数据
////            temp = UartHandle->hdmarx->Instance->NDTR;//获取DMA中未传输的数据个数
//            temp = 80-temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
//            for (i=0;i<temp;i++)//转移并清零数组
//            {
//                gT_rx4buff.ucha_pro_80[i] = gT_rx4buff.ucha_rx_80[i];
//                gT_rx4buff.ucha_rx_80[i]  = 0;
//            }
//            gT_rx4buff.ucha_rx_status = e_UART_RX_end;//接收完成一帧数据
//            gT_rx4buff.ucha_rx_len = temp;//保存接收的数据长度
//            HAL_UART_Receive_DMA(&huart4,gT_rx4buff.ucha_rx_80,80);//开启接收重新
        }		
        else if( UartHandle->Instance == USART2  )// USART2 										
        {
            i++;
        }	
        else if( UartHandle->Instance == USART3  )// USART3										
        {
            i++;

        }	        
    }
}
/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :UART接收回调函数
* 输入参数   :
* 返回值     :
* 备注       :
****************************************************************************************/

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

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :UART发送回调函数
* 输入参数   :
* 返回值     :
* 备注       :
****************************************************************************************/
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
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
