#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

//typedef  void (*pFunction)(void);

extern uint8_t    gucha_remote_ip_4[4];                            	  /* Զ��IP��ַ                   */
extern uint16_t   gun_remote_port;                            	  /* Զ�˶˿ں�                   */
extern uint16_t   gun_local_port;                             	  /* ���屾�ض˿�                 */



#define MAX_BUF_SIZE		 	1460//每个数据包大小
#define KEEP_ALIVE_TIME	     	30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0//默认IP参数
#define IP_FROM_DHCP	        1//DHCP动态分配IP参数

extern  uint8_t	guch_ipfrom;
extern  uint8_t gucha_mac_6[6];
#pragma pack(1)//C编译器将按照1个字节对齐。

// W5500主要配置参数；
typedef struct {
    uint8_t *uchp_mac_6;//MAC地址
    uint8_t *uchp_sub_4;//子网掩码地址
    uint8_t *uchp_gwy_4;//网关地址
    uint8_t *uchp_dns_4;//DNS服务器地址
    uint8_t *uchp_lip_4;//本地IP地址
    uint8_t *uchp_rip_4;//远程IP地址
    uint16_t un_lport;//本地端口号
    uint16_t un_rport;//远端端口号
} W5500Config_T;

#pragma pack()//取消自定义字节对齐方式

extern W5500Config_T  gT_netconfig;
extern uint8_t dhcp_ok;//
extern uint32_t	dhcp_time;//
extern __IO uint8_t	ntptimer;//

//W5500 - SPI 读写相关函数
void  vFN_W5500_Write1Baye_IO( uint32_t mui_addr,uint8_t much_data);//写1个字节数据到W5500
void  vFN_W5500_Write2Baye_IO(uint32_t mui_addr,uint16_t much_data);//写2个字节数据到W5500
void vFN_W5500_Write4Baye_IO(uint32_t mui_addr,uint32_t much_data);
uint16_t unFN_W5500WriteBuf_IO(uint32_t mui_addr,uint8_t* much_buf,uint16_t mun_len);//写N个字节数据到W5500

uint8_t  uchFN_W5500_Read1Baye_IO(uint32_t mui_addr);//从W5500读出1个字节
uint16_t unFN_W5500_Read2Baye_IO(uint32_t mui_addr);//从W5500读出2个字节
uint32_t uiFN_W5500_Read4Baye_IO(uint32_t mui_addr);//从W5500读出4个字节
uint16_t unFN_W5500ReadBuf_IO(uint32_t mui_addr,uint8_t* much_buf,uint16_t mun_len);//从W5500读出N个字节

//W5500基本配置相关函数
void vFN_W5500Reset_IO(void);//硬件复位W5500
void vFN_W5500_Set_MAC(void);//配置W5500 MAC地址
void vFN_W5500_Set_IP(void);//配置W5500 IP地址

#endif
