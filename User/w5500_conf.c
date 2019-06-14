/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		胡鹏 
* @version 		V1.0
* @date    		2019-04-16
* @brief  		配置W5500 , MAC\IP地址等等
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "gpio.h"
#include "w5500_conf.h"
#include "w5500.h"
#include "spi.h"

W5500Config_T  gT_netconfig;//配置结构体
//定义IP相关信息
uint8_t gucha_mac_6[6]={0x12,0x34,0x56,0x78,0x9A,0xBC};//MAC地址
uint8_t gucha_dns_server_4[4]={8,8,8,8};//DNS
uint8_t gucha_local_ip_4[4]  ={10,7,5,121};//本地IP地址
uint8_t gucha_subnet_4[4]    ={255,255,255,0};//子网掩码
uint8_t gucha_gateway_4[4]   ={10,7,5,1};//网关
uint16_t gun_local_port=4999;//本地端口号

//定义远端IP信息
uint8_t  gucha_remote_ip_4[4]={10,7,5,1};//远端IP地址
uint16_t gun_remote_port=6000;//远端端口号

//IP配置方法
uint8_t	guch_ipfrom = IP_FROM_DEFINE;				

/**
*@brief		W5500初始化
*@param		无
*@return	无
*/
void vFN_W5500Init_Pro(void)
{
    vFN_W5500Reset_IO(); //硬件复位
    vFN_W5500_Set_IP();  //设置IP地址
}



/**
*@brief		硬件复位W5500
*@param		无
*@return	无
*/
void vFN_W5500Reset_IO(void)
{
    W5500_RESET_ON_GPIO_LOW;
    HAL_Delay(2);//默认500us
    W5500_RESET_OFF_GPIO_HIGH;
    HAL_Delay(2);//默认1ms
}

/**
*@brief		配置W5500 IP地址
*@param		无
*@return	无
*/
void vFN_W5500_Set_IP(void)
{		
   //IP配置赋值到结构体
    if(guch_ipfrom==IP_FROM_DEFINE)
    {
        gT_netconfig.uchp_mac_6 = gucha_mac_6;//MAC地址
        gT_netconfig.uchp_dns_4 = gucha_dns_server_4;//DNS
        gT_netconfig.uchp_lip_4 = gucha_local_ip_4;//本地IP地址
        gT_netconfig.uchp_sub_4 = gucha_subnet_4;//子网掩码
        gT_netconfig.uchp_gwy_4 = gucha_gateway_4;//网关
	}
	//获取 DHCP IP参数，需要调用DHCP子函数	
	else if(guch_ipfrom==IP_FROM_DHCP)								
	{
		//判断是否DHCP成功
		if(dhcp_ok==1)
		{		 
//			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
//			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
//			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
//			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
//			printf(" DHCP子程序未运行，或者不成功\r\n");
//			printf(" 使用定义的的IP信息配置W5500\r\n");
		}
	}
		

	//将IP参数写入 W5500寄存器 
    vFN_W5500setSHAR(gT_netconfig.uchp_mac_6);//设置MAC地址
    vFN_W5500setSIPR(gT_netconfig.uchp_lip_4);//设置本地IP地址
	vFN_W5500setSUBR(gT_netconfig.uchp_sub_4);//设置子网掩码地址
	vFN_W5500setGAR(gT_netconfig.uchp_gwy_4);//设置网关地址

}

