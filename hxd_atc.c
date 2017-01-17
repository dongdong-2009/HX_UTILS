/*
 * hxd_atc.c
 *
 *  Created on: 2016年11月14日
 *      Author: houxd
 */

#include "hxd_atc.h"
#include "hx_device.h"
#include "hx_utils.h"
#include "stdio.h"
#include "string.h"
#include "stddef.h"
#include "stdarg.h"
#include "hxd_uart.h"
#include "hxd_param.h"

#define VSPRINTF_BUFF_SIZE  (512)

/*
	the interface of net 
*/
HX_DEV g_at_uart;
static HX_DEV at_io_pwr;
static HX_DEV at_io_rst;


void nic_pwr(int level)
{
	char c = level;
	hx_write(&at_io_pwr,&c,1);
}
void nic_rst(int level)
{
	char c = level;
	hx_write(&at_io_rst,&c,1);
}
//-----------------------------------------------------------------

struct NET_PARAM_T g_net_param = {0};

static const PARAM_ITEM_T g_param_tbl[] = {
	{"step",		"%u",	&g_net_param.step},
	{"ap",			"%s",	g_net_param.apn},
	{"rm_ip",		"%hhu 4 .",	g_net_param.rm_ip},
	{"rm_port",		"%hu",	&g_net_param.rm_port},
};

static const PARAM_DRV_T param_drv = {
	.cdrv = CDRV_PARAM_STD_DRIVER,
};

const PARAM_DEV_T net_param_dev = {
	.dev = {"net_param",0,&param_drv},
	.tbl = g_param_tbl,
	.count = sizeof(g_param_tbl)/sizeof(g_param_tbl[0]),
};
//-----------------------------------------------------------------
//this must in dev struct
static int atc_open(HX_DEV *dev,const char *param)
{
	const ATC_DEV_T *atcdev = (const ATC_DEV_T *)dev->pdev;
	const struct HX_NIC_T *nic = atcdev->nic;
	
	//open interface of net
	if(hx_open("at_uart","115200",&g_at_uart)){
		fprintf(stderr,"at_uart cannot opend\n");
		return -1;
	}
	if(hx_open("at_io_pwr","1",&at_io_pwr)){
		fprintf(stderr,"at_io_pwr cannot opend\n");
		return -1;
	}
	if(hx_open("at_io_rst","1",&at_io_rst)){
		fprintf(stderr,"at_io_rst cannot opend\n");
		return -1;
	}
	
	//copy default params and init private param
	g_net_param = *(nic->default_param);
	g_net_param.step = 0;
	hx_register_params(&net_param_dev);
	
	//init nic
	if(nic->init)
		return nic->init(nic);
	return 0;
}
static int _atc_poll(HX_DEV *dev,void *vp,int ip)
{
	const ATC_DEV_T *atcdev = (const ATC_DEV_T *)dev->pdev;
	const struct HX_NIC_T *nic = atcdev->nic;
	if(nic->at_tbl_count == dev->offset)
		return 0;
	dev->offset = atc_poll(nic,&(atcdev->param->step),atcdev->param);
	return -1;
}
static int atc_read(HX_DEV *dev,void *buf,int _size)
{
	const ATC_DEV_T *atcdev = (const ATC_DEV_T *)dev->pdev;
	const struct HX_NIC_T *nic = atcdev->nic;
	if(nic->at_tbl_count < dev->offset){
		/*cmd mode need to achive.*/
	}else{
		if(nic->read){
			return nic->read(nic,buf,_size);
		}
		UART_DRV_T *udrv = (UART_DRV_T *)(g_at_uart.pdev->driver);
		if(udrv->cdrv.read)
			return udrv->cdrv.read(&g_at_uart,buf,_size);
	}
	return -1;
}

