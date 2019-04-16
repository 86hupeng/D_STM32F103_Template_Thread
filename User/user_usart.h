/**
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __User_usart_H
#define __User_usart_H

  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal.h"

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/


/* USER CODE BEGIN Private defines */
typedef struct {
	uint8_t  ucha_tx_status;				//发送缓存的数据类型，数据-状态字符；
	uint8_t  uch_array_len;				//当前缓存数据的数量；
    uint8_t  uch_one_point;	            //前指针位置，存入一条加1，循环覆盖；
    uint8_t  uch_two_point;	            //后指针位置，发送完成一个加1，循环覆盖；
    
	uint8_t  ucha_80[80];		        //最大发送80个字节
	uint32_t uia_data_8[8]; 		    //8个缓存数据
	uint8_t  ucha_wait_ms_8[8];         //8个缓存数据的发送间隔时间
} TxBuff_T;

// UART 接收缓存
typedef struct {
    uint8_t  ucha_rx_status;            //接收状态
    uint8_t  ucha_rx_len;               //接收长度
	uint8_t  ucha_rx_80[80];		    //最大接收80个字节
    uint8_t  ucha_pro_80[80];		    //数据处理数组80个字节
} RxBuff_T;

// TX数据常量类型，记录字符串指针与字符串长度；
typedef struct {
	uint8_t  Cuch_count;			//长度；
	char    *Cchp_data;           //指向常量字符串的指针
} StringData_T;


// UART状态类型；
enum USE_UART_STATUS_TYPE
{	
	e_UART_RX_going   = 0x01,           //接收进行中
	e_UART_RX_end,                      //接收完成
	e_UART_RP_going,                    //接收处理中    
	e_UART_RP_end,                      //接收处理完成 
	e_UART_TX_going   = 0x10,           //发送进行中
	e_UART_TX_end,                      //发送完成
	e_UART_TP_going,                    //发送处理中
	e_UART_TP_end,                      //发送处理完成
};


extern  StringData_T Cgch_sys_time ;
extern  StringData_T Cgch_sys_time ;
extern  RxBuff_T gT_rx1buff;

void  User_Rx1_FrameEnd (void);
void  User_Rx1_Analysis (char * Date);
void  vFN_UserTx1Analy_Pro(void); 
void  vFN_UART_RxDMA_IDLEBack(UART_HandleTypeDef *UartHandle); 
/* USER CODE END Private defines */


/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __User_usart_H */

/************************ (C) COPYRIGHT  HP  *****END OF FILE****/
