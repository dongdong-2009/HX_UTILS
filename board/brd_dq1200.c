#include "hx_utils.h"
#include "stdio.h"
#include "hxd_uart.h"
#include "hxd_param.h"
#include "hxd_atc.h"
#include "../board/brd_dq1200.h"
#include "../cpu/cpu_lpc2378.h"
#include "hxd_iopin.h"
#include "hxd_ioport.h"

HX_DEV g_u0_com;

extern unsigned int __get_fpclk(void);

//-----------------------------------------------------------------
void mb_iomode(uint port,uint pin,uint mode)
{
	cpu_iomode(port,pin,(IOMODE_T)mode);
}
void mb_ioctrl(uint port,uint val)
{
	cpu_ioctrl(port,val);
}
uint mb_ioval(uint port)
{
	return cpu_ioval(port);
}
static const struct IODESC_T mbio = {mb_iomode,mb_ioctrl,mb_ioval};

//-----------------------------------------------------------------
void sb_iomode(uint port,uint pin,uint mode)
{
	HX_DBG_PRINTLN("SB IOMODE port=%u pin=%x. mode=%u",port,pin,mode);
	hxl_printf(&g_u0_com,"$hx+iomode+3+%x+%x+%x\r\n",
		port,pin,mode);
	char buff[64];
	if(hxl_gets_timeout3(&g_u0_com,buff,64,200,"OK"))
		HX_DBG_PRINTLN("OK.");
	else
		HX_DBG_PRINTLN("ERROR.");
}
void sb_ioctrl(uint port,uint val)
{
	HX_DBG_PRINTLN("SB IOCTRL port=%u val=%x.",port,val);
	hxl_printf(&g_u0_com,"$hx+ioctrl+2+%x+%x\r\n",
		port,val);
	char buff[64];
	if(hxl_gets_timeout3(&g_u0_com,buff,64,200,"OK"))
		HX_DBG_PRINTLN("OK.");
	else
		HX_DBG_PRINTLN("ERROR.");	
}

uint sb_ioval(uint port)
{
	int res ;
	HX_DBG_PRINTLN("SB IOVAL port=%u.",port);
	hxl_printf(&g_u0_com,"$hx+ioval+1+%x\r\n",port);
	char buff[64];
	res = hxl_gets_timeout3(&g_u0_com,buff,64,200,"res=");
	if(res>0){
		uint val;
		res = sscanf(buff,"res=%x",&val);
		if(res==1){
			HX_DBG_PRINTLN("GET %s.",buff);
			return val;
		}
	}
	HX_DBG_PRINTLN("ERROR default return 0.");
	return 0;
}

static const struct IODESC_T sbio = {sb_iomode,sb_ioctrl,sb_ioval};

//-----------------------------------------------------------------
static const struct IODESC_T *g_iotbl[] = {&mbio,&sbio};

static const IOPORT_DRV_T gpio_drv = {
	.drv = HX_IOPORT_STD_DRIVER,
	.iodesc = &mbio,
};
static const DEV_T cdev_gpio_tbl[] = {
	{"gpio0", 0,(const DEV_DRV_T*)&gpio_drv},
	{"gpio1", 1,(const DEV_DRV_T*)&gpio_drv},
	{"gpio2", 2,(const DEV_DRV_T*)&gpio_drv},
};

static const IOPORT_DRV_T sbio_drv = {
	.drv = HX_IOPORT_STD_DRIVER,
	.iodesc = &sbio,
};
static const DEV_T cdev_sbio = {"sbio0", 0,(const DEV_DRV_T*)&sbio_drv,};

//-----------------------------------------------------------------
static const IOPIN_DRV_T iopin_drv = {
	.drv = HX_IOPIN_STD_DRIVER,
	.iotbl = g_iotbl,
	.tbl_count = sizeof(g_iotbl)/sizeof(g_iotbl[0]),
};
static const DEV_T cdev_at_io_pwr = 
	{"at_io_pwr",(_IOPIN_ID(1,0,13)|PIN_ATTR_INVERSE),(const DEV_DRV_T*)&iopin_drv};
static const DEV_T cdev_at_io_rst = 
	{"at_io_rst",(_IOPIN_ID(1,0,6)|PIN_ATTR_INVERSE),(const DEV_DRV_T*)&iopin_drv};
//--------------------------------------------------------
extern volatile unsigned __g_cpu_tickcount;
static unsigned int __fpclk;
static const PARAM_ITEM_T g_param_tbl[] = {
	{"clock",		"%u",	(void*)&__g_cpu_tickcount},
	{"pclk",		"%u",	&__fpclk},
	//{"dev_sn",			"bcd 4",	Bus_Info_St.dev_sn},
	//{"bus_no",			"bcd 4",	Bus_Info_St.BusNo},
	//{"psam_no",			"bcd,6",	g_psam_nr},
	//{"ip",				"%hhu 4 .",	ParamCardSt.IP},
	//{"port",			"msb 2",	ParamCardSt.Port},
};

static int param_load(void)
{
	/* load params here */
	__fpclk = __get_fpclk();
	return 0;
}
static int param_store(void)
{
	/* store params here */
	return 0;
}

static const PARAM_DRV_T param_drv = {
	.cdrv = CDRV_PARAM_STD_DRIVER,
	.load = param_load,
	.store = param_store,
};

