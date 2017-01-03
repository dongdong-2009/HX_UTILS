#include "hx_utils.h"
#include "string.h"
#include "stdio.h"
#include "hxd_atc.h"

static int config_ap(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	struct NET_PARAM_T *p = msg;
	sprintf(buf,"AT+CIPCSGP=1,\"%s\",\"\",\"\"",p->apn);
	return 0;
}

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
	if(event==AT_PUT){
		struct NET_PARAM_T *p = msg;
		sprintf(buf,"AT+CIPSTART=\"TCP\",\"%u.%u.%u.%u\",\"%u\"",
			(unsigned int)(p->rm_ip[0]),
			(unsigned int)(p->rm_ip[1]),
			(unsigned int)(p->rm_ip[2]),
			(unsigned int)(p->rm_ip[3]),
			(unsigned int)(p->rm_port));
		return 0;
	}else if(event == AT_GET){
		if(strstr(buf,"FAIL"))	//CONNECT FAIL
			return -1;
		if(strstr(buf,"ERROR"))	//CONNECT ERROR
			return -2;			
		if(strstr(buf,"CONNECT"))	//CONNECT 115200 / ALREADY CONNECT etc..
			return 0;
		return -1;
	}
	return -1;
}

static const struct ATCMD_T at_tbl[] = {
	//cmd					res			timeout		trytimes	check_res_proc
	{"AT",					"AT",		2000,		20, 		0},
	{"ATE0",				"OK",		2000,		20, 		0},
	{"AT+CSQ",				NULL,		2000,		20, 		check_csq},
	{"AT+CIPMODE=1",		"OK",		2000,		5, 			0},
	//AT+CIPCSGP=1,"cmnet","",""
	{NULL/*"AT+CIPCSGP=1,\"cmnet\",\"\",\"\""*/,		
							"OK",		2000,		5, 			config_ap},
	{"AT+CIPSHUT",			"SHUT OK",	2000,		5, 			0},
	//180.89.58.27:9020
//	{"AT+CIPSTART=\"TCP\",\"119.75.218.70\",\"80\"",	
//							NULL/*"CONNECT"*/,	5000,5, 		check_connect},
	//AT+CIPSTART="TCP","180.89.58.27","9020"
	//{"AT+CIPSTART=\"TCP\",\"119.75.218.70\",\"80\"",	
	{NULL/*"AT+CIPSTART=\"TCP\",\"180.89.58.27\",\"9020\""*/,	
							NULL/*"CONNECT"*/,	30000,5, 		check_connect},
};

static int _init(const struct HX_NIC_T *this)
{
	nic_pwr(0);
	hx_delay_ms(500);
	nic_pwr(1);
	hx_delay_ms(1000);
	
	nic_rst(0);
	hx_delay_ms(500);
	nic_rst(1);
	hx_delay_ms(1000);
	return 0;
}

static const struct NET_PARAM_T defprm = {
	.apn = "cmnet",
	.rm_ip = {180,89,58,27},
	.rm_port = 9020,
};
const struct HX_NIC_T nic_sim800c = {
	.default_param = &defprm,
	.at_tbl = at_tbl,
	.at_tbl_count = sizeof(at_tbl)/sizeof(at_tbl[0]),
	.init = _init,
};

