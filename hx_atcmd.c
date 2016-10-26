#include "hx_utils.h"
#include "hx_board.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"



static HX_ATARG_T g_atarg = {0};
HX_NIC_INFO_T g_nic_info = {0};


void atc_get_arg(HX_ATARG_T *parg)
{
	*parg = g_atarg;
}
void atc_set_arg(const HX_ATARG_T *parg)
{
	g_atarg = *parg;
}

int atc_default_init(const struct HX_NIC_T *this, int *pstep, HX_ATARG_T *arg)
{
	if(pstep)
		*pstep = 0;
	if(arg)
		g_atarg = *(HX_ATARG_T*)arg;
	else
		g_atarg = *(this->default_arg);
	return 0;
}


#define ATC_POLL_BUFSIZE		(4096)

/*
	arguements:
	nic	:	net card struct
	pstep : instans of current step
	msg : private data here
	return : current step

	return:
	normal return number is the current step.
	when all step continued , return sum of item of at_tbl.
*/
int atc_poll(const struct HX_NIC_T *nic, int *pstep, void *msg)
{
	if(nic == NULL | pstep == NULL)
		return -1;
	char buff[ATC_POLL_BUFSIZE];
	const struct ATCMD_T *at_tbl = nic->at_tbl;
    int tbl_len = nic->at_tbl_count;
    int buff_size = ATC_POLL_BUFSIZE;
    int res;
	int step = *pstep;
    if(step >= tbl_len)
        return tbl_len;
    static int is_send = 0;
    int tmout_at_once = at_tbl[step].tmout_at_once;
    int try_times = at_tbl[step].trytimes;
    static int tmof_last_state_change = -1;
    if(tmof_last_state_change == -1)
        tmof_last_state_change = hx_get_tick_count();
    if(!is_send) {
        if(at_tbl[step].cmd) {	//if has cmd than send it
            //this is delay cmd ???
            if(try_times == -1 && tmout_at_once>0) {
                if(hx_check_timeout2(&tmof_last_state_change,tmout_at_once)) {
                    step ++ ;
                    is_send = 0;
                }
				*pstep = step;
                return step;
            } else {
                hx_uart_printf(UART_AT_PORT,"%s\r\n",at_tbl[step].cmd);
                is_send = 1;
                HX_DBG_PRINT("ATCMD_TX: [%s]\r\n",at_tbl[step].cmd);
            }
        } else if(at_tbl[step].callback) {	//has none, than callback
            memset(buff,0,buff_size);
            at_tbl[step].callback(step,AT_PUT,buff,NULL);
            hx_uart_printf(UART_AT_PORT,"%s\r\n",buff);
            is_send = 1;
            HX_DBG_PRINT("ATCMD_TX: [%s]\r\n",buff);
        } else {
            //no this stuation
            HX_DBG_PRINT("**error: ATCMD bad line:\r\n");
            HX_DBG_PRINT("\t step: %s\r\n", step);
            HX_DBG_PRINT("\t cmd: %s\r\n", at_tbl[step].cmd);
            HX_DBG_PRINT("\t want_res: %s\r\n", at_tbl[step].want_res);
            HX_DBG_PRINT("\t timeout_At_once: %d\r\n", at_tbl[step].tmout_at_once);
            HX_DBG_PRINT("\t trytimes: %d\r\n", at_tbl[step].trytimes);
            HX_DBG_PRINT("\t cmd: %d\r\n", at_tbl[step].callback);
        }
    }
again:
    res = hx_uart_gets_noblock(UART_AT_PORT,buff,buff_size);
    if(res>=0) {
        int get_ok = 0;
        HX_DBG_PRINT("\tRX: [%s]\r\n",buff);
        char *s = strtrim(buff,"\r\n ");
        if(at_tbl[step].want_res &&
                strcmp(buff,at_tbl[step].want_res)==0) {
            get_ok = 1;
        } else if(at_tbl[step].callback) {
            res = at_tbl[step].callback(step,AT_GET,s,msg);
            if(res == 0)
                get_ok = 1;
        }
        if(get_ok) {
            step ++ ;
            is_send = 0;
            tmof_last_state_change = hx_get_tick_count();
            if(step == tbl_len) {
                HX_DBG_PRINT("\tModule Init Complete\r\n");
				*pstep = step;
                return -1;		//��ͷ��
            } else {
				*pstep = step;
                return step;
            }
        } else {
            goto again;
        }
    } else {
        res = hx_check_timeout(tmof_last_state_change,tmout_at_once);
        if(res) {
            tmof_last_state_change = hx_get_tick_count();
            if(--try_times<0) {
                step = 0;
                is_send = 0;	//��λ
				*pstep = step;
                return 0;
            } else {
                is_send = 0;	//�ط�
            }
        }
    }
	
	if(step < nic->at_tbl_count){
		*pstep = step;
		return step;
	}else{
		return -1;
	}
}

int atc_getc_noblock(int *pc)
{
    return hx_uart_getc_noblock(UART_AT_PORT,pc);
}
int atc_getc_timeout(int *pc,int t)
{
    return hx_uart_getc_timeout(UART_AT_PORT,pc,t);
}
int atc_getc_block(int *pc)
{
    return hx_uart_getc_block(UART_AT_PORT);
}
int atc_gets_noblock(char *bf,int bl)
{
    return hx_uart_gets_noblock(UART_AT_PORT,bf,bl);
}
int atc_gets_timeout(char *bf,int bl,int t)
{  
	return hx_uart_gets_timeout(UART_AT_PORT,bf,bl,t);
}
void atc_gets_block(char *bf, int bl)
{   
	 hx_uart_gets_block(UART_AT_PORT,bf,bl);
}
void atc_rxclr(void)
{
     hx_uart_atc_rxclr(UART_AT_PORT);
}
void atc_putc(int c)
{
     hx_uart_putc(UART_AT_PORT,c);
}
void atc_send(const char *d,int l)
{
     hx_uart_send(UART_AT_PORT,d,l);
}
void atc_put(const  char *s)
{
     hx_uart_put(UART_AT_PORT,s);
}
void atc_puts(const  char *s)
{
     hx_uart_puts(UART_AT_PORT,s);
}
int atc_printf(const char *format, ...)
{
    int port_nr = UART_AT_PORT;

    int res;
    //use data or stack!
    //static
    char  buffer[VSPRINTF_BUFF_SIZE];

    va_list  vArgs;
    va_start(vArgs, format);
    res = vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    hx_uart_put(port_nr, buffer);
    hx_uart_send(port_nr,NULL,0);
    return res;
}


