/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		≈‰÷√MCU£¨“∆÷≤W5500≥Ã–Ú–Ë“™–ﬁ∏ƒµƒŒƒº˛£¨≈‰÷√W5500µƒMAC∫ÕIPµÿ÷∑
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"
//#include "GeneralTIM/bsp_GeneralTIM.h"
//#include "i2c/bsp_EEPROM.h"

CONFIG_MSG  ConfigMsg;																	/*≈‰÷√Ω·ππÃÂ*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM¥Ê¥¢–≈œ¢Ω·ππÃÂ*/
SPI_HandleTypeDef hspi_w5500;

/*∂®“ÂMACµÿ÷∑,»Áπ˚∂‡øÈW5500Õ¯¬Á  ≈‰∞Â‘⁄Õ¨“ªœ÷≥°π§◊˜£¨«Î π”√≤ªÕ¨µƒMACµÿ÷∑*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*∂®“Âƒ¨»œIP–≈œ¢*/
uint8 local_ip[4]  ={192,168,31,231};										/*∂®“ÂW5500ƒ¨»œIPµÿ÷∑*/
uint8 subnet[4]    ={255,255,255,0};										/*∂®“ÂW5500ƒ¨»œ◊”Õ¯—⁄¬Î*/
uint8 gateway[4]   ={192,168,31,1};											/*∂®“ÂW5500ƒ¨»œÕ¯πÿ*/
uint8 dns_server[4]={114,114,114,114};									/*∂®“ÂW5500ƒ¨»œDNS*/

uint16 local_port=5000;	                       					/*∂®“Â±æµÿ∂Àø⁄*/

/*∂®“Â‘∂∂ÀIP–≈œ¢*/
uint8  remote_ip[4]={192,168,31,230};			  						/*‘∂∂ÀIPµÿ÷∑*/
uint16 remote_port=8080;																/*‘∂∂À∂Àø⁄∫≈*/

/*IP≈‰÷√∑Ω∑®—°‘Ò£¨«Î◊‘––—°‘Ò*/
uint8	ip_from=IP_FROM_DEFINE;				

uint8         dhcp_ok   = 0;													   			/*dhcp≥…π¶ªÒ»°IP*/
uint32	      ms        = 0;															  	/*∫¡√Îº∆ ˝*/
uint32	      dhcp_time = 0;															  	/*DHCP‘À––º∆ ˝*/
__IO uint8_t  ntptimer  = 0;															  	/*NPT√Îº∆ ˝*/

/**
*@brief		”≤º˛∏¥ŒªW5500
*@param		Œﬁ
*@return	Œﬁ
*/
void reset_w5500(void)
{
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN,GPIO_PIN_RESET);
  delay_ms(10);  
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN,GPIO_PIN_SET);
  delay_ms(1600);
}

