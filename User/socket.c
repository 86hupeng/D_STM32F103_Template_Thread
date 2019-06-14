/**
******************************************************************************
* @file    		Socket.c
* @author  		胡鹏
* @version 		V1.0
* @date    		2019-04-xx
* @brief   		Socket 相关函数 
******************************************************************************
*/
#include "socket.h"
#include "W5500_conf.h"
#include "stdio.h"
#include "w5500.h"

/**
*@brief   This Socket function initialize the channel in perticular mode, 
            and set the port and wait for W5200 done it.
*@param		s: socket number.
*@param		protocol: The socket to chose.
*@param		port:The port to bind.
*@param		flag: Set some bit of MR,such as **< No Delayed Ack(TCP) flag.
*@return  1 for sucess else 0.
*/
uint8_t uchFN_W5500SocketInit(SOCKET s, uint8_t much_protocol, uint16_t mun_port, uint8_t mun_flag)
{
   uint8_t much_ret;
   if ( ((much_protocol&0x0F) == Sn_MR_TCP)    ||
        ((much_protocol&0x0F) == Sn_MR_UDP)    ||
        ((much_protocol&0x0F) == Sn_MR_IPRAW)  ||
        ((much_protocol&0x0F) == Sn_MR_MACRAW) ||
        ((much_protocol&0x0F) == Sn_MR_PPPOE)
      )
   {
      vFN_W5500_SocketClose(s);//关闭secket 通道
      vFN_W5500_Write1Baye_IO(Sn_MR(s) ,much_protocol | mun_flag);//写socket Mode register, W5500控制方式
      if (mun_port != 0)
      {
          vFN_W5500_Write2Baye_IO( Sn_PORT0(s),mun_port);//设置本地端口
      }
      else 
      {
          gun_local_port++; // if don't set the source port, set local_port number.
          vFN_W5500_Write2Baye_IO( Sn_PORT0(s),gun_local_port);
      }
      vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_OPEN); // run sockinit Sn_CR，initialize or open socket

      /* wait to process the command... */
      while( uchFN_W5500_Read1Baye_IO(Sn_CR(s))); // Sn_CR 写成功后改变的是Sn_SR的值，读出来是0; 
      /* ------- */
      much_ret = 1;
   }
   else
   {
      much_ret = 0;
   }
   return much_ret;
}


/**
*@brief   This function close the socket and parameter is "s" which represent the socket number
*@param		s: socket number.
*@return  None
*/
void vFN_W5500_SocketClose(SOCKET s)
{

   vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_CLOSE);

   /* wait to process the command... */
   while( uchFN_W5500_Read1Baye_IO(Sn_CR(s) ) )
       ;/* ------- */
   
	vFN_W5500_Write1Baye_IO( Sn_IR(s) , 0xFF);	 /* all clear */
}


/**
*@brief   This function established  the connection for the channel in passive (server) mode. 
					This function waits for the request from the peer.
*@param		s: socket number.
*@return  1 for success else 0.
*/
uint8_t uchFN_W5500SocketListen(SOCKET s)
{
   uint8_t much_ret;
   if (uchFN_W5500_Read1Baye_IO( Sn_SR(s) ) == SOCK_INIT)
   {
      vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_LISTEN);
      /* wait to process the command... */
      while( uchFN_W5500_Read1Baye_IO(Sn_CR(s)));
      /* ------- */
      much_ret = 1;
   }
   else
   {
      much_ret = 0;
   }
   return much_ret;
}


/**
*@brief		This function established  the connection for the channel in Active (client) mode.
					This function waits for the untill the connection is established.
*@param		s: socket number.
*@param		addr: The server IP address to connect
*@param		port: The server IP port to connect
*@return  1 for success else 0.
*/
uint8_t uchFN_W5500SocketConnect(SOCKET s, uint8_t * muchp_addr, uint16_t mun_port)
{
    uint8_t much_ret;
    if ( *((uint32_t *)&muchp_addr[0]) == 0xFFFFFFFF ||
         *((uint32_t *)&muchp_addr[0]) == 0U ||
          (mun_port == 0x00))
    {
        much_ret = 0;
    }
    else
    {
        much_ret = 1;
        // set destination IP        
        unFN_W5500WriteBuf_IO( Sn_DIPR0(s),muchp_addr,4);//设置IP
        vFN_W5500_Write2Baye_IO( Sn_DPORT0(s),mun_port);//设置端口
        vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_CONNECT);//send connection request in tcp mode(Client mode)
        /* wait for completion */
        while ( uchFN_W5500_Read1Baye_IO(Sn_CR(s) ) ) ;// Sn_CR 写成功后改变的是Sn_SR的值，读出来是0; 

        while ( uchFN_W5500_Read1Baye_IO(Sn_SR(s)) != SOCK_SYNSENT )//connection state, 已经发了请求，循环判断
        {
            if(uchFN_W5500_Read1Baye_IO(Sn_SR(s)) == SOCK_ESTABLISHED)//success to connect
            {
                break;
            }
            if (uchFN_W5500getSn_IR(s) & Sn_IR_TIMEOUT)//超时 
            {
                vFN_W5500_Write1Baye_IO(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
                much_ret = 0;
                break;
            }
        }
    }
   return much_ret;
}

