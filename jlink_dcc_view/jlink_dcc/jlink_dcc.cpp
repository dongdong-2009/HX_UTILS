// jlink_dcc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <typeinfo>
#include <conio.h>
#include "tlhelp32.h" 
#include<signal.h>
#include "direct.h"
#include <time.h>

void (*JLINKARM_Open)(void);
void(*JLINKARM_Close)(void);
bool (*JLINKARM_IsOpen)(void);
UINT32 (*JLINKARM_GetSN)(void);
UINT32 (*JLINKARM_GetId)(void);
UINT32 (*JLINKARM_GetDLLVersion)(void);
UINT32 (*JLINKARM_GetSpeed)(void);
UINT32 (*JLINKARM_ReadDCC)(UINT32 *buf, UINT32 size, INT timeout);
UINT32 (*JLINKARM_WriteDCC)(const UINT32 *buf, UINT32 size, INT timeout);
UINT32 (*JLINKARM_ReadDCCFast)(UINT32 *buf, UINT32 size, INT timeout);
UINT32 (*JLINKARM_WriteDCCFast)(const UINT32 *buf, UINT32 size, INT timeout);
UINT32 (*JLINKARM_WaitDCCRead)(UINT32 timeout);
void (*JLINKARM_SetSpeed)(UINT32 spd);
UINT32(*JLINKARM_ExecCommand)(const char* cmd,UINT32 a,UINT32 b);

int load_jlinkarm_dll(void)
{
	HINSTANCE lib = LoadLibrary("JLinkARM.dll");
	if (lib == NULL) {
		return -1;
	}

	JLINKARM_Open = (void(*)(void))(GetProcAddress(lib, "JLINKARM_Open"));
	JLINKARM_Close = (void(*)(void))(GetProcAddress(lib, "JLINKARM_Close"));
	JLINKARM_IsOpen = (bool(*)(void))GetProcAddress(lib, "JLINKARM_IsOpen");
	JLINKARM_GetSN = (UINT32(*)(void))GetProcAddress(lib, "JLINKARM_GetSN");
	JLINKARM_GetId = (UINT32(*)(void))GetProcAddress(lib, "JLINKARM_GetId");
	JLINKARM_GetDLLVersion = (UINT32(*)(void))GetProcAddress(lib, "JLINKARM_GetDLLVersion");
	JLINKARM_GetSpeed = (UINT32(*)(void))GetProcAddress(lib, "JLINKARM_GetSpeed");
	JLINKARM_ReadDCC = (UINT32(*)(UINT32 *, UINT32, INT))GetProcAddress(lib, "JLINKARM_ReadDCC");
	JLINKARM_WriteDCC = (UINT32(*)(const UINT32 *, UINT32, INT))GetProcAddress(lib, "JLINKARM_WriteDCC");
	JLINKARM_ReadDCCFast = (UINT32(*)(UINT32 *, UINT32, INT))GetProcAddress(lib, "JLINKARM_ReadDCCFast");
	JLINKARM_WriteDCCFast = (UINT32(*)(const UINT32 *, UINT32, INT))GetProcAddress(lib, "JLINKARM_WriteDCCFast");
	JLINKARM_WaitDCCRead = (UINT32(*)(UINT32))GetProcAddress(lib, "JLINKARM_WaitDCCRead");
	JLINKARM_SetSpeed = (void(*)(UINT32))GetProcAddress(lib, "JLINKARM_SetSpeed");
	JLINKARM_ExecCommand = (UINT32(*)(const char *, UINT32, UINT32))GetProcAddress(lib, "JLINKARM_ExecCommand");

	return 0;
}


/*
FOREGROUND_BLUE 前景色为蓝
FOREGROUND_GREEN 前景色为绿
FOREGROUND_RED 前景色为红
FOREGROUND_INTENSITY 前景色为强光
BACKGROUND_BLUE 背景色为蓝
BACKGROUND_GREEN 背景色为绿
BACKGROUND_RED 背景色为红
BACKGROUND_INTENSITY 背景色为强光
*/
#define FORGOUND_WHITE	(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BACKGOUND_WHITE	(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
int change_ch(int ch)
{
	int res;
	static int last = 0;
	UINT color_tbl[] = {
		//字体亮
		FOREGROUND_INTENSITY | FORGOUND_WHITE,	//白
		FOREGROUND_INTENSITY | FOREGROUND_RED,	//红
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,	//绿
		FOREGROUND_INTENSITY | FOREGROUND_BLUE,	//蓝
		FORGOUND_WHITE,	//白
		FOREGROUND_RED,	//红
		FOREGROUND_GREEN,	//绿
		FOREGROUND_BLUE,	//蓝
		//背景亮
		BACKGROUND_INTENSITY | BACKGOUND_WHITE,	//白
		BACKGROUND_INTENSITY | BACKGROUND_RED,
		BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		BACKGOUND_WHITE,	//白
		BACKGROUND_RED,
		BACKGROUND_GREEN,
		BACKGROUND_BLUE,
	};
	if (ch == 'w')ch = 0;
	if (ch == 'r')ch = 1;
	if (ch == 'g')ch = 2;
	if (ch == 'b')ch = 3;
	if (ch == 'W')ch = 8;
	if (ch == 'R')ch = 9;
	if (ch == 'G')ch = 10;
	if (ch == 'B')ch = 11;
	if (ch >= sizeof(color_tbl) / sizeof(UINT))
		return last;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_tbl[ch]);
	res = last;
	last = ch;
	return res;
}
void dcc_send(int c)
{
	UINT32 res;
	UINT32 data = 0x55000000u | c;
	//while (in_api);
	//in_api = 1;
	int ts = 20;
	while (--ts > 0) {
		res = JLINKARM_WriteDCC(&data, 1, 50);
		if (res == 1) {
			break;
		}
	}
}