/**
*@brief		≈‰÷√W5500µƒIPµÿ÷∑
*@param		Œﬁ
*@return	Œﬁ
*/
void set_w5500_ip(void)
{		
   /*∏¥÷∆∂®“Âµƒ≈‰÷√–≈œ¢µΩ≈‰÷√Ω·ππÃÂ*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
		printf("  π”√∂®“ÂµƒIP–≈œ¢≈‰÷√W5500\r\n");
	
	/* π”√EEPROM¥Ê¥¢µƒIP≤Œ ˝*/	
	if(ip_from==IP_FROM_EEPROM)
	{
		/*¥”EEPROM÷–∂¡»°IP≈‰÷√–≈œ¢*/
		read_config_from_eeprom();		
		
		/*»Áπ˚∂¡»°EEPROM÷–MAC–≈œ¢,»Áπ˚“—≈‰÷√£¨‘Úø… π”√*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM\r\n");
			/*∏¥÷∆EEPROM≈‰÷√–≈œ¢µΩ≈‰÷√µƒΩ·ππÃÂ±‰¡ø*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROMŒ¥≈‰÷√, π”√∂®“ÂµƒIP–≈œ¢≈‰÷√W5500,≤¢–¥»ÎEEPROM\r\n");
			write_config_to_eeprom();	/* π”√ƒ¨»œµƒIP–≈œ¢£¨≤¢≥ı ºªØEEPROM÷– ˝æ›*/
		}			
	}

	/* π”√DHCPªÒ»°IP≤Œ ˝£¨–Ëµ˜”√DHCP◊”∫Ø ˝*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*∏¥÷∆DHCPªÒ»°µƒ≈‰÷√–≈œ¢µΩ≈‰÷√Ω·ππÃÂ*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
			printf(" DHCP◊”≥Ã–ÚŒ¥‘À––,ªÚ’ﬂ≤ª≥…π¶\r\n");
			printf("  π”√∂®“ÂµƒIP–≈œ¢≈‰÷√W5500\r\n");
		}
	}
		
	/*“‘œ¬≈‰÷√–≈œ¢£¨∏˘æ›–Ë“™—°”√*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*Ω´IP≈‰÷√–≈œ¢–¥»ÎW5500œ‡”¶ºƒ¥Ê∆˜*/	
	vFN_W5500setSUBR(ConfigMsg.sub);
	vFN_W5500setGAR(ConfigMsg.gw);
	vFN_W5500setSIPR(ConfigMsg.lip);
	
	vFN_W5500getSIPR (local_ip);			
	printf(" W5500 IPµÿ÷∑   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	vFN_W5500getSUBR(subnet);
	printf(" W5500 ◊”Õ¯—⁄¬Î : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	vFN_W5500getGAR(gateway);
	printf(" W5500 Õ¯πÿ     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief		≈‰÷√W5500µƒMACµÿ÷∑
*@param		Œﬁ
*@return	Œﬁ
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	vFN_W5500setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		≈‰÷√W5500µƒGPIOΩ”ø⁄
*@param		Œﬁ
*@return	Œﬁ
*/
void gpio_for_w5500_config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  WIZ_SPIx_RCC_CLK_ENABLE();
  WIZ_SPI_GPIO_ClK_ENABLE();
  WIZ_SPIx_SCS_CLK_ENABLE();
	WIZ_INT_CLK_ENABLE();
  WIZ_RESET_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = WIZ_SPIx_SCLK_PIN|WIZ_SPIx_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = WIZ_SPIx_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = WIZ_SPIx_SCS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStruct);

  /* Disable the Serial Wire Jtag Debug Port SWJ-DP */
  __HAL_AFIO_REMAP_SWJ_DISABLE();

  hspi_w5500.Instance = WIZ_SPIx;
  hspi_w5500.Init.Mode = SPI_MODE_MASTER;
  hspi_w5500.Init.Direction = SPI_DIRECTION_2LINES;
  hspi_w5500.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi_w5500.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi_w5500.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi_w5500.Init.NSS = SPI_NSS_SOFT;
  hspi_w5500.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi_w5500.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi_w5500.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi_w5500.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi_w5500.Init.CRCPolynomial = 7;
  HAL_SPI_Init(&hspi_w5500);
	
  /*∂®“ÂINT“˝Ω≈*/	
  GPIO_InitStruct.Pin = WIZ_INT_PIN;                  
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStruct);

  /*∂®“ÂRESET“˝Ω≈*/
  HAL_GPIO_WritePin(WIZ_RESET_PORT, WIZ_RESET_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = WIZ_RESET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(WIZ_RESET_PORT, &GPIO_InitStruct);
}

/**
*@brief		W5500∆¨—°–≈∫≈…Ë÷√∫Ø ˝
*@param		val: Œ™°∞0°±±Ì æ∆¨—°∂Àø⁄Œ™µÕ£¨Œ™°∞1°±±Ì æ∆¨—°∂Àø⁄Œ™∏ﬂ
*@return	Œﬁ
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
	  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_RESET);
	}
	else if (val == HIGH)
	{
	  HAL_GPIO_WritePin(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS_PIN, GPIO_PIN_SET);
	}
}

/**
*@brief		…Ë÷√W5500µƒ∆¨—°∂Àø⁄SCSnŒ™µÕ
*@param		Œﬁ
*@return	Œﬁ
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		…Ë÷√W5500µƒ∆¨—°∂Àø⁄SCSnŒ™∏ﬂ
*@param		Œﬁ
*@return	Œﬁ
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

uint8_t SPI_SendByte(uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspi_w5500,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=0XFF;
  
  return d_read; 
}

/**
*@brief		STM32 SPI1∂¡–¥8Œª ˝æ›
*@param		dat£∫–¥»Îµƒ8Œª ˝æ›
*@return	Œﬁ
*/
uint8_t  IINCHIP_SpiSendData(uint8_t dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		–¥»Î“ª∏ˆ8Œª ˝æ›µΩW5500
*@param		addrbsb: –¥»Î ˝æ›µƒµÿ÷∑
*@param   data£∫–¥»Îµƒ8Œª ˝æ›
*@return	Œﬁ
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
*@brief		¥”W5500∂¡≥ˆ“ª∏ˆ8Œª ˝æ›
*@param		addrbsb: –¥»Î ˝æ›µƒµÿ÷∑
*@param   data£∫¥”–¥»Îµƒµÿ÷∑¥¶∂¡»°µΩµƒ8Œª ˝æ›
*@return	Œﬁ
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);//
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}