static int atc_write(HX_DEV *dev,const void *buf, int size)
{
	const ATC_DEV_T *atcdev = (const ATC_DEV_T *)dev->pdev;
	const struct HX_NIC_T *nic = atcdev->nic;
	if(nic->at_tbl_count < dev->offset){
		/*cmd mode need to achive.*/
	}else{
		if(nic->write){
			return nic->write(nic,buf,size);
		}
		UART_DRV_T *udrv = (UART_DRV_T *)(g_at_uart.pdev->driver);
		if(udrv->cdrv.write)
			return udrv->cdrv.write(&g_at_uart,buf,size);
	}
	return -1;
}
static int atc_close(HX_DEV *dev)
{
	const ATC_DEV_T *atcdev = (const ATC_DEV_T *)dev->pdev;
	const struct HX_NIC_T *nic = atcdev->nic;
	g_net_param.step = 0;
	if(nic->reset)
		return nic->reset(nic);
	return 0;
}

const DEV_DRV_T hx_atc_drv = {
	.open = atc_open,
	.read = atc_read,
	.write = atc_write,
	.close = atc_close,
	.poll = _atc_poll,
};




//------------------------------------------------------------------------

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
int atc_poll(const struct HX_NIC_T *nic, int *pstep, struct NET_PARAM_T *param)
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
				hxl_rxclr(&g_at_uart);
                hxl_printf(&g_at_uart,"%s\r\n",at_tbl[step].cmd);
                is_send = 1;
                HX_DBG_PRINT("ATCMD_TX: [%s]\r\n",at_tbl[step].cmd);
            }
        } else if(at_tbl[step].callback) {	//has none, than callback
            memset(buff,0,buff_size);
            at_tbl[step].callback(step,AT_PUT,buff,param);
			hxl_rxclr(&g_at_uart);
            hxl_printf(&g_at_uart,"%s\r\n",buff);
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
    res = hxl_gets_noblock(&g_at_uart,buff,buff_size);
    if(res>=0) {
        int get_ok = 0;
        HX_DBG_PRINT("\tRX: [%s]\r\n",buff);
        char *s = strtrim(buff,"\r\n ");
        if(at_tbl[step].want_res &&
                strcmp(buff,at_tbl[step].want_res)==0) {
            get_ok = 1;
        } else if(at_tbl[step].callback) {
            res = at_tbl[step].callback(step,AT_GET,s,param);
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
                return -1;		//到头了
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
                is_send = 0;	//复位
				*pstep = step;
                return 0;
            } else {
                is_send = 0;	//重发
            }
        }
    }
	
	//if(step < nic->at_tbl_count){
	*pstep = step;
	return step;
	//}else{
	//	return -1;
	//}
}

int atc_getc_noblock(int *pc)
{
    return hxl_getc_noblock(&g_at_uart,pc);
}
int atc_getc_timeout(int *pc,int t)
{
    return hxl_getc_timeout(&g_at_uart,pc,t);
}
int atc_getc_block(int *pc)
{
    return hxl_getc_block(&g_at_uart);
}
int atc_gets_noblock(char *bf,int bl)
{
    return hxl_gets_noblock(&g_at_uart,bf,bl);
}
int atc_gets_timeout(char *bf,int bl,int t)
{  
	return hxl_gets_timeout(&g_at_uart,bf,bl,t);
}
void atc_gets_block(char *bf, int bl)
{   
	 hxl_gets_block(&g_at_uart,bf,bl);
}
void atc_rxclr(void)
{
     hxl_rxclr(&g_at_uart);
}
void atc_putc(int c)
{
     hxl_putc(&g_at_uart,c);
}
void atc_send(const char *d,int l)
{
     hxl_send(&g_at_uart,d,l);
}
void atc_put(const  char *s)
{
     hxl_put(&g_at_uart,s);
}
void atc_puts(const  char *s)
{
     hxl_puts(&g_at_uart,s);
}
int atc_printf(const char *format, ...)
{
    int res;
    //use data or stack!
    //static
    char  buffer[VSPRINTF_BUFF_SIZE];

    va_list  vArgs;
    va_start(vArgs, format);
    res = vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    hxl_put(&g_at_uart, buffer);
    hxl_send(&g_at_uart,NULL,0);
    return res;
}


