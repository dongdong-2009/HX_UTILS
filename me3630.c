#include "hx_utils.h"
#include "hx_board.h"
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

static int on_csq(
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
static int check_zipcall(
	int step, 
	enum ATEVENT_T event,
	char *buf, 
	void *msg)
{
	int id;
	sscanf(buf,"+ZIPCALL: %d,",&id);
	if(id>0){
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
static const struct ATCMD_T at_tbl[] = {
	//cmd					res			timeout		trytimes	event_proc
	{"ATE1",				"OK",		2000,		20, 		0},
	{"ATE0",				"OK",		2000,		20, 		0},
	{"AT+CSQ",				NULL,		2000,		20, 		on_csq},
	{"AT+ZSNT=0,0,0",		"OK",		2000,		5,			0},
	//{"AT+CEREG?",			NULL,		2000,		10,			on_cereg},
	//{"AT+ZPAS?",			NULL,		2000,		5,			on_zpas},
	
	//{"AT+CGDCONT=1,\"IP\",\"CMNET\"",		
	//						"OK",		2000,		5, 			0},
	
	{"AT+ZIPCFG=cmnet",		"OK",		2000,		5,			0},
	{"AT+ZIPCALL=1",		NULL,		3000,		10,			check_zipcall,},
	ATCMD_DELAY(3000),
	{"AT+ZIPOPEN=1,0,180.89.58.27,9020",
							NULL,		3000,		5,			check_zipstat},	
};

static const HX_ATARG_T defarg = {
	.rm_ip = "180.89.58.27",
	.rm_port = 9020,
	.apn = "cmnet",
	.user = "",
	.passwd = "",
};

static int _init(const struct HX_NIC_T *this, int *pstep, HX_ATARG_T *arg)
{
	atc_default_init(this,pstep,arg);
	#if defined(BRD_NIC_PWR) && defined(BRD_NIC_RST)
		printf("sim7100c init.\n");
		brd_ioctrl(BRD_NIC_PWR,1);	//default
		brd_ioctrl(BRD_NIC_RST,1);
		
		brd_iomode(BRD_NIC_PWR,IM_OUT);	//output
		brd_iomode(BRD_NIC_RST,IM_OUT);
		
		brd_ioctrl(BRD_NIC_PWR,0);	//power low 500
		hx_delay_ms(500);
		brd_ioctrl(BRD_NIC_PWR,1);
		hx_delay_ms(1000);
		brd_ioctrl(BRD_NIC_RST,0);	//rst low 500
		hx_delay_ms(500);
		brd_ioctrl(BRD_NIC_RST,1);
		hx_delay_ms(1000);
	#else
		#error ***No Define Hardware Port, Might be not Work! 
	#endif
	return 0;
}

const struct HX_NIC_T nic_me3630 = {
	.default_arg = &defarg,
	.at_tbl = at_tbl,
	.at_tbl_count = sizeof(at_tbl)/sizeof(at_tbl[0]),
	.init = _init,
};
