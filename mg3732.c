#include "hx_atcmd.h"
#include "hx_utils.h"
#include "string.h"
#include "stdio.h"


static int check_zipcall(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	if(strstr(buf,"+ZIPCALL")){
		return 0;
	}
	return -1;
}

static int check_zipstat(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	int socket,state;
	sscanf(buf,"+ZIPSTAT: %d,%d",&socket,&state);
    if(socket>0 && state==1)
        return 0;
    return -1;
}

const struct ATCMD_T mg3732_at_tbl[] = {
	//cmd					res			timeout		trytimes	check_res_proc
	{"AT",					"AT",		2000,		20, 		0},
	{"ATE0",				"OK",		2000,		20, 		0},
	//{"AT+CSQ",				NULL,		2000,		20, 		check_csq},
	{"AT+ZIPCFG=cmnet",		"OK",		2000,		5, 			0},
	{"AT+ZIPCALL=1",		NULL,		30000,		10,			check_zipcall,},
	//这里需要延时
	ATCMD_DELAY(2000),
	//AT+CIPCSGP=1,"cmnet","",""
	//{"AT+CIPCSGP=1,\"cmnet\",\"\",\"\"",		
	//						"OK",		2000,		5, 			0},
	//{"AT+CIPSHUT",			"SHUT OK",	2000,		5, 			0},
	//180.89.58.27:9020
//	{"AT+CIPSTART=\"TCP\",\"119.75.218.70\",\"80\"",	
//							NULL/*"CONNECT"*/,	5000,5, 		check_connect},
	//AT+CIPSTART="TCP","180.89.58.27","9020"
	{"AT+ZIPOPEN=1,0,180.89.58.27,9020,80",
							NULL,		30000,		5,			check_zipstat},	
//	{"AT+CIPSTART=\"TCP\",\"180.89.58.27\",\"9020\"",	
//							NULL/*"CONNECT"*/,	30000,5, 		check_connect},
};

const int g_mg3732_step_count = sizeof(mg3732_at_tbl)/sizeof(mg3732_at_tbl[0]);


//return 0 is connect ,others are not
int mg3732_poll(void)
{
	char buf[4096];
	int step = atc_sequence_poll(
					mg3732_at_tbl,
					sizeof(mg3732_at_tbl)/sizeof(mg3732_at_tbl[0]),
					buf,
					4096,
					NULL);
	if(step < sizeof(mg3732_at_tbl)/sizeof(mg3732_at_tbl[0])){
		return step;
	}else{
		return -1;
	}
}