const PARAM_DEV_T brd_params = {
	.dev = {"param",0,&param_drv},
	.tbl = g_param_tbl,
	.count = sizeof(g_param_tbl)/sizeof(g_param_tbl[0]),
};



//---------------------------------------------------------

#define RS485_RD	PORT1,(1<<29)

#define RS485_CFG_RECV()	cpu_ioclr(RS485_RD)
#define RS485_CFG_SEND()	cpu_ioset(RS485_RD)

/*
	must enable tx interrupt.
*/
static int uart_tx_start_byte(const UART_DEV_T *dev,int data)
{
	int id = dev->dev.devid;
	if(id==3){
		RS485_CFG_SEND();
	}
	cpu_uart_tx_start_byte(id,data);
	return 0;
}	
static int uart_tx_end_session(const UART_DEV_T *dev)
{
	int id = dev->dev.devid;
	cpu_uart_tx_end_session(id);
	if(id==3){
		RS485_CFG_RECV();
	}
	return 0;
}
static int uart_open(const UART_DEV_T *dev, int bps)
{
	int id = dev->dev.devid;
	if(id==3){
		cpu_iomode(RS485_RD,IM_OUT);
		RS485_CFG_RECV();
	}
	return cpu_uart_reopen(id,bps,__get_fpclk());
}
static int uart_close(const UART_DEV_T *dev)
{
	return 0;
}
static int uart_interrupt_ctrl(const UART_DEV_T *dev,int en)
{
	cpu_interrupt_ctrl(en);
	return 0;
}
static const UART_DRV_T uart_drv = {
	.cdrv = CDRV_UART_STD_DRIVER,
	.tx_start_byte = uart_tx_start_byte,
	.tx_end_session = uart_tx_end_session,
	.interrupt_ctrl = uart_interrupt_ctrl,
	.open = uart_open,
	.close = uart_close,
};
static UART_PRIVATE_T uart0_prv;
char uart0_txbuf[128],uart0_rxbuf[128];
//char uart1_txbuf[128],uart1_rxbuf[128];
//char uart2_txbuf[128],uart2_rxbuf[128];
static UART_PRIVATE_T uart3_prv;
char uart3_txbuf[128],uart3_rxbuf[128];
static const UART_DEV_T cdev_uart0 = {
	.dev = {"u0_com",0,&uart_drv},
	.txbuf = uart0_txbuf,
	.rxbuf = uart0_rxbuf,
	.txbuf_size = 128,
	.rxbuf_size = 128,
	.default_bps = 115200,
	.prv = &uart0_prv,
};
static const UART_DEV_T cdev_at_uart = {
	.dev = {"at_uart",0,&uart_drv},
	.txbuf = uart0_txbuf,
	.rxbuf = uart0_rxbuf,
	.txbuf_size = 128,
	.rxbuf_size = 128,
	.default_bps = 115200,
	.prv = &uart0_prv,
};
static const UART_DEV_T cdev_uart3 = {
	.dev = {"u3_485",3,&uart_drv},
	.txbuf = uart3_txbuf,
	.rxbuf = uart3_rxbuf,
	.txbuf_size = 128,
	.rxbuf_size = 128,
	.default_bps = 57600,
	.prv = &uart3_prv,
};

//----------------------------------------------------------------

#define BRD_DBG_PRINT(ch,...)	fprintf(ch,__VA_ARGS__)
int sub_brd_init(void)
{
	int res;
	
	res = hx_open("u0_com","115200",&g_u0_com);
	if(res){
		BRD_DBG_PRINT(stderr,"please init uart0 before init sb\r\n");
		return -1;
	}
	hxl_printf(&g_u0_com,"$hx+version+0\r\n");
	char buff[64];
	do{
		res = hxl_gets_timeout(&g_u0_com,buff,64,500);
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

int brd_init(void)
{
	HX_DBG_PRINTLN ("BOARD:dq1200");
	cpu_init_tickcount_1m_by_pclk(__get_fpclk());
	for(int i=0;i<sizeof(cdev_gpio_tbl)/sizeof(cdev_gpio_tbl[0]);
		i++){
		hx_register_char_device(&cdev_gpio_tbl[i]);
	}
	hx_register_uart_device(&cdev_uart0);
	hx_register_uart_device(&cdev_uart3);
	hx_register_char_device(&cdev_sbio);
	
	hx_register_uart_device(&cdev_at_uart);
	hx_register_char_device(&cdev_at_io_pwr);
	hx_register_char_device(&cdev_at_io_rst);
	hx_register_char_device((DEV_T*)&g_cdev_sim800c);
	
	sub_brd_init();
	return 0;
}



void brd_uart_rx_byte(int id, int data)
{
	if(id==0)
		hxd_uart_rx_byte(&cdev_uart0,data);
	else if(id==3)
		hxd_uart_rx_byte(&cdev_uart3,data);
	else
		fprintf(stderr,"warn:%s:id=%d\n",__FUNCTION__,id);
}
void brd_uart_tx_byte(int id)
{
	if(id==0)
		hxd_uart_tx_byte(&cdev_uart0);
	else if(id==3)
		hxd_uart_tx_byte(&cdev_uart3);
	else
		fprintf(stderr,"warn:%s:id=%d\n",__FUNCTION__,id);
}

