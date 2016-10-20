#ifndef __HX_BOARD_H__
#define __HX_BOARD_H__

#include "hx_cpu.h"

/*
	define default board configs
*/
#define HX_DEBUG_PORT		(-1)

//======================================================
//uart
#ifndef UART_PORTS_NUM
#define UART_PORTS_NUM				(4)
#define UART0_RX_BUFF_SIZE			(2048)
#define UART0_TX_BUFF_SIZE			(2048)
#define UART1_RX_BUFF_SIZE			(0)
#define UART1_TX_BUFF_SIZE			(0)
#define UART2_RX_BUFF_SIZE			(0)
#define UART2_TX_BUFF_SIZE			(0)
#define UART3_RX_BUFF_SIZE			(512)
#define UART3_TX_BUFF_SIZE			(512)
#endif
#if UART_PORTS_NUM>4
	#error current version can not surpport uart count more than 4
#endif
	
//=====================================================
//PORTS OF DEBUG
/*
	config debug info output uart index
*/
#ifndef UART_DEBUG_PORT
#define UART_DEBUG_PORT		(-1)
#endif
//=====================================================
//PORTS OF TERMINAL
/*
	config debug info output uart index
*/
#ifndef UART_TERMINAL_PORT
#define UART_TERMINAL_PORT	(-1)
#endif
//=====================================================
//PORTS OF AT CMD PORT
#ifndef UART_AT_PORT
#define UART_AT_PORT		(0)
#endif
//=====================================================
// uart_printf use vsprintf, need buffer data
#ifndef VSPRINTF_BUFF_SIZE
#define VSPRINTF_BUFF_SIZE	(128)
#endif

/*
	this file define the board procs
*/


extern void brd_uart_tx_start_byte(int id,int data);
extern void brd_uart_tx_end_session(int id);
extern int brd_uart_reopen(int id, int bps, int pclk);

extern int brd_dp_rxclr(int id);
extern int brd_dp_getc_noblock(int id,int *c);
extern int brd_dp_send(int id,const char* s, int len);


int brd_param_loadstore(int load0store1);



#endif