FILE* flog;
time_t log_time;
DWORD log_tickcount;
void log_putchar(int c)
{
	if (flog == NULL)
		return;
	fputc(c, flog);
	if (c == '\n') {
		DWORD n = GetTickCount()- log_tickcount;
		time_t t = log_time + n / 1000;
		int ms = n % 1000;
		tm tm;
		localtime_s(&tm, &t);
		fprintf(flog, "[%02u:%02u:%02u:%03u]\t",
			tm.tm_hour,tm.tm_sec,tm.tm_sec,ms);
	}
	fflush(flog);
}
//volatile BOOL in_api = 0;
DWORD WINAPI ThreadFun(LPVOID pM)
{
	UINT32 res;
	while (1) {
		unsigned char c = _getch();
		if (c == 0xE0) {
			//^V<> HPKM


			c = _getch();
			switch (c)
			{
			case 'H':	//u
				dcc_send('\x1b');
				dcc_send('[');
				dcc_send('A');
				break;
			case 'P':	//d
				dcc_send('\x1b');
				dcc_send('[');
				dcc_send('B');
				break;
			case 'M':	//l
				dcc_send('\x1b');
				dcc_send('[');
				dcc_send('C');
				break;
			case 'K':	//r
				dcc_send('\x1b');
				dcc_send('[');
				dcc_send('D');
				break;
				//case 'I':	//pu
				//	dcc_send('D');
				//	break;
				//case 'Q':	//pd
				//	dcc_send('Q');
				//	break;
				//case 'G':	//home
				//	dcc_send('G');
				//	break;
				//case 'O':	//end
				//	dcc_send('O');
				//	break;
			default:
				break;
			}
		}
		else
		{
			dcc_send(c);
			//printf("input: [%c] %02X\r\n", c, (int)c);

			//in_api = 0;
		}
	}
}
int main(int argc, char *argv[])
{
	signal(SIGINT, 0);
	printf("jlink_dcc_view_V0.3 by houxd ,build %s %s\r\n",__DATE__,__TIME__);
	int res;
	res = load_jlinkarm_dll();
	if (res) {
		printf("Cannot Load Dll JLinkARM.dll \r\n");
		getchar();
		return -1;
	}
	printf("Load Dll JLinkARM.dll Success \r\n");

	char *self = argv[0];
	char log_file[512] = { 0 };
	{
		strcpy_s(log_file, 512, argv[0]);
		
		time_t t;
		time(&t);
		tm tm;
		localtime_s(&tm, &t);
		char log_fname[512];
		sprintf_s(log_fname, "\\log-%04u%02u%02u.txt",
			tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);
		char *p = strrchr(log_file, '\\');
		if (p) {
			strcpy_s(p, 512, log_fname);
		}
		else {
			strcpy_s(log_file, 512, "\\jlinkdcc_log.txt");
		}
	}
	fopen_s(&flog,log_file, "a+");
	if (flog == NULL) {
		printf("WARNING: log file can not creat.\r\n");
	}

	char config_file[512] = { 0 };
	{	
		strcpy_s(config_file, 512, argv[0]);
		char *p = strrchr(config_file, '\\');
		if (p) {
			strcpy_s(p, 512, "\\jlinkarm_cfg.ini");
		}
		else {
			strcpy_s(config_file, 512, "jlinkarm_cfg.ini");
		}
	}


	char device[64]="";
	GetPrivateProfileString("JLINKARM", "device", "", device, 64, config_file);
	UINT32 speed = GetPrivateProfileInt("JLINKARM", "speed", 200, config_file);

	printf("Open JLink ... ");
	JLINKARM_Open();
	if (JLINKARM_IsOpen())
	{
		printf("OK\r\n");
		if (device[0]) {
			char buff[128];
			sprintf_s(buff, "device = %s", device);
			JLINKARM_ExecCommand(buff, 0, 0);
		}
		else {
			printf("Not config device, Press Any Key to setlect.\r\n");
			_getch();
		}
		JLINKARM_SetSpeed(speed);

		printf("JLink Info:\r\n");
		printf("DEVICE = %s\r\n", device);
		printf("SN = %08u\r\n", JLINKARM_GetSN());
		printf("ID = %08X\r\n", JLINKARM_GetId());
		printf("VER = %u\r\n", JLINKARM_GetDLLVersion());
		printf("Speed = %u\r\n", JLINKARM_GetSpeed());
		printf("=============================================================\r\n");

		HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);
		UINT32 buf[256];
		fprintf(flog, "------------------------------------------------------------\r\n");
		log_tickcount = GetTickCount();
		time(&log_time);
		log_putchar('\n');	//record a time label
		while (1) {
			//while (in_api);
			//in_api = 1;
			res = JLINKARM_ReadDCC(buf, 256, 2);
			//in_api = 0;
			if (res) {
				for (int i = 0; i < res; i++) {
					UINT32 data = buf[i];
					if ((data>>24) != 0x54)
						continue;
					int ch = (data >> 8) & 0xFFu;
					int chbak = change_ch(ch);
					int c = data & 0xFFu;
					if (ch >= 0 && ch <= 0x7F) {
						putchar(c);
						log_putchar(c);
					}
					change_ch(chbak);
				}
			}
			Sleep(10);
		}

		WaitForSingleObject(handle, INFINITE);
		JLINKARM_Close();
	}

	return 0;
}
