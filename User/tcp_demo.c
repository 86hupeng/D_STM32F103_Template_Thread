/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		胡鹏 
* @version 		V1.0
* @date    		2019-04-14
* @brief   		TCP 功能函数
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"

uint8_t buff[2048];//缓存

/**
*@brief		TCP Server 回环函数
*@param		无
*@return	无
*/
void vFN_W5500_TCP_Server(void)
{	
	uint16_t mun_len=0;  
	switch(uchFN_W5500getSn_SR(SOCK_TCPS))// 获取socket状态
	{
		case SOCK_CLOSED://socket处于关闭状态
                uchFN_W5500SocketInit(SOCK_TCPS ,Sn_MR_TCP,gun_local_port,Sn_MR_ND);//打开socket，将处于已经初始化状态
                break;     
    
		case SOCK_INIT://socket处于已经初始化状态
                uchFN_W5500SocketListen(SOCK_TCPS);//socket建立监听，
                break;
		
		case SOCK_ESTABLISHED://socket处于连接建立状态，
		
                if(uchFN_W5500getSn_IR(SOCK_TCPS) & Sn_IR_CON)
                {
                    vFN_W5500setSn_IR(SOCK_TCPS, Sn_IR_CON);//清除接收中断标志，
                }
                mun_len = unFN_W5500getSn_RX_RSR(SOCK_TCPS);//获取已接收数据的长度
                if(mun_len>0)
                {
                    unFN_W5500SocketRecv(SOCK_TCPS,buff,mun_len);//接网络收数据
                    buff[mun_len]=0x00;//添加字符串结束符
                    // printf("%s\r\n",buff); 输出到串口
                    unFN_W5500SocketSend(SOCK_TCPS,buff,mun_len);//向网络发送数据
                }
                break;
		
		case SOCK_CLOSE_WAIT://处于等待关闭状态
                vFN_W5500_SocketClose(SOCK_TCPS);
                break;
	}
}

/**         需要增加网线插入检测
*@brief		TCP Client回环函数
*@param		无
*@return	无
*/
void vFN_W5500_TCP_Client(void)
{	
    uint16_t mun_len=0;	
    static uint8_t much_return=0;
	switch(uchFN_W5500getSn_SR(SOCK_TCPC))// 获取socket状态，SOCK_TCPC=socket端口号
	{
		case SOCK_CLOSED://socket处于关闭状态
                uchFN_W5500SocketInit(SOCK_TCPC,Sn_MR_TCP,gun_local_port++,Sn_MR_ND);//打开socket，将处于已经初始化状态 - Sn_CR_OPEN
                break;
		
		case SOCK_INIT://socket处于已经初始化状态
                uchFN_W5500SocketConnect(SOCK_TCPC,gucha_remote_ip_4,gun_remote_port);//socket连接服务器， socket号 + IP地址 + 端口
                break;
		
		case SOCK_ESTABLISHED://socket处于连接建立状态，
                if(uchFN_W5500getSn_IR(SOCK_TCPC) & Sn_IR_CON)
                {
                    vFN_W5500setSn_IR(SOCK_TCPC, Sn_IR_CON);//清除接收中断标志，
                }
            
                mun_len = unFN_W5500getSn_RX_RSR(SOCK_TCPC);//获取已接收数据的长度
                if(mun_len>0)
                {
                    unFN_W5500SocketRecv(SOCK_TCPC,buff,mun_len);//接网络收数据
                    buff[mun_len++] = ++much_return;//添加计数
                    buff[mun_len] = 0x00;//添加字符串结束符
                    unFN_W5500SocketSend(SOCK_TCPC,buff,mun_len);//向网络发送数据
                }		  
                break;
			
		case SOCK_CLOSE_WAIT://处于等待关闭状态
                vFN_W5500_SocketClose(SOCK_TCPC);
                break;
	}
}

