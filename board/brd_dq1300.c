#include "hx_utils.h"
#include "stdio.h"
#include "hxd_uart.h"
#include "hxd_param.h"
#include "hxd_atc.h"
#include "../board/brd_dq1300.h"
#include "../cpu/lpc1769/cpu_lpc1769.h"
#include "hxd_iopin.h"
#include "hxd_ioport.h" 
#include "string.h"

extern unsigned int __get_sysclk(void);

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
void mb_ioset(int port,uint pin)
{
	mb_ioctrl((port),cpu_ioval((port))|(pin));
}
void mb_ioclr(int port,uint pin)
{	
	mb_ioctrl((port),cpu_ioval((port))&(~(pin)));
}
static const struct IODESC_T mbio = {mb_iomode,mb_ioctrl,mb_ioval};


//-----------------------------------------------------------------
static const struct IODESC_T *g_iotbl[] = {&mbio};

static const IOPORT_DRV_T gpio_drv = {
	.drv = HX_IOPORT_STD_DRIVER,
	.iodesc = &mbio,
};
static const DEV_T cdev_gpio_tbl[] = {
	{"P0", 0,(const DEV_DRV_T*)&gpio_drv},
	{"P1", 1,(const DEV_DRV_T*)&gpio_drv},
	{"P2", 2,(const DEV_DRV_T*)&gpio_drv},
};

//-----------------------------------------------------------------
static const IOPIN_DRV_T iopin_drv = {
	.drv = HX_IOPIN_STD_DRIVER,
	.iotbl = g_iotbl,
	.tbl_count = sizeof(g_iotbl)/sizeof(g_iotbl[0]),
};
static const DEV_T cdev_at_io_pwr = 
	{"at_io_pwr",(_IOPIN_ID(0,3,25)),(const DEV_DRV_T*)&iopin_drv};
static const DEV_T cdev_at_io_rst = 
	{"at_io_rst",(_IOPIN_ID(0,3,26)),(const DEV_DRV_T*)&iopin_drv};

	
//--------------------------------------------------------
extern volatile unsigned __g_cpu_tickcount;
static unsigned int __sysclk;
static const PARAM_ITEM_T g_param_tbl[] = {
	{"clock",		"%u",	(void*)&__g_cpu_tickcount},
	{"sysclk",		"%u",	&__sysclk},
};

