/*
 * hx_atcmd.h
 *
 *  Created on: 2016Äê8ÔÂ15ÈÕ
 *      Author: houxd
 */

#ifndef __AT_PORT_H__
#define __AT_PORT_H__

#include "hx_target.h"
#include "hx_serial.h"


/*
	usefor call back function, indicate a event.
*/
enum ATEVENT_T {AT_PUT,AT_GET};

/*
	define cmd list item struct 
	if cmd set NULL, this case will call callback fuction,
		and send AT_PUT event.
	if want_res set NULL, send AT_GET event.
	callback :
		step current step
		event 
		buf when event==AT_PUT,that use for return a cmd
			when event==AT_GET,that use for get res of at revc.
		msg use for user defined data struct . set NULL. if no use
		return value 0 will complete current step. others are not.
*/
struct ATCMD_T
{
	const char *cmd;
	const char *want_res;
	int tmout_at_once;
	int trytimes;
	int (*callback)(
		int step, 
		enum ATEVENT_T event,
		char *buf, 
		void *msg);
};




/*
	arguements:
	at_tbl : define at cmd list and operation functions
	tbl_len : at cmd list size
	buff: recv data save here
	buff_size: buffer len ,max save chars
	msg: user defined data struct ,  usefor describ mes info

	return:
	normal return number is the current step.
	when all step continued , return sum of item of at_tbl.
*/
extern int atc_sequence_poll(
	const struct ATCMD_T *at_tbl, 
	int tbl_len,
	char *buff,
	int buff_size,
	void *msg
);
	
	
#define ATCMD_DELAY(ms)	 	{"DELAY",NULL,ms,-1,NULL,}
#define atc_getc_noblock(pc)	hx_uart_getc_noblock(UART_AT_PORT,pc)
#define atc_getc_timeout(pc,t)	hx_uart_getc_timeout(UART_AT_PORT,pc,t)
#define atc_getc_block(pc)		hx_uart_getc_block(UART_AT_PORT)
#define atc_gets_noblock(bf,bl)		hx_uart_gets_noblock(UART_AT_PORT,bf,bl)
#define atc_gets_timeout(bf,bl,t)	hx_uart_gets_timeout(UART_AT_PORT,bf,bl,t)
#define atc_gets_block(bf,bl)		hx_uart_gets_block(UART_AT_PORT,bf,bl)
#define atc_rxclr()			hx_uart_atc_rxclr(UART_AT_PORT)
#define atc_putc(c)			hx_uart_putc(UART_AT_PORT,c)
#define atc_send(d,l)		hx_uart_send(UART_AT_PORT,d,l)
#define atc_put(s)			hx_uart_put(UART_AT_PORT,s)
#define atc_puts(s)			hx_uart_puts(UART_AT_PORT,s)
#define atc_printf(...)		hx_uart_printf(UART_AT_PORT,__VA_ARGS__)

//===============================================================
//export at cmd drivers based on atcmd.c 
//rak415.c
//if param_file set NULL,that use deault.
extern int rak415_init(const char *param_file);
extern int rak415_reset(void);
// normal return <=-1(dB) , >=0 is not connect
extern int rak415_rssi(void);

//sim800c.c
//return 0 is connect ,others are not
extern const struct ATCMD_T sim800c_at_tbl[];
extern const int sim800c_step_count;
extern int sim800c_poll(void);

//sim7100c.c
extern const struct ATCMD_T sim7100c_at_tbl[];
extern const int sim7100c_step_count;
extern int sim7100c_poll(void);

//mg3732.c
//return 0 is connect ,others are not
extern const struct ATCMD_T mg3732_at_tbl[];
extern const int g_mg3732_step_count;
extern int mg3732_poll(void);

//ne4110s.c
struct NE4110_ST {
	char sn[10];
	char firm_ver[10];
	char module_name[20];
	char mac[20];
};
struct NE4110_CFG_ST
{
	int dhcp;
	char ip[16];
	char mask[16];
	char gw[16];
};
extern struct NE4110_ST g_nic_info;
extern void ne4110_init(void);
extern int ne4110_config(char *param_file);
extern int ne4110_reset(void);

#endif

