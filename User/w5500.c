/**
******************************************************************************
* @file    		W5500.c
* @author  		胡鹏 
* @version 		V1.0
* @date    		2019-04-14
* @brief   		读写W5500寄存器底层函数
******************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"

uint8_t guch_txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2}; // Tx buffer ，加起来必须小于16
uint8_t guch_rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2}; // Rx buffer ，加起来必须小于16

/**
*@brief		This function is to get the Max size to receive.
*@param		s: socket number
*@return	
*/
void vFN_W5500Reset(void)
{
    vFN_W5500setMR( MR_RST );  //软件复位
}

/**
*@brief		This function is to get the Max size to receive.
*@param		s: socket number
*@return	This function return the RX buffer size has been setted
*/
uint8_t unFN_getSockRxSize(SOCKET s)
{
	return guch_rxsize[s];
}	

/**
*@brief		This function is to get the Max size to transport.
*@param		s: socket number
*@return	This function return the TX buffer size has been setted
*/
uint8_t unFN_getSockTxSize(SOCKET s)
{
	return guch_txsize[s];
}

/**
*@brief		This function is to set the MR register.
*@param		val: the value to set to MR  
*@return	None
*/
void vFN_W5500setMR(uint8_t val)
{
    vFN_W5500_Write1Baye_IO(MR,val);
}

/**
*@brief		This function is to set up gateway IP address.
*@param		addr: a pointer to a 4 -byte array responsible to set the Gateway IP address 
*@return	None
*/
void vFN_W5500setGAR(uint8_t * addr )
{
    unFN_W5500WriteBuf_IO(GAR0, addr, 4);
}

/**
*@brief		This function is to get gateway IP address.
*@param		addr: a pointer to a 4 -byte array responsible to get the Gateway IP address  
*@return	None
*/
void vFN_W5500getGAR(uint8_t * addr)
{
    unFN_W5500ReadBuf_IO(GAR0, addr, 4);
}

/**
*@brief 	This function is to set up SubnetMask address
*@param		addr: a pointer to a 4 -byte array responsible to set the subway IP address.  
*@return	None
*/
void vFN_W5500setSUBR(uint8_t * addr)
{   
    unFN_W5500WriteBuf_IO(SUBR0, addr, 4);
}

/**
*@brief		This function is to get Subnet mask.
*@param		addr:a pointer to a 4 -byte array responsible to set the Subnet mask.  
*@return	None
*/
void vFN_W5500getSUBR(uint8_t * addr)
{
    unFN_W5500ReadBuf_IO(SUBR0, addr, 4);
}

/**
*@brief		This function is to set up MAC address.
*@param		addr: a pointer to a 6 -byte array responsible to set the MAC address.  
*@return	None
*/
void vFN_W5500setSHAR(uint8_t * addr)
{
    unFN_W5500WriteBuf_IO(SHAR0, addr, 6);  
}

/**
*@brief		This function is to get up Source MAC .
*@param		addr: a pointer to a 6 -byte array responsible to get the MAC  
*@return	None
*/
void vFN_W5500getSHAR(uint8_t * addr)
{
    unFN_W5500ReadBuf_IO(SHAR0, addr, 6);
}

/**
*@brief		This function is to set up Source IP address.
*@param		addr:a pointer to a 4 -byte array responsible to set the Source IP addres.  
*@return	None
*/
void vFN_W5500setSIPR(uint8_t * addr)
{
    unFN_W5500WriteBuf_IO(SIPR0, addr, 4);  
}

/**
*@brief		This function is to get up Source IP .
*@param		addr: a pointer to a 4 -byte array responsible to get the Source IP  
*@return	None
*/
void vFN_W5500getSIPR(uint8_t * addr)
{
    unFN_W5500ReadBuf_IO(SIPR0, addr, 4);
}


/**
*@brief		This function is to get Interrupt register in common register.
*@param		None  
*@return	The value read from the IR register
*/
uint8_t uchFN_W5500getIR( void )
{
   return uchFN_W5500_Read1Baye_IO(IR);
}