/**
*@brief   This function used for disconnect the socket s
*@param		s: socket number.
*@return  1 for success else 0.
*/
void vFN_W5500SocketDisconnect(SOCKET s)
{
   vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_DISCON);

   /* wait to process the command... */
   while( uchFN_W5500_Read1Baye_IO(Sn_CR(s) ) ) ;
   /* ------- */
}

/**
*@brief   This function used to send the data in TCP mode
*@param		s: socket number.
*@param		buf: data buffer to send.
*@param		len: data length.
*@return  1 for success else 0.
*/
uint16_t unFN_W5500SocketSend(SOCKET s, const uint8_t * muchp_buf, uint16_t mun_len)
{
    uint8_t much_status=0;
    uint16_t mun_ret=0;
    uint16_t mun_freesize=0;

    if (mun_len > unFN_getSockTxSize(s)*1000) mun_ret = unFN_getSockTxSize(s); // check size not to exceed MAX size.
    else mun_ret = mun_len;

  // if freebuf is available, start.
    do
    {
        mun_freesize = unFN_W5500getSn_TX_FSR(s);
        much_status = uchFN_W5500_Read1Baye_IO(Sn_SR(s));
        if ((much_status != SOCK_ESTABLISHED) && (much_status != SOCK_CLOSE_WAIT))
        {
            mun_ret = 0;
            break;
        }
    } while (mun_freesize < mun_ret);
  
    // copy data
    vFN_W5500Tx_Pro(s, (uint8_t *)muchp_buf, mun_ret);
    vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_SEND);

  /* wait to process the command... */
    while( uchFN_W5500_Read1Baye_IO(Sn_CR(s) ) );

    while ( (uchFN_W5500_Read1Baye_IO(Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
    {
        much_status = uchFN_W5500_Read1Baye_IO(Sn_SR(s));
        if ((much_status != SOCK_ESTABLISHED) && (much_status != SOCK_CLOSE_WAIT) )
        {
            printf("SEND_OK Problem!!\r\n");
            vFN_W5500_SocketClose(s);
            return 0;
        }
    }
    vFN_W5500_Write1Baye_IO( Sn_IR(s) , Sn_IR_SEND_OK);
    vFN_W5500_Write1Baye_IO( Sn_IR(s) , Sn_IR_SEND_OK);//写两次Sn_IR
    return mun_ret;
}

/**
*@brief		This function is an application I/F function which is used to receive the data in TCP mode.
            It continues to wait for data as much as the application wants to receive.
*@param		s: socket number.
*@param		buf: data buffer to receive.
*@param		len: data length.
*@return  received data size for success else 0.
*/
uint16_t unFN_W5500SocketRecv(SOCKET s, uint8_t * muchp_buf, uint16_t mun_len)
{
   uint16_t mun_ret=0;
   if ( mun_len > 0 )
   {
      vFN_W5500Rx_Pro(s, muchp_buf, mun_len);
      vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_RECV);
      /* wait to process the command... */
      while( uchFN_W5500_Read1Baye_IO(Sn_CR(s) ));
      /* ------- */
      mun_ret = mun_len;
   }
   return mun_ret;
}

/**
*@brief   This function is an application I/F function which is used to send the data for other then TCP mode.
          Unlike TCP transmission, The peer's destination address and the port is needed.
*@param		s: socket number.
*@param		buf: data buffer to send.
*@param		len: data length.
*@param		addr: IP address to send.
*@param		port: IP port to send.
*@return  This function return send data size for success else 0.
*/
uint16_t unFN_W5500SocketSendUDP(SOCKET s, const uint8 * buf, uint16_t mun_len, uint8 * muchp_addr, uint16_t mun_port)
{
   uint16_t mun_ret=0;

   if (mun_len > unFN_getSockTxSize(s)) 
   mun_ret = unFN_getSockTxSize(s); // check size not to exceed MAX size.
   else mun_ret = mun_len;
    
   if( *((uint32 *)&muchp_addr[0]) == 0 || ((mun_port == 0x00)) )//||(ret == 0) )
   {
      /* added return value */
      mun_ret = 0;
   }
   else
   {
      unFN_W5500WriteBuf_IO(Sn_DIPR0(s),muchp_addr,4);//设置IP
      vFN_W5500_Write2Baye_IO( Sn_DPORT0(s),mun_port);//设置端口
      // copy data
      vFN_W5500Tx_Pro(s, (uint8 *)buf, mun_ret);
      vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_SEND);
      /* wait to process the command... */
      while( uchFN_W5500_Read1Baye_IO( Sn_CR(s) ) )
	  ;
      /* ------- */
     while( (uchFN_W5500_Read1Baye_IO( Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
     {
      if (uchFN_W5500_Read1Baye_IO( Sn_IR(s) ) & Sn_IR_TIMEOUT)
      {
            /* clear interrupt */
      vFN_W5500_Write1Baye_IO( Sn_IR(s) , (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
      return 0;
      }
     }
      vFN_W5500_Write1Baye_IO( Sn_IR(s) , Sn_IR_SEND_OK);
   }
   return mun_ret;
}

/**
*@brief   This function is an application I/F function which is used to receive the data in other then
					TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.
*@param		s: socket number.
*@param		buf: data buffer to receive.
*@param		len: data length.
*@param		addr: IP address to receive.
*@param		port: IP port to receive.
*@return	This function return received data size for success else 0.
*/
uint16_t unFN_W5500SocketRecvfrom(SOCKET s, uint8_t * muchp_buf, uint16_t mun_len, uint8_t * muchp_addr, uint16_t *munp_port)
{
   uint8_t much_head[8];
   uint16_t mun_data_len=0;
   uint16_t mun_ptr=0;
   uint32_t mui_addr =0;
   if ( mun_len > 0 )
   {
      mun_ptr = unFN_W5500_Read2Baye_IO(Sn_RX_RD0(s));     
      mui_addr = (uint32_t)(mun_ptr<<8) + (s<<5) + 0x18;
      
      switch (uchFN_W5500_Read1Baye_IO(Sn_MR(s) ) & 0x07)
      {
      case Sn_MR_UDP :
            unFN_W5500ReadBuf_IO(mui_addr, much_head, 0x08);        
            mun_ptr += 8;
            // read peer's IP address, port number.
          
            *((uint32_t *)&muchp_addr[0]) = *((uint32_t *)&much_head[0]);
      
            ((uint8_t *)&munp_port)[1] = much_head[4];
            ((uint8_t *)&munp_port)[0] = much_head[5];
      
            ((uint8_t *)&mun_data_len)[1] = much_head[6];
            ((uint8_t *)&mun_data_len)[0] = much_head[7];

            mui_addr = (uint32_t)(mun_ptr<<8) + (s<<5) + 0x18;
            unFN_W5500ReadBuf_IO(mui_addr, muchp_buf, mun_data_len);                
            mun_ptr += mun_data_len;

            vFN_W5500_Write2Baye_IO( Sn_RX_RD0(s), mun_ptr);         
            break;

      case Sn_MR_IPRAW :
            unFN_W5500ReadBuf_IO(mui_addr, much_head, 0x06);
                   
            mun_ptr += 6;
            *((uint32_t *)&muchp_addr[0]) = *((uint32_t *)&much_head[0]);
            ((uint8_t *)&mun_ptr)[1] = much_head[4];
            ((uint8_t *)&mun_ptr)[0] = much_head[5];

            mui_addr  = (uint32_t)(mun_ptr<<8) +  (s<<5) + 0x18;
           
            unFN_W5500ReadBuf_IO(mui_addr, muchp_buf, mun_data_len);
            mun_ptr += mun_data_len;

            vFN_W5500_Write2Baye_IO( Sn_RX_RD0(s), mun_ptr);
            
            break;

      case Sn_MR_MACRAW :
            unFN_W5500ReadBuf_IO(mui_addr, much_head, 0x02);
            mun_ptr+=2;
      
            ((uint8_t *)&mun_data_len)[1] = much_head[0];
            ((uint8_t *)&mun_data_len)[0] = much_head[1] - 2;
        
            if(mun_data_len > 1514)
            {
               printf("data_len over 1514\r\n");
               while(1);
            }

            mui_addr  = (uint32)(mun_ptr<<8) +  (s<<5) + 0x18;
            unFN_W5500ReadBuf_IO(mui_addr, muchp_buf, mun_data_len);
            mun_ptr += mun_data_len;

            vFN_W5500_Write2Baye_IO( Sn_RX_RD0(s), mun_ptr);
            break;

      default :
            break;
      }
      vFN_W5500_Write1Baye_IO( Sn_CR(s) ,Sn_CR_RECV);

      /* wait to process the command... */
      while( uchFN_W5500_Read1Baye_IO( Sn_CR(s)) ) ;
      /* ------- */
   }
   return mun_data_len;
}


