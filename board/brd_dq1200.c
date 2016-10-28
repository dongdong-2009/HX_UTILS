#include "hx_board.h"
#include "hx_utils.h"
#include "hx_cpu.h"
#include "stdio.h"

//#define SUB_BOARD_IO_INVERSE

#define BRD_DBG_PRINT(ch,...)	fprintf(ch,__VA_ARGS__)

int brd_init(void)
{
	int res;
	hx_uart_init(0,115200,hx_get_gpclk());
	
	atc_printf("hx+version+0\r\n");
	
	char buff[64];
	do{
		res = atc_gets_timeout(buff,64,200);
		if(res>4 && buff[0]=='r'){
			uint val;
			res = sscanf(buff,"res=%u",&val);
			if(res==1){
				const int need_ver = 3;
				if(val>=need_ver){
					BRD_DBG_PRINT(stdin,"board init ok: sb ver: %s\r\n",&buff[4]);
					
					return 0;
				}else{
					BRD_DBG_PRINT(stderr,"board init error: SBVER too low %d,"
						"must>=%d\r\n",val,need_ver);
					return -2;
				}
			}else{
				break;
			}
		}
	}while(res>=0);
	BRD_DBG_PRINT(stderr,"board init error: no response\r\n");
	return -1;
}

static inline int port_equal(int po1,uint pi1,int po2,uint pi2)
{
	if(po1==po2 && pi1==pi2)
		return 1;
	return 0;
}
static inline int port_equal2(int po1,uint pi1,int po2)
{
	if(po1==po2)
		return 1;
	return 0;
}


void brd_iomode(int port,uint pin,IOMODE_T val)
{
	if(port_equal(BRD_NIC_PWR,port,pin) ||
		port_equal(BRD_NIC_RST,port,pin)){
		atc_printf("hx+iomode+3+%x+%x+%x\r\n",
			port,pin,(int)(val==IM_OUT?1:0));
		return;
	}
	cpu_iomode(port,pin,val);
}
uint brd_ioval(int port)
{
	int res ;
	uint val = cpu_ioval(port);
	if(port_equal2(BRD_NIC_PWR,port) ||
		port_equal2(BRD_NIC_RST,port)){
		atc_printf("hx+ioval+1+%x\r\n",
			port);
		char buff[64];
		do{
			res = atc_gets_timeout(buff,64,200);
			if(res>4 && buff[0]=='r'){
				uint val;
				res = sscanf(buff,"res=%u",&val);
				if(res==1){
					return val;
				}else{
					break;
				}
			}
		}while(res>=0);
	}
	return 0;
}
void brd_ioctrl(int port,uint pin,int val)
{
#ifdef SUB_BOARD_IO_INVERSE
	val = val?0:1;
#endif
	if(port_equal(BRD_NIC_PWR,port,pin) ||
		port_equal(BRD_NIC_RST,port,pin)){
		atc_printf("hx+ioctrl+3+%x+%x+%x\r\n",
			port,pin,(int)val);
		return;
	}else{
		cpu_ioctrl(port,pin,val);
	}
}


//==============================================================

#warning belows are copies of vd7200
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
		fputc(s[i],stdout);
	}
	return len;
}

//================================================================================
//cmds
//#include "includes.h"
int dummy;
const DEV_PARAM_T g_param_tbl[] = {
	{"dummy",		"%d",	&dummy},
	//{"dev_sn",			"bcd 4",	Bus_Info_St.dev_sn},
	//{"bus_no",			"bcd 4",	Bus_Info_St.BusNo},
	//{"psam_no",			"bcd,6",	g_psam_nr},
	//{"ip",				"%hhu 4 .",	ParamCardSt.IP},
	//{"port",			"msb 2",	ParamCardSt.Port},
};

const int g_param_count = sizeof(g_param_tbl)/sizeof(g_param_tbl[0]);
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