/**
*@brief		This function is to the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)
            If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
            set in IR register.
*@param		mask: the bits to clear
*@return	None
*/
void vFN_W5500clearIR(uint8_t mask)
{
    vFN_W5500_Write1Baye_IO(IR, ~mask | uchFN_W5500getIR() );  // must be setted 0x10.
}

/**
@brief		This function is to set up Retransmission time.
            If there is no response from the peer or delay in response then retransmission
            will be there as per RTR (Retry Time-value Register)setting
*@param		timeout: The value write to  the RTR0 register 
*@return	None
*/
void vFN_W5500setRTR(uint16 timeout)
{
    vFN_W5500_Write2Baye_IO( RTR0,timeout);
}

/**
@brief		This function is to set the number of Retransmission.
            If there is no response from the peer or delay in response then recorded time
            as per RTR & RCR register seeting then time out will occur.
*@param		retry: Times to  retry 
*@return	None
*/
void vFN_W5500setRCR(uint8_t retry)
{
    vFN_W5500_Write1Baye_IO(WIZ_RCR,retry);
}



/**
*@brief  	This function is to set the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*@param		s: socket number
*@param		Sn_MSSR: the maximum segment size
*@return	None
*/
void vFN_W5500setSn_MSS(SOCKET s, uint16 Sn_MSSR)
{
    vFN_W5500_Write2Baye_IO( Sn_MSSR0(s),Sn_MSSR);//更新Sn_MSSR寄存器的值      
}

/**
*@brief  	This function is to set the IP Time to live(TTL) Register
*@param		s: socket number
*@param		Sn_MSSR: the IP Time to live
*@return	None
*/
void vFN_W5500setSn_TTL(SOCKET s, uint8_t ttl)
{    
   vFN_W5500_Write1Baye_IO( Sn_TTL(s) , ttl);
}

/**
*@brief		This function is to read the Interrupt & Soket Status registe
*@param		s: socket number
*@return	socket interrupt status
*/
uint8_t uchFN_W5500getSn_IR(SOCKET s)
{
   return uchFN_W5500_Read1Baye_IO(Sn_IR(s));
}

/**
*@brief 	This function is to write the Interrupt & Soket Status register to clear the interrupt
*@param		s: socket number
*@return  socket interrupt status
*/
void vFN_W5500setSn_IR(uint8_t s, uint8_t val)
{
    vFN_W5500_Write1Baye_IO(Sn_IR(s), val);
}

/**
*@brief 	This function is to get socket status
*@param		s: socket number
*@return  socket status
*/
uint8_t uchFN_W5500getSn_SR(SOCKET s)
{
   return uchFN_W5500_Read1Baye_IO(Sn_SR(s));
}

/**
*@brief		This fuction is to get socket TX free buf size
            This gives free buffer size of transmit buffer. This is the data size that user can transmit.
            User shuold check this value first and control the size of transmitting data
*@param		s: socket number
*@return  socket TX free buf size
*/
uint16_t unFN_W5500getSn_TX_FSR(SOCKET s)
{
    uint16_t val=0,val1=0;
    do
    {
        val1 = unFN_W5500_Read2Baye_IO(Sn_TX_FSR0(s));//读Sn_TX_FSR的值           
        if (val1 != 0)
        {
            val = unFN_W5500_Read2Baye_IO(Sn_TX_FSR0(s));//读Sn_TX_FSR的值    
        }
    } while (val != val1);//读两次值，对比相等则退出
    return val;//将val的值返回给getSn_TX_FSR
}

/**
*@brief		This fuction is to give size of received data in receive buffer.
            获取空闲接收缓存寄存器的值
*@param		s: socket number
*@return    socket TX free buf size
*/
uint16_t unFN_W5500getSn_RX_RSR(SOCKET s)
{
    uint16_t val=0,val1=0;
    do
    {
        val1 = unFN_W5500_Read2Baye_IO(Sn_RX_RSR0(s));//读Sn_RX_RSR的值
        if(val1 != 0)// 若Sn_RX_RSR的值不为0，将其赋给val
        {
            val = unFN_W5500_Read2Baye_IO(Sn_RX_RSR0(s));//读Sn_RX_RSR的值
        }
    } while (val != val1);//读两次值，对比相等则退出
    return val;//将val的值返回给getSn_RX_RSR
}