/**
*@brief		ÂêëW5500ÂÜôÂÖ•lenÂ≠óËäÇÊï∞ÊçÆ ÔºåIINCHIP_SpiSendDataÂáΩÊï∞ÈúÄË¶ÅÊõøÊç¢ÈáçÂÜô
*@param		addrbsb:ÂÜôÂÖ•Âú∞ÂùÄ
*@param     bufÔºöÂÜôÂÖ•ÁöÑÁºìÂ≠òÊï∞ÊçÆ
*@param     lenÔºöÂÜôÂÖ•Êï∞ÊçÆÈïøÂ∫¶
*@return	lenÔºöËøîÂõûÂÜôÂÖ•Êï∞ÊçÆÈïøÂ∫¶÷∑Ôºå
*/
uint16_t unFN_W5500WriteBuf(uint32_t mui_addr,uint8_t* much_buf,uint16_t mun_len)
{
   uint16_t i = 0;
   if(mun_len == 0) return mun_len; 
   iinchip_csoff();                               
   IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[2]);//IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[1]);//IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[0] & 0xF8 + 4); //IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
    
   for(i=0;i<mun_len;i++)
   {
     IINCHIP_SpiSendData(much_buf[i]);
   }
   iinchip_cson();                           
   return mun_len;  
}

/**
*@brief		ÂêëW5500ËØªÂá∫lenÂ≠óËäÇÊï∞ÊçÆ ÔºåIINCHIP_SpiSendDataÂáΩÊï∞ÈúÄË¶ÅÊõøÊç¢ÈáçÂÜô
*@param		addrbsb: ËØªÂá∫Âú∞ÂùÄ
*@param 	buf: ËØªÂá∫ÁöÑÁºìÂ≠òÊï∞ÊçÆ
*@param		len: ËØªÂá∫Êï∞ÊçÆÈïøÂ∫¶
*@return	len: ËøîÂõûËØªÂá∫Êï∞ÊçÆÈïøÂ∫¶÷∑Ôºå
*/
uint16 unFN_W5500ReadBuf(uint32_t mui_addr,uint8_t* much_buf,uint16_t mun_len)
{
  uint16 i = 0;
  if(mun_len == 0) return mun_len; 

  iinchip_csoff();                                
  IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[2]);//IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[1]);//IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( ((uint8_t *)&mui_addr)[0] & 0xF8);    //IINCHIP_SpiSendData( (addrbsb & 0x000000F8)); 
  for(i = 0; i < mun_len; i++)                   
  {
    much_buf[i] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return mun_len;
}

/**
*@brief		–¥≈‰÷√–≈œ¢µΩEEPROM
*@param		Œﬁ
*@return	Œﬁ
*/
void write_config_to_eeprom(void)
{
	uint16 dAddr=0;
	EEPROM_WriteBytes(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN);				
	delay_ms(10);																							
}

/**
*@brief		¥”EEPROM∂¡≈‰÷√–≈œ¢
*@param		Œﬁ
*@return	Œﬁ
*/
void read_config_from_eeprom(void)
{
  EEPROM_CheckOk();
	EEPROM_ReadBytes(EEPROM_MSG.mac,0,EEPROM_MSG_LEN);
	delay_us(10);
}

/**
*@brief		STM32∂® ±∆˜2≥ı ºªØ
*@param		Œﬁ
*@return	Œﬁ
*/
void timer2_init(void)
{
	GENERAL_TIMx_Init();		/* TIM2ø™ ºº∆ ± */
  /* ‘⁄÷–∂œƒ£ Ωœ¬∆Ù∂Ø∂® ±∆˜ */
  HAL_TIM_Base_Start_IT(&htimx);
}

/**
  * ∫Ø ˝π¶ƒ‹: ∑«◊Ë»˚ƒ£ Ωœ¬∂® ±∆˜µƒªÿµ˜∫Ø ˝
  *  ‰»Î≤Œ ˝: htim£∫∂® ±∆˜æ‰±˙
  * ∑µ ªÿ ÷µ: Œﬁ
  * Àµ    √˜: Œﬁ
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  timer2_isr();
}

/**
*@brief		dhcp”√µΩµƒ∂® ±∆˜≥ı ºªØ
*@param		Œﬁ
*@return	Œﬁ
*/
void dhcp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		ntp”√µΩµƒ∂® ±∆˜≥ı ºªØ
*@param		Œﬁ
*@return	Œﬁ
*/
void ntp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		∂® ±∆˜2÷–∂œ∫Ø ˝
*@param		Œﬁ
*@return	Œﬁ
*/
void timer2_isr(void)
{
  ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP∂® ±º”1S*/
	  #ifndef	__NTP_H__
	  ntptimer++;																						/*NTP÷ÿ ‘ ±º‰º”1S*/
	  #endif
  }
}
/**
*@brief		STM32œµÕ≥»Ì∏¥Œª∫Ø ˝
*@param		Œﬁ
*@return	Œﬁ
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" œµÕ≥÷ÿ∆Ù÷–°≠°≠\r\n");
  JumpAddress = *(__IO uint32_t*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}


