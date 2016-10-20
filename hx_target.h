/*
 * hx_port.h
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#ifndef __HX_TARGET_H__
#define __HX_TARGET_H__

//=====================================================
#define DQ1200			(1)
#define DQ1300			(2)
#define VD7400			(3)
#define VD7200			(4)

/*
	config 
*/
#define HX_TARGET_BOARD		(VD7200)


#if HX_TARGET_BOARD == VD7200
#include "vd7200.h"
#endif
#if HX_TARGET_BOARD == VD7400
#include "vd7400.h"
#endif

//====================================================
//default configs

#include "hx_serial.h"
#include "int.h"
//=====================================================
//PORTS OF UART
/*
	these two proc is usefor recv & send uart data 
	please call it at uart interrupt function
*/
extern void UART_RX_BYTE(int id, int data);
extern void UART_TX_BYTE(int id);

/*
	if define below as zero ,that is to disbale this port.
*/
#ifndef UART_PORTS_NUM
//DQ1200
//#define UART0_RX_BUFF_SIZE			(2048)
//#define UART0_TX_BUFF_SIZE			(2048)
//#define UART1_RX_BUFF_SIZE			(0)
//#define UART1_TX_BUFF_SIZE			(0)
//#define UART2_RX_BUFF_SIZE			(0)
//#define UART2_TX_BUFF_SIZE			(0)
//#define UART3_RX_BUFF_SIZE			(512)
//#define UART3_TX_BUFF_SIZE			(512)
//DQ1300
#define UART0_RX_BUFF_SIZE			(128)
#define UART0_TX_BUFF_SIZE			(128)
#define UART1_RX_BUFF_SIZE			(0)
#define UART1_TX_BUFF_SIZE			(0)
#define UART2_RX_BUFF_SIZE			(128)
#define UART2_TX_BUFF_SIZE			(128)
#define UART3_RX_BUFF_SIZE			(0)
#define UART3_TX_BUFF_SIZE			(0)

#define UART_PORTS_NUM				(4)

#endif

#if UART_PORTS_NUM>4
	#error current version can not surpport uart count more than 4
#endif

//=====================================================
//PORT of timer tick count 
/*
	call this func in timer 
	n is interval in ms
*/
extern void TICK_COUNT_INC(int n);

//=====================================================
//PORTS OF DEBUG
/*
	config debug info output uart index
*/
#ifndef UART_DEBUG_PORT
#define UART_DEBUG_PORT		(3)
#endif
//=====================================================
//PORTS OF TERMINAL
/*
	config debug info output uart index
*/
#ifndef UART_TERMINAL_PORT
#define UART_TERMINAL_PORT	(3)
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

#endif

