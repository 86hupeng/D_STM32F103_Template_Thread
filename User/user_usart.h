/**
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __User_usart_H
#define __User_usart_H

  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* USER CODE BEGIN Includes */

typedef struct {
	uint8_t  uch_data_type;				//发送缓存的数据类型，数据-状态字符；
	uint8_t  uch_array_num;				//当前缓存数据的数量；
    uint8_t  uch_first_point_num;	    //前指针位置，存入一条加1，循环覆盖；
    uint8_t  uch_secend_point_num;	    //后指针位置，发送完成一个加1，循环覆盖；
    
	uint8_t  ucha_tx_buff[80];			//最大发送80个字节
	uint32_t uia_data_buff[10]; 		//10个缓存数据
	uint8_t  ucha_wait_time_ms[10];     //10个缓存数据的发送间隔时间
} Tx1BuffTypedef;


/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/



/* USER CODE BEGIN Private defines */
void  User_Rx1_FrameEnd (void);
void  User_Rx1_Analysis (char * Date);


/* USER CODE END Private defines */


/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __User_usart_H */

/************************ (C) COPYRIGHT  HP  *****END OF FILE****/