/**
*@brief		写入一个字节数据到W5500，发送4个字节数据
*@param		mui_addr: 写入地址
*@param     much_data：写入数据
*@return	无
*/
void vFN_W5500_Write1Baye_IO(uint32_t mui_addr,uint8_t much_data)
{
    uint8_t much_tx_data_4[4]= {0,0,0,0};
    //缓存数据
    much_tx_data_4[0] = ((uint8_t *)&mui_addr)[2];
    much_tx_data_4[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_4[2] = (((uint8_t *)&mui_addr)[0] & 0xF8) + 4;
    much_tx_data_4[3] = much_data;
    //发送数据
    W5500_SPI1CS_ON_GPIO_LOW;   
    HAL_SPI_Transmit(&hspi1, much_tx_data_4, 4, 10);
    W5500_SPI1CS_OFF_GPIO_HIGH;
}

/**
*@brief		写入2个字节数据到W5500，发送5个字节数据
*@param		mui_addr: 写入地址
*@param     much_data：写入数据
*@return	无
*/
void vFN_W5500_Write2Baye_IO(uint32_t mui_addr,uint16_t much_data)
{
    uint8_t much_tx_data_5[5]= {0,0,0,0,0};
    //缓存数据
    much_tx_data_5[0] = ((uint8_t *)&mui_addr)[2];//高字节在前
    much_tx_data_5[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_5[2] = (((uint8_t *)&mui_addr)[0] & 0xF8) + 4;
    much_tx_data_5[3] = ((uint8_t *)&much_data)[1];//高字节在前
    much_tx_data_5[4] = ((uint8_t *)&much_data)[0];
    //发送数据
    W5500_SPI1CS_ON_GPIO_LOW;
    HAL_SPI_Transmit(&hspi1, much_tx_data_5, 5, 10);
    W5500_SPI1CS_OFF_GPIO_HIGH;
}

/**
*@brief		写入2个字节数据到W5500，发送7个字节数据
*@param		mui_addr: 写入地址
*@param     much_data：写入数据
*@return	无
*/
void vFN_W5500_Write4Baye_IO(uint32_t mui_addr,uint32_t much_data)
{
    uint8_t much_tx_data_7[7]= {0,0,0,0,0};
    //缓存数据
    much_tx_data_7[0] = ((uint8_t *)&mui_addr)[2];//高字节在前
    much_tx_data_7[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_7[2] = (((uint8_t *)&mui_addr)[0] & 0xF8) + 4;
    much_tx_data_7[3] = ((uint8_t *)&much_data)[3];//高字节在前
    much_tx_data_7[4] = ((uint8_t *)&much_data)[2];
    much_tx_data_7[5] = ((uint8_t *)&much_data)[1];
    much_tx_data_7[6] = ((uint8_t *)&much_data)[0];
    //发送数据
    W5500_SPI1CS_ON_GPIO_LOW;   
    HAL_SPI_Transmit(&hspi1, much_tx_data_7, 7, 10);
    W5500_SPI1CS_OFF_GPIO_HIGH;
}

/**
*@brief		向W5500写入N字节数据 
*@param		addrbsb:写入地址
*@param     buf：写入的缓存数据
*@param     len：写入数据长度
*@return	len：返回写入数据长度ַ，
*/
uint16_t unFN_W5500WriteBuf_IO(uint32_t mui_addr,uint8_t* muchp_buf,uint16_t mun_len)
{
    uint16_t i=0,j=0;
    uint8_t much_tx_data_500[500]; 
    if(mun_len > 0) 
    {   //缓存数据      
        much_tx_data_500[i++] = ((uint8_t *)&mui_addr)[2];
        much_tx_data_500[i++] = ((uint8_t *)&mui_addr)[1];
        much_tx_data_500[i++] = (((uint8_t *)&mui_addr)[0] & 0xF8) + 4;
        for(j=0;j<mun_len;j++)
        {
            much_tx_data_500[i++] = muchp_buf[j]; 
        }
        i = mun_len +3;
         //发送数据  
        W5500_SPI1CS_ON_GPIO_LOW;   
        HAL_SPI_Transmit(&hspi1, much_tx_data_500, i, 100);
        W5500_SPI1CS_OFF_GPIO_HIGH;
    }
    return mun_len;  
}

/**
*@brief		从W5500地址读出一个字节
*@param		mui_addr:读地址
*@param     
*@return	data：读到的数据
*/
uint8_t uchFN_W5500_Read1Baye_IO(uint32_t mui_addr)
{
    uint8_t much_data = 0;
    uint8_t much_tx_data_4[4] = {0,0,0,0};

    W5500_SPI1CS_ON_GPIO_LOW;  
    much_tx_data_4[0] = ((uint8_t *)&mui_addr)[2];
    much_tx_data_4[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_4[2] = ((uint8_t *)&mui_addr)[0] & 0xF8;
    HAL_SPI_Transmit(&hspi1, much_tx_data_4, 3, 10);
    HAL_SPI_Receive(&hspi1, &much_data, 1, 10);
    W5500_SPI1CS_OFF_GPIO_HIGH;

    return much_data;    
}

/**
*@brief		从W5500地址读出连续的2个字节，uint16_t数据，高字节在前
*@param		mui_addr:读地址
*@param     
*@return	mun_data：读到的uint16_t数据
*/
uint16_t unFN_W5500_Read2Baye_IO(uint32_t mui_addr)
{
    uint8_t much_tx_data_4[4] = {0,0,0,0};
    uint8_t much_data[2] = {0,0};
    uint16_t mun_data = 0;
    //数据读取
    W5500_SPI1CS_ON_GPIO_LOW;  
    much_tx_data_4[0] = ((uint8_t *)&mui_addr)[2];
    much_tx_data_4[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_4[2] = ((uint8_t *)&mui_addr)[0] & 0xF8;
    HAL_SPI_Transmit(&hspi1, much_tx_data_4, 3, 10);
    HAL_SPI_Receive(&hspi1, much_data, 2, 10); //读两个字节
    W5500_SPI1CS_OFF_GPIO_HIGH;
    //数据处理，转换成uint16_t
    ((uint8_t *)&mun_data)[1] = much_data[0];//高字节
    ((uint8_t *)&mun_data)[0] = much_data[1];
    return mun_data;    
}

/**
*@brief		从W5500地址读出连续的4个字节
*@param		mui_addr:读地址
*@param     
*@return	data：读到的数据
*/
uint32_t uiFN_W5500_Read4Baye_IO(uint32_t mui_addr)
{
    uint8_t much_rxdata[4] = {0,0,0,0};
    uint32_t mui_data = 0;
    uint8_t much_tx_data_4[4] = {0,0,0,0};

    W5500_SPI1CS_ON_GPIO_LOW;  
    much_tx_data_4[0] = ((uint8_t *)&mui_addr)[2];
    much_tx_data_4[1] = ((uint8_t *)&mui_addr)[1];
    much_tx_data_4[2] = ((uint8_t *)&mui_addr)[0] & 0xF8;
    HAL_SPI_Transmit(&hspi1, much_tx_data_4, 3, 10);
    HAL_SPI_Receive(&hspi1, much_rxdata, 4, 10); //读四个字节
    W5500_SPI1CS_OFF_GPIO_HIGH;
    
    ((uint8_t *)&mui_data)[3] = much_rxdata[0];//高字节
    ((uint8_t *)&mui_data)[2] = much_rxdata[1];//
    ((uint8_t *)&mui_data)[1] = much_rxdata[2];//
    ((uint8_t *)&mui_data)[0] = much_rxdata[3];//
    return mui_data;    
}

/**
*@brief		向W5500读出N字节数据 
*@param		addrbsb: 读出地址
*@param 	buf: 读出的缓存数据
*@param		len: 读出数据长度
*@return	len: 返回读出数据长度ַ，
*/
uint16_t unFN_W5500ReadBuf_IO(uint32_t mui_addr,uint8_t* much_buf,uint16_t mun_len)
{
    uint8_t much_tx_data_4[4];  
    
    if(mun_len > 0) 
    {
        much_tx_data_4[0] = ((uint8_t *)&mui_addr)[2];
        much_tx_data_4[1] = ((uint8_t *)&mui_addr)[1];
        much_tx_data_4[2] = ((uint8_t *)&mui_addr)[0] & 0xF8;

        W5500_SPI1CS_ON_GPIO_LOW;  
        HAL_SPI_Transmit(&hspi1, much_tx_data_4, 3, 10);
        HAL_SPI_Receive(&hspi1, much_buf, mun_len, 10);
        W5500_SPI1CS_OFF_GPIO_HIGH;
     } 
    return mun_len;
}


