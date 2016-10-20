#include "hx_board.h"
#include "hx_utils.h"
#include "hx_cpu.h"
#include "stdio.h"

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

