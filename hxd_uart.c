/*
 * hxd_uart.c
 *
 *  Created on: 2016��11��14��
 *      Author: houxd
 */

#include "hxd_uart.h"
#include "hx_device.h"
#include "hx_utils.h"
#include "stdio.h"
#include "string.h"
#include "stddef.h"


//----------------------------------------------------------------
int hxd_uart_open(HX_DEV *dev,const char *param)
{
	/*
	 * param : "115200,8N1" like this
	 */
	const UART_DEV_T *uart = (const UART_DEV_T *)(dev->pdev);
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	int bps,bit;
	char stop;
	bps = uart->default_bps;
	if(param)
		sscanf(param,"%u,%d%c",&bps,&bit,&stop);
	return drv->open(uart,bps);
}
static int uart_getc_noblock(const UART_DEV_T *uart,int *c)
{
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	UART_PRIVATE_T *prv = uart->prv;
	int res = -1;
	drv->interrupt_ctrl(uart,0);
	if(prv->rx_pos>0){
		if(prv->rx_read<prv->rx_pos){
			*c = (int)(uart->rxbuf[prv->rx_read]);
			prv->rx_read++;
			res = 0;
		}else{
			prv->rx_pos = 0;
			prv->rx_read = 0;
		}
	}
	drv->interrupt_ctrl(uart,1);
	return res;
}
int hxd_uart_read(HX_DEV *dev,void *buf,int _size)
{
	const UART_DEV_T *uart = (const UART_DEV_T *)(dev->pdev);
	int res,c;
	int size = _size;
	unsigned char *p = buf;
	if(size<=0)
		return -2;
	int i;
	for(i=0;i<size;i++){
		res = uart_getc_noblock(uart,&c);
		if(res==0)
			p[i] = c;
		else
			return i;
	}
	return size;
}
static void wait_last_trans_complete(const UART_DEV_T *uart)
{
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	int pos,wr;
	do{
		drv->interrupt_ctrl(uart,0);
		pos = uart->prv->tx_pos;
		wr = uart->prv->tx_wtire;
		drv->interrupt_ctrl(uart,1);
	}while(wr&&(pos!=wr));

}
int hxd_uart_write(HX_DEV *dev,const void *buf, int size)
{
	UART_DEV_T *uart = (UART_DEV_T*)(dev->pdev);
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	//check len big than buff
	int n = size;
	if(n==0){
		wait_last_trans_complete(uart);
		return 0;
	}
	while(n>uart->txbuf_size){
		wait_last_trans_complete(uart);
		//start a new session
		memcpy((void*)(uart->txbuf),buf,uart->txbuf_size);
		uart->prv->tx_wtire = uart->txbuf_size;
		uart->prv->tx_pos = 0;
		drv->tx_start_byte(uart,uart->txbuf[0]);
		n -= uart->txbuf_size;
	}
	wait_last_trans_complete(uart);
	//start a new session
	memcpy((void*)(uart->txbuf),buf,n);
	uart->prv->tx_wtire = n;
	uart->prv->tx_pos = 0;
	drv->tx_start_byte(uart,uart->txbuf[0]);
	return size;
}
int hxd_uart_close(HX_DEV *dev)
{
	UART_DEV_T *uart = (UART_DEV_T*)(dev->pdev);
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	return drv->close(uart);
}

void hxd_uart_rx_byte(const UART_DEV_T *uart, int _byte)
{
	//UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	UART_PRIVATE_T *prv = uart->prv;
	if(prv->rx_pos<uart->rxbuf_size)
	{
		uart->rxbuf[prv->rx_pos] = _byte;
		prv->rx_pos ++;
	} else {
		prv->rx_pos = 0;
		prv->rx_read = 0;
	}
}
void hxd_uart_tx_byte(const UART_DEV_T *uart)
{
	UART_DRV_T *drv = (UART_DRV_T*)(uart->dev.driver);
	UART_PRIVATE_T *prv = uart->prv;
	prv->tx_pos ++;
	if(prv->tx_wtire && prv->tx_pos<prv->tx_wtire){
		drv->tx_start_byte(uart,uart->txbuf[prv->tx_pos]);
	}else{
		prv->tx_wtire = 0;
		drv->tx_end_session(uart);
	}
}

int hx_register_uart_device(const UART_DEV_T *dev)
{

	return hx_register_char_device((DEV_T*)dev);

}