static int param_load(void)
{
	/* load params here */
	__sysclk = __get_sysclk();
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
	.dev = {"param",0,(DRIVER_T*)&param_drv},
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
	return cpu_uart_reopen(id,bps,__get_sysclk());
}
static int uart_close(const UART_DEV_T *dev)
{
	return 0;
}
static int uart_interrupt_ctrl(const UART_DEV_T *dev,int en)
{
	cpu_uart_interrupt_ctrl(dev->dev.devid,en);
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

//-------------------------------------------------------------
#define UART0_BUFSIZ			(2048)
#define UART0_DEF_BANDRATE		(115200)
static UART_PRIVATE_T uart0_prv;
char uart0_txbuf[UART0_BUFSIZ],uart0_rxbuf[UART0_BUFSIZ];
static const UART_DEV_T cdev_uart0 = {
	.dev = {"u0_com",0,(DRIVER_T*)&uart_drv},
	.txbuf = uart0_txbuf,
	.rxbuf = uart0_rxbuf,
	.txbuf_size = UART0_BUFSIZ,
	.rxbuf_size = UART0_BUFSIZ,
	.default_bps = UART0_DEF_BANDRATE,
	.prv = &uart0_prv,
};
static const UART_DEV_T cdev_at_uart = {
	.dev = {"at_uart",0,(DRIVER_T*)&uart_drv},
	.txbuf = uart0_txbuf,
	.rxbuf = uart0_rxbuf,
	.txbuf_size = UART0_BUFSIZ,
	.rxbuf_size = UART0_BUFSIZ,
	.default_bps = UART0_DEF_BANDRATE,
	.prv = &uart0_prv,
};
//-------------------------------------------------------
#define UART1_BUFSIZ			(512)
#define UART1_DEF_BANDRATE		(38400)
static UART_PRIVATE_T uart1_prv;
char uart1_txbuf[UART1_BUFSIZ],uart1_rxbuf[UART1_BUFSIZ];
static const UART_DEV_T cdev_uart1 = {
	.dev = {"u1_1516",1,(DRIVER_T*)&uart_drv},
	.txbuf = uart1_txbuf,
	.rxbuf = uart1_rxbuf,
	.txbuf_size = UART1_BUFSIZ,
	.rxbuf_size = UART1_BUFSIZ,
	.default_bps = UART1_DEF_BANDRATE,
	.prv = &uart1_prv,
};
//-------------------------------------------------------
#define UART2_BUFSIZ			(128)
#define UART2_DEF_BANDRATE		(57600)
static UART_PRIVATE_T uart2_prv;
char uart2_txbuf[UART2_BUFSIZ],uart2_rxbuf[UART2_BUFSIZ];
static const UART_DEV_T cdev_uart2 = {
	.dev = {"u2_485",2,(DRIVER_T*)&uart_drv},
	.txbuf = uart2_txbuf,
	.rxbuf = uart2_rxbuf,
	.txbuf_size = UART2_BUFSIZ,
	.rxbuf_size = UART2_BUFSIZ,
	.default_bps = UART2_DEF_BANDRATE,
	.prv = &uart2_prv,
};
//-------------------------------------------------------
const ATC_DEV_T g_cdev_sim800c = {
	.dev = {"sim800c",0,&hx_atc_drv},
	.param = &g_net_param,
	.nic = &nic_sim800c,
};

const ATC_DEV_T g_cdev_sim7600c = {
	.dev = {"sim7600c",0,&hx_atc_drv},
	.param = &g_net_param,
	.nic = &nic_sim7600c,
};
const ATC_DEV_T g_cdev_me3630_c1a = {
	.dev = {"me3630_c1a",0,&hx_atc_drv},
	.param = &g_net_param,
	.nic = &nic_me3630_pid_c1a,
};
const ATC_DEV_T g_cdev_me3630_c1b = {
	.dev = {"me3630_c1b",0,&hx_atc_drv},
	.param = &g_net_param,
	.nic = &nic_me3630_pid_c1b,
};
//------------------------------------------------------------------
void timer_init(void)
{
	timer_init2(__get_sysclk());
}
#include "psam.h"
const HX_IFMBR psam_inf0_mbs[] = {
	{IF_GPIO_PSAM_VCC,&(const IO_t){2,(1<<6),}},
	{IF_GPIO_PSAM_RST,&(const IO_t){2,(1<<2),}},
	{IF_GPIO_PSAM_CLK,&(const IO_t){0,(1<<20),}},
	{IF_GPIO_PSAM_DATA,&(const IO_t){2,(1<<11),}},
};
const INF_T psam_inf = {
	.name = "psam_if0",
	.devtype = DT_PSAM_IF,
	.devid = 0,
	.driver = 0,
	.members = psam_inf0_mbs,
	.count = 4,
};
extern DEV_DRV_T g_psam_drv ;
static const DEV_T psam0 = {"psam0",0,&g_psam_drv};
//-------------------------------------------------------------------
//const HX_IFMBR atc_inf_mbs[] = {
//	{IF_UART_ATC,&cdev_uart0,}},
//	{IF_GPIO_ATC_PWR,&(const IO_t){3,(1<<25),}},
//	{IF_GPIO_ATC_RST,&(const IO_t){3,(1<<26),}},
//};
//const INF_T atc_inf = {
//	.name = "psam_if0",
//	.devtype = DT_ATC_IF,
//	.devid = 0,
//	.driver = 0,
//	.members = atc_inf_mbs,
//	.count = 4,
//};
int brd_init(void)
{
	
	HX_DBG_PRINTLN ("BOARD:DQ1300");
	timer_init_use_for_tickcount(__get_sysclk());
	for(int i=0;i<sizeof(cdev_gpio_tbl)/sizeof(cdev_gpio_tbl[0]);
		i++){
		hx_register_device(&cdev_gpio_tbl[i]);
	}
	hx_register_uart_device(&cdev_uart0);
	hx_register_uart_device(&cdev_uart1);
	hx_register_uart_device(&cdev_uart2);
	hx_register_uart_device(&cdev_at_uart);
	
	//sim800c
	hx_register_device(&cdev_at_io_pwr);
	hx_register_device(&cdev_at_io_rst);
	
	//sim7100c
	hx_register_device((DEV_T*)&g_cdev_me3630_c1a);
	hx_register_device((DEV_T*)&g_cdev_me3630_c1b);
	
	
	hx_register_device((DEV_T*)&g_cdev_sim800c);
	hx_register_device((DEV_T*)&g_cdev_sim7600c);
	
	hx_register_device((DEV_T*)&psam_inf);
	hx_register_device((DEV_T*)&psam0);
	
	
	return 0;
}



void brd_uart_rx_byte(int id, int data)
{
	if(id==0)
		hxd_uart_rx_byte(&cdev_uart0,data);
	else if(id==1)
		hxd_uart_rx_byte(&cdev_uart1,data);
	else if(id==2)
		hxd_uart_rx_byte(&cdev_uart2,data);
	else
		fprintf(stderr,"warn:%s:id=%d\n",__FUNCTION__,id);
}
void brd_uart_tx_byte(int id)
{
	if(id==0)
		hxd_uart_tx_byte(&cdev_uart0);
	else if(id==1)
		hxd_uart_tx_byte(&cdev_uart1);
	else if(id==2)
		hxd_uart_tx_byte(&cdev_uart2);
	else
		fprintf(stderr,"warn:%s:id=%d\n",__FUNCTION__,id);
}

