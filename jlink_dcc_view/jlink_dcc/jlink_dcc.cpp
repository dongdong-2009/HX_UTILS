// jlink_dcc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <typeinfo>
#include <conio.h>

#include "tlhelp32.h" 

int GetProcessCount(const TCHAR* szExeName)
{
	TCHAR sztarget[MAX_PATH];
	lstrcpy(sztarget, szExeName);
	CharLowerBuff(sztarget, MAX_PATH);

	int count = 0;
	PROCESSENTRY32 my;
	HANDLE l = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (((int)l) != -1)
	{
		my.dwSize = sizeof(my);
		if (Process32First(l, &my))
		{
			do
			{
				CharLowerBuff(my.szExeFile, MAX_PATH);
				if (lstrcmp(sztarget, my.szExeFile) == 0)
				{
					count++;
				}
			} while (Process32Next(l, &my));
		}
		CloseHandle(l);
	}

	return count;
}

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
volatile BOOL in_api = 0;
DWORD WINAPI ThreadFun(LPVOID pM)
{
	UINT32 res;
	while (1) {
		do {
			unsigned char c;
			res = fread(&c, 1, 1, stdin);
			if (res == 1) {
				//UINT32 c = getc(stdin);// getchar();	//getch(); no echo
				UINT32 data = c;
				while (in_api);
				in_api = 1;
				res = JLINKARM_WriteDCC(&data, 1, 50);
				in_api = 0;
			}
			else 
			{
				res = -1;
			}
		} while (res == 0);
	}
}
int main()
{
	printf("jlink_dcc_view_V0.1 by houxd ,build %s %s\r\n",__DATE__,__TIME__);
	int res;
	HINSTANCE lib = LoadLibrary("JLinkARM.dll");
	if (lib == NULL) {
		printf("Cannot Load Dll JLinkARM.dll \r\n");
		getchar();
		return -1;
	}
	printf("Load Dll JLinkARM.dll Success \r\n");

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
	printf("Open JLink ... ");

	JLINKARM_Open();
	if (JLINKARM_IsOpen())
	{
		printf("OK\r\n");

		printf("JLink Info:\r\n");
		printf("SN=%08u\r\n", JLINKARM_GetSN());
		printf("ID=%08X\r\n", JLINKARM_GetId());
		printf("VER=%u\r\n", JLINKARM_GetDLLVersion());
		printf("Speed=%u\r\n", JLINKARM_GetSpeed());
		printf("=============================================================\r\n");
		printf("Jlink DCC View Ver0.1, Build %s %s\r\n", __DATE__, __TIME__);

		HANDLE handle = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);
		//JLINKARM_Close();
		UINT32 buf[1024];
		while (1) {

			//GetProcessCount()
			//while (JLINKARM_IsOpen());
			//JLINKARM_Open();
			while (in_api);
			in_api = 1;
			res = JLINKARM_ReadDCC(buf, 1024, 2);
			in_api = 0;
			//JLINKARM_Close();
			if (res) {
				//printf("get dcc size=%d, data=%08X\r\n",res,buf[0]);
				for (int i = 0; i < res; i++) {
					UINT32 data = buf[i];
					int ch = (data >> 8) & 0xFFu;
					int chbak = change_ch(ch);
					int c = data & 0xFFu;
					if (ch >= 0 && ch <= 0x7F) {
						putchar(c);
					}
					change_ch(chbak);
				}
			}
			//if (_kbhit()) {
			//	UINT32 res = getchar();// getchar();	//getch(); no echo
			//	res = JLINKARM_WriteDCC(&res, 1, 50);
			//	if (res == 1) {
			//		
			//	}
			//}

		}

		WaitForSingleObject(handle, INFINITE);

	}
	//getchar();
	//JLINKARM_Close();
	return 0;
}
