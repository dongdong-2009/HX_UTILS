#include "hx_utils.h"
#include "hx_board.h"
#include "string.h"
#include "stdio.h"

static int check_csq(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	//+CSQ: %d,%d
	if(buf[0] == '+'){
		int a=0,b=0;
		sscanf(buf,"+CSQ: %d,%d",&a,&b);
		if(a>0)
			return 0;
	}
	return -1;
}
static int check_connect(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	//+CSQ: %d,%d
	if(strcmp(buf,"CONNECT")==0
	||strcmp(buf,"ALREADY CONNECT")==0	
	){
		return 0;
	}
	return -1;
}

const struct ATCMD_T sim800c_at_tbl[] = {
	//cmd					res			timeout		trytimes	check_res_proc
	{"AT",					"AT",		2000,		20, 		0},
	{"ATE0",				"OK",		2000,		20, 		0},
	{"AT+CSQ",				NULL,		2000,		20, 		check_csq},
	{"AT+CIPMODE=1",		"OK",		2000,		5, 			0},
	//AT+CIPCSGP=1,"cmnet","",""
	{"AT+CIPCSGP=1,\"cmnet\",\"\",\"\"",		
							"OK",		2000,		5, 			0},
	{"AT+CIPSHUT",			"SHUT OK",	2000,		5, 			0},
	//180.89.58.27:9020
//	{"AT+CIPSTART=\"TCP\",\"119.75.218.70\",\"80\"",	
//							NULL/*"CONNECT"*/,	5000,5, 		check_connect},
	//AT+CIPSTART="TCP","180.89.58.27","9020"
	{"AT+CIPSTART=\"TCP\",\"180.89.58.27\",\"9020\"",	
							NULL/*"CONNECT"*/,	30000,5, 		check_connect},
};

const int sim800c_step_count = sizeof(sim800c_at_tbl)/sizeof(sim800c_at_tbl[0]);

//return 0 is connect ,others are not
int sim800c_poll(void)
{
	char buf[4096];
	int step = atc_sequence_poll(
					sim800c_at_tbl,
					sizeof(sim800c_at_tbl)/sizeof(sim800c_at_tbl[0]),
					buf,
					4096,
					NULL);
	if(step < sizeof(sim800c_at_tbl)/sizeof(sim800c_at_tbl[0])){
		return -1;
	}else{
		return 0;
	}
}


//#define GPRSBordPowerON     LPC_GPIO3->FIOSET|=(1<<25);
//#define GPRSBordPowerOFF    LPC_GPIO3->FIOCLR|=(1<<25);
//#define GPRSPowerL     LPC_GPIO1->FIOSET|=(1<<20);//按键开关  反向的					
//#define GPRSPowerH     LPC_GPIO1->FIOCLR|=(1<<20);//按键开关  反向的
//BYTE GPRS_init(void)	//20140311 OK								   
//{
//	
//	  LPC_GPIO3->FIODIR|=1<<25;//?????????
////	  LPC_GPIO1->FIODIR&=~(1<<23);//GPRS??????  ??
////	  LPC_GPIO1->FIODIR|=1<<20;//?????
////	  GPRSBordPowerOFF;//??
////	  delay_10ms(20);
//	  GPRSBordPowerON;//??
//	  return 0;	
//}