/**
*@brief   This function is being called by send() and sendto() function also.
        This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
        register. User should read upper byte first and lower byte later to get proper value.
*@param		s: socket number
*@param		data: data buffer to send
*@param		len: data length
*@return  socket TX free buf size
*/
void vFN_W5500Tx_Pro(SOCKET s, uint8_t *much_data, uint16_t mun_len)
{
    uint16_t mun_ptr = 0;
    uint32_t mui_addr = 0;
    if(mun_len > 0)
    {
        // MCU读取Sn_RX_RD接收写指针寄存器的值，并赋给ptr
        // Sn_RX_RD保存接收缓存中数据的首地址，若有数据接收，则接收完后该寄存器值要更新
        mun_ptr = unFN_W5500_Read2Baye_IO(Sn_TX_WR0(s));

        mui_addr = (uint32)(mun_ptr<<8) + (s<<5) + 0x10;//获取接收到的数据的绝对地址
        unFN_W5500WriteBuf_IO(mui_addr, much_data, mun_len);//读绝对地址的数据到接收缓存
        mun_ptr += mun_len;
        
        vFN_W5500_Write2Baye_IO( Sn_TX_WR0(s),mun_ptr);// 更新Sn_TX_WR寄存器的值     
    }
}

/**
*@brief  	This function is being called by recv() also.
            This function read the Rx read pointer register
            and after copy the data from receive buffer update the Rx write pointer register.
            User should read upper byte first and lower byte later to get proper value.
*@param		s: socket number
*@param		much_data: data buffer to receive
*@param		len: data length
*@return  None
*/
void vFN_W5500Rx_Pro(SOCKET s, uint8_t *much_data, uint16_t mun_len)
{
    uint16_t mun_ptr = 0;
    uint32_t mui_addr = 0;

    if(mun_len > 0)
    {
        // MCU读取Sn_RX_RD接收写指针寄存器的值，并赋给ptr
        // Sn_RX_RD保存接收缓存中数据的首地址，若有数据接收，则接收完后该寄存器值要更新
        mun_ptr = unFN_W5500_Read2Baye_IO(Sn_RX_RD0(s));

        mui_addr = (uint32)(mun_ptr<<8) + (s<<5) + 0x18;//获取接收到的数据的绝对地址
        unFN_W5500ReadBuf_IO(mui_addr, much_data, mun_len);//读绝对地址的数据到接收缓存-much_data
        mun_ptr += mun_len;//

        vFN_W5500_Write2Baye_IO( Sn_RX_RD0(s),mun_ptr);// 更新Sn_RX_RD寄存器的值
    }
}

/**
*@brief		This function set the transmit & receive buffer size as per the channels is used
*@Note: 	TMSR and RMSR bits are as follows\n
            Maximum memory size for Tx, Rx in the W5500 is 16K Bytes,\n
            In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
            Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
            and to data transmission and receiption from non-allocated channel may cause some problems.\n
            If the 16KBytes memory is already  assigned to centain channel, \n
            other 3 channels couldn't be used, for there's no available memory.\n
            If two 4KBytes memory are assigned to two each channels, \n
            other 2 channels couldn't be used, for there's no available memory.\n

            W5500有8个Socket，每个Socket有对应独立的收发缓存区。
            每个Socket的发送/接收缓存区都在一个16KB的物理发送内存中，初始化分配为2KB。
            无论给每个Socket分配多大的收/发缓存，都必须在16KB以内。

*@param		tx_size: tx buffer size to set=tx_size[s]*(1024)
*@param		rx_size: rx buffer size to set=rx_size[s]*(1024)
*@return	None
*/
void vFN_W5500SocketBufInit(uint8_t * tx_size, uint8_t * rx_size  )
{
    uint8_t i;
    for (i = 0 ; i < MAX_SOCK_NUM; i++)// Set the size, masking and base address of Tx & Rx memory by each channel
    {
        vFN_W5500_Write1Baye_IO( (Sn_TXMEM_SIZE(i)), tx_size[i]);
        vFN_W5500_Write1Baye_IO( (Sn_RXMEM_SIZE(i)), rx_size[i]);
    }
}







