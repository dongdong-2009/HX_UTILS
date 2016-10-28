#include "hx_board.h"
#include "hx_utils.h"

__weak int brd_init(void)
{
	return 0;
}

__weak void brd_iomode(int port,uint pin,IOMODE_T val)
{
	cpu_iomode(port,pin,val);
}
__weak uint brd_ioval(int port)
{
	return cpu_ioval(port);
}
__weak void brd_ioctrl(int port,uint pin,int val)
{
	cpu_ioctrl(port,pin,val);
}
__weak void brd_ioclr(int port,uint pin)
{
	brd_ioctrl(port,pin,0);
}
__weak void brd_ioset(int port,uint pin)
{
	brd_ioctrl(port,pin,1);
}


//================================================================================

__weak void brd_uart_tx_start_byte(int id,int data)
{
	cpu_uart_tx_start_byte(id,data);

}	
__weak void brd_uart_tx_end_session(int id)
{
	cpu_uart_tx_end_session(id);
}

__weak int brd_uart_reopen(int id, int bps,int pclk)
{
	return cpu_uart_reopen(id,bps,pclk);
}

__weak int brd_dp_rxclr(int id)
{
	return 0;

}
__weak int brd_dp_getc_noblock(int id,int *c)
{
	return 0;

}
__weak int brd_dp_send(int id,const char* s, int len)
{
	return 0;
}

//================================================================================
//cmds
//#include "includes.h"
static int dummy;
__weak const DEV_PARAM_T g_param_tbl[] = {
	{"dummy",		"%d",	&dummy},
	//{"dev_sn",			"bcd 4",	Bus_Info_St.dev_sn},
	//{"bus_no",			"bcd 4",	Bus_Info_St.BusNo},
	//{"psam_no",			"bcd,6",	g_psam_nr},
	//{"ip",				"%hhu 4 .",	ParamCardSt.IP},
	//{"port",			"msb 2",	ParamCardSt.Port},
};

__weak const int g_param_count = sizeof(g_param_tbl)/sizeof(g_param_tbl[0]);
__weak int HX_PARAM_LOADSTORE(int load0store1)
{
	return 0;
}

