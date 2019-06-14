#ifndef	_SOCKET_H_
#define	_SOCKET_H_

#include "stm32f1xx_hal.h"
#include "types.h"

/*Socket 端口选择，可按自己的习惯定义*/
#define SOCK_TCPS             0
#define SOCK_HUMTEM			  0
#define SOCK_PING			  0
#define SOCK_TCPC             1
#define SOCK_UDPS             2
#define SOCK_WEIBO      	  2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_SMTP             6
#define SOCK_NTP              7
//#define NETBIOS_SOCK    6 //在netbios.c已定义

extern uint8_t uchFN_W5500SocketInit(SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void vFN_W5500_SocketClose(SOCKET s); // Close socket
extern uint8_t uchFN_W5500SocketConnect(SOCKET s, uint8_t * addr, uint16_t port); // Establish TCP connection (Active connection)
extern void vFN_W5500SocketDisconnect(SOCKET s); // disconnect the connection
extern uint8_t uchFN_W5500SocketListen(SOCKET s);	// Establish TCP connection (Passive connection)
extern uint16_t unFN_W5500SocketSend(SOCKET s, const uint8_t * buf, uint16_t len); // Send data (TCP)
extern uint16_t unFN_W5500SocketRecv(SOCKET s, uint8_t * buf, uint16_t len);	// Receive data (TCP)
extern uint16_t unFN_W5500SocketSendUDP(SOCKET s, const uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port); // Send data (UDP/IP RAW)
extern uint16_t unFN_W5500SocketRecvfrom(SOCKET s, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t  *port); // Receive data (UDP/IP RAW)



#endif
/* _SOCKET_H_ */

