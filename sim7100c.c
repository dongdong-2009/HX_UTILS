#include "hx_atcmd.h"
#include "hx_utils.h"
#include "string.h"
#include "stdio.h"

/*
========================================================
//����ģʽ
AT+CRESET 
AT+CSQ
AT+NETCLOSE
AT+CGDCONT=1,"IP","apn"
AT+CGSOCKCONT=1,"IP","CMNET"
AT+CSOCKSETPN=1
AT+CIPMODE=0
AT+NETOPEN
AT+CIPOPEN=3,"TCP","119.75.218.70",80
AT+CIPSEND=3,5
>
[OK]

//͸��ģʽ
AT+CRESET 
AT+CSQ
AT+NETCLOSE
AT+CGDCONT=1,"IP","apn"
AT+CGSOCKCONT=1,"IP","CMNET"
AT+CSOCKSETPN=1
AT+CIPMODE=1
AT+NETOPEN
AT+CIPOPEN=0,"TCP","119.75.218.70",80
AT+CIPSEND=3,5

AT+CIPOPEN=0,"TCP","180.89.58.27",9020


==========================================================
//ping ����
AT+CPING="www.baidu.com",1,4,64,1000,10000,255
AT+CPING="118.186.199.151",1,4,64,1000,10000,255
AT+CPING="www.qq.com",1,4,64,1000,10000,255
AT+CPING="119.75.218.70",1,4,64,1000,10000,255
AT+CPING="www.tcps.com.cn",1,4,64,1000,10000,255



//telnet����
telnet 118.186.199.151 9020
===============================================================
C:\Users\houxd>ping www.qq.com

���� Ping www.qq.com [182.254.18.159] ���� 32 �ֽڵ�����:
���� 182.254.18.159 �Ļظ�: �ֽ�=32 ʱ��=10ms TTL=50
���� 182.254.18.159 �Ļظ�: �ֽ�=32 ʱ��=10ms TTL=50
���� 182.254.18.159 �Ļظ�: �ֽ�=32 ʱ��=9ms TTL=50
���� 182.254.18.159 �Ļظ�: �ֽ�=32 ʱ��=9ms TTL=50

182.254.18.159 �� Ping ͳ����Ϣ:
    ���ݰ�: �ѷ��� = 4���ѽ��� = 4����ʧ = 0 (0% ��ʧ)��
�����г̵Ĺ���ʱ��(�Ժ���Ϊ��λ):
    ��� = 9ms��� = 10ms��ƽ�� = 9ms

C:\Users\houxd>ping www.baidu.com

���� Ping www.a.shifen.com [119.75.218.70] ���� 32 �ֽڵ�����:
���� 119.75.218.70 �Ļظ�: �ֽ�=32 ʱ��=14ms TTL=53
���� 119.75.218.70 �Ļظ�: �ֽ�=32 ʱ��=7ms TTL=53
���� 119.75.218.70 �Ļظ�: �ֽ�=32 ʱ��=8ms TTL=53
���� 119.75.218.70 �Ļظ�: �ֽ�=32 ʱ��=8ms TTL=53

119.75.218.70 �� Ping ͳ����Ϣ:
    ���ݰ�: �ѷ��� = 4���ѽ��� = 4����ʧ = 0 (0% ��ʧ)��
�����г̵Ĺ���ʱ��(�Ժ���Ϊ��λ):
    ��� = 7ms��� = 14ms��ƽ�� = 9ms

*/

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
	if(strstr(buf,"FAIL"))	//CONNECT FAIL
		return -1;
	if(strstr(buf,"ERROR"))	//CONNECT ERROR
		return -2;			
	if(strstr(buf,"CONNECT"))	//CONNECT 115200 / ALREADY CONNECT etc..
		return 0;
	return -1;
}

static int check_netopen(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	int res;
	if(buf[0] == '+'){
		int a=-1;
		res = sscanf(buf,"+NETOPEN: %d",&a);
		if(res==1 && a==0)
			return 0;
	}
	return -1;
}

const struct ATCMD_T sim7100c_at_tbl[] = {
	//cmd					res			timeout		trytimes	check_res_proc
	{"AT",					"AT",		2000,		20, 		0},
	{"ATE0",				"OK",		2000,		20, 		0},
	{"AT+CSQ",				NULL,		2000,		20, 		check_csq},
	{"AT+CGDCONT=1,\"IP\",\"apn\"",		
							"OK",		2000,		5, 			0},
	{"AT+CGSOCKCONT=1,\"IP\",\"CMNET\"",		
							"OK",		2000,		5, 			0},
	{"AT+CSOCKSETPN=1",		"OK",		2000,		5, 			0},
	{"AT+CIPMODE=1",		"OK",		2000,		5, 			0},
	ATCMD_DELAY(2000),
	{"AT+NETOPEN",		 	NULL,		2000,		5, 			check_netopen},
	{"AT+CIPOPEN=0,\"TCP\",\"180.89.58.27\",9020",	
							NULL/*"CONNECT"*/,	30000,5, 		check_connect},
};

const int sim7100c_step_count = sizeof(sim7100c_at_tbl)/sizeof(sim7100c_at_tbl[0]);

//return 0 is connect ,others are not
int sim7100c_poll(void)
{
	char buf[4096];
	int step = atc_sequence_poll(
					sim7100c_at_tbl,
					sim7100c_step_count,
					buf,
					4096,
					NULL);
	if(step < sim7100c_step_count){
		return step;
	}else{
		return -1;
	}
}

