/*******************************************************************
 *    TCPS.,Co. (c) Copyright 2008
 *
 *    FILENAME:
 *
 *    DESCRIPTION: 程序要用到的定义
 *
 *    AUTHOR: Tiger.gao
 *
 *    HISTORY: 2008-07-18
 *
 *******************************************************************/
#include "LPC23xx.h"                       /* LPC23xx/24xx definitions */
#include "TK_Typedef.h"
#ifndef __UART_H
#define __UART_H
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04
#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01
#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80
//*************************************************
//com const value
//*************************************************
//串口接收命令状态
#define UART_RC_PERMINT 0   //允许接收
#define UART_RC_SUCCESS 1   //接收成功
#define UART_RC_XOR_ERROR 2  //接收到的命令数据异或错误

#define UART_TR_BEGIN 0   //发送开始
#define UART_TR_FINISH 1   //发送完成

#define UART_RC_HEADER 0xaa
#define UART_RC_HEADER_CLR 0x00

#define UART0_NUM 0
#define UART1_NUM 1
#define UART2_NUM 2
#define UART3_NUM 3  //串口号              
#define UART0_RC_BUFSIZE 0x220
#define UART0_TR_BUFSIZE 0x220
#define UART1_RC_BUFSIZE 0x100
#define UART1_TR_BUFSIZE 0x100
#define UART2_RC_BUFSIZE 0x20
#define UART2_TR_BUFSIZE 0x20
#define UART3_RC_BUFSIZE 0x220
#define UART3_TR_BUFSIZE 0x220//串口接收大小   


#define tkUART_BAUD_RATE19200 19200
#define tkUART_BAUD_RATE38400 38400
#define tkUART_BAUD_RATE57600 57600
#define tkUART_BAUD_RATE115200 115200
extern volatile tk_uint8 ucUART0_RC_BUF[UART0_RC_BUFSIZE];
extern volatile tk_int16 iUART0_RC_LEN;
extern volatile tk_uint8 ucUART0_TR_BUF[UART0_TR_BUFSIZE];
extern volatile tk_int16 iUART0_TR_LEN;
extern volatile tk_int16 iUART0_CURR_TR_LEN;
extern volatile tk_uint8 ucUART1_RC_BUF[UART1_RC_BUFSIZE];
extern volatile tk_int16 iUART1_RC_LEN;
extern volatile tk_uint8 ucUART1_TR_BUF[UART1_TR_BUFSIZE];
extern volatile tk_int16 iUART1_TR_LEN;
extern volatile tk_int16 iUART1_CURR_TR_LEN;
extern volatile tk_uint8 ucUART2_RC_BUF[UART2_RC_BUFSIZE];
extern volatile tk_int16 iUART2_RC_LEN;
extern volatile tk_uint8 ucUART2_TR_BUF[UART2_TR_BUFSIZE];
extern volatile tk_int16 iUART2_TR_LEN;
extern volatile tk_int16 iUART2_CURR_TR_LEN;
extern volatile tk_uint8 ucUART3_RC_BUF[UART3_RC_BUFSIZE];
extern volatile tk_int16 iUART3_RC_LEN;
extern volatile tk_uint8 ucUART3_TR_BUF[UART3_TR_BUFSIZE];
extern volatile tk_int16 iUART3_TR_LEN;
extern volatile tk_int16 iUART3_CURR_TR_LEN;

extern tk_uint32 UART_Init( tk_uint32 portNum, tk_uint32 Baudrate );
extern void UART_SendTrBuf(tk_int16 iPortNum);

extern void WriteCom(tk_int16 iPortNum,tk_uint8 *ucpTrData,tk_int16 iTrLen);
extern tk_int16 ReadCom(tk_int16 iPortNum,tk_uint8 *ucpRcData);
//extern void SELECT_RSAorIRDA(tk_uint8 RSA);
extern void EnableUart(tk_uint8 portnum);


#define MAX_UART0_RX_BUFFER_SIZE	(2048)
extern volatile int g_uart0_rx_pos;
extern volatile char g_uart0_rx_buffer[MAX_UART0_RX_BUFFER_SIZE];
extern volatile tk_int16 uart0_recv_read_pos;


#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
