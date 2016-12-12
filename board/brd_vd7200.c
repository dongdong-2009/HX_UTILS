#include "hx_board.h"
#include "hx_utils.h"
#include "hx_cpu.h"
#include "stdio.h"
#include "string.h"

#define RS485_RD	PORT1,(1<<29)

#define RS485_CFG_RECV()	cpu_ioclr(RS485_RD)
#define RS485_CFG_SEND()	cpu_ioset(RS485_RD)


/*
	must enable tx interrupt.
*/
void brd_uart_tx_start_byte(int id,int data)
{
	cpu_uart_tx_start_byte(id,data);

}	
void brd_uart_tx_end_session(int id)
{
	cpu_uart_tx_end_session(id);
	if(id==3){
		RS485_CFG_SEND();
	}
}

int brd_uart_reopen(int id, int bps,int pclk)
{
	if(id==3){
		cpu_iomode(RS485_RD,IM_OUT);
		RS485_CFG_RECV();
	}
	return cpu_uart_reopen(id,bps,pclk);
}
//================================================================================
static int __getc(void)
{
	return fgetc(stdin);
}

int brd_dp_rxclr(int id)
{
	int c;
	do{
		c = __getc();
	}while(c==EOF);
	return 0;
}

int brd_dp_getc_noblock(int id,int *c)
{
	int res = __getc();
	if(res!=EOF){
		*c = 0xFFu & res;
		return 0;
	}
	return -1;
}
int brd_dp_send(int id,const char* s, int len)
{
	for(int i=0;i<len;i++){
		fputc(s[i],(FILE*)id);
	}
	return len;
}

//================================================================================
//cmds
//#include "includes.h"
//int dummy;
//const DEV_PARAM_T g_param_tbl[] = {
//	{"dummy",		"%d",	&dummy},
//	//{"dev_sn",			"bcd 4",	Bus_Info_St.dev_sn},
//	//{"bus_no",			"bcd 4",	Bus_Info_St.BusNo},
//	//{"psam_no",			"bcd,6",	g_psam_nr},
//	//{"ip",				"%hhu 4 .",	ParamCardSt.IP},
//	//{"port",			"msb 2",	ParamCardSt.Port},
//};

//const int g_param_count = sizeof(g_param_tbl)/sizeof(g_param_tbl[0]);
int brd_param_loadstore(int load0store1)
{
	return 0;
//	int res;
//	if(load0store1 == 0){
//		return 0;
//	}else{
//		res = Writebusn2flash((BYTE *) &Bus_Info_St);
//		if (res != 0){
//			hxt_printf("ERRPR: Write Bus Info To Flash Return %d\r\n",res);
//			return -9;
//		}
//		res = update_param_to_flash();
//		if (res != 0){
//			hxt_printf("ERRPR: Write Param To Flash Return %d\r\n",res);
//			return -9;
//		}
//		return 0;
//	}
}

//=======================================================================
static char psam_buff[256+2]={0};
static int psam_buff_len=0;
extern int psamcard_cmd(int slot_nr, const uchar *cmd, int cmdlen, uchar *rbuf, int *rlen);
static int psam_read(const DEV_T *dev,void *buf,int size)
{
	int res = psam_buff_len;
	if(psam_buff_len>0){
		memcpy(buf,psam_buff,psam_buff_len);
		psam_buff_len = 0;
		return res;
	}
	return -1;
}
static int psam_write(const DEV_T *dev,const void *buf, int size)
{
	psam_buff_len = 0;
	int res = psamcard_cmd(dev->devid,buf,size,(void*)(psam_buff+2),&psam_buff_len);
	HX_MSB_W2B(res,psam_buff);
	return size;
}

CDEV_DRV_T psam_drv = {
	//.open = psam_open,
	.read = psam_read,
	.write = psam_write,
	//.close = psam_close,
};
//--------------------------------------------------------------
static char card_buff[256+2]={0};
static int card_buff_len=0;
extern int ll_cpucard_cmd(const uchar *cmd, int cmdlen, uchar *resbuf, int *reslen);	
static int card_read(const DEV_T *dev,void *buf,int size)
{
	int res = card_buff_len;
	if(card_buff_len>0){
		memcpy(buf,card_buff,card_buff_len);
		card_buff_len = 0;
		return res;
	}
	return -1;
}
static int card_write(const DEV_T *dev,const void *buf, int size)
{
	card_buff_len = 0;
	int res = ll_cpucard_cmd(buf,size,(void*)(card_buff+2),&card_buff_len);
	HX_MSB_W2B(res,card_buff);
	return size;
}

CDEV_DRV_T card_drv = {
	//.open = psam_open,
	.read = card_read,
	.write = card_write,
	//.close = psam_close,
};
//------------------------------------------------------------
static const DEV_T cdev_psam0 = {"psam0",0,&psam_drv};
static const DEV_T cdev_psam1 = {"psam1",1,&psam_drv};
static const DEV_T cdev_psam2 = {"psam2",2,&psam_drv};
static const DEV_T cdev_card  = {"card",0,&card_drv};


int brd_init(void)
{
	hx_register_char_device(&cdev_psam0);
	hx_register_char_device(&cdev_psam1);
	hx_register_char_device(&cdev_psam2);
	hx_register_char_device(&cdev_card);
	//hx_register_char_device(&cdev_uart);
	HX_DBG_PRINTLN ("BOARD:VD7200");
	return 0;
}

