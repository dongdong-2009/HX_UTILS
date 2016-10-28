/*
 * hx_term_cmds.c
 *
 *  Created on: 2016ƒÍ7‘¬25»’
 *      Author: houxd
 */

#include "hx_utils.h"
#include "hx_board.h"
#include "stdio.h"
#include "string.h"


/*
	Board Params need
*/
extern const DEV_PARAM_T g_param_tbl[];
extern const int g_param_count;
extern int HX_PARAM_LOADSTORE(int load0store1);


//void asc_to_hex(const void* asc,int len,void *hex)
//{
//	const unsigned char *_asc = asc;
//	unsigned char *_bin = hex;
//	for(int i=0;i<len;i+=2)
//	{
//		unsigned int h = _asc[i];
//		h = h>='A'?h-'A'+0xA:h-'0';
//		unsigned int l = _asc[i+1];
//		l = l>='A'?l-'A'+0xA:l-'0';
//		unsigned int res = (h<<4) + (0xFu & l);
//		_bin[i/2] = (unsigned char) res;
//		
//	}
//}


////#include "tk_typedef.h"
//extern tk_uint16 uiSMTCARD_RC_LEN;	//psam recieve data length
//extern tk_uint8 ucSMTCARD_RC_DATA[256];	//psam recieve data buffer
//extern size_t hex_to_asc(const void* hex,size_t hex_len,void *asc);
//HXT_DEF_PROC(psam)
//{
//	char buf[MAX_CHARS_LINE];
//	memset(buf,0,MAX_CHARS_LINE);
//	if(argc<2){
//		hxt_puts("need arguements!");
//		return -1;
//	}
//	int l = strlen(argv[1]);
//	if(l>=MAX_CHARS_LINE/2)
//		return -2;
//	asc_to_hex(argv[1],l,buf);
//	
//	extern tk_uint16 tkSmartCardCmdBase(tk_int16 iTrLen,tk_uint8 * ucpTrData);
//	int res = tkSmartCardCmdBase(l/2,(void*)buf);
//	hex_to_asc(ucSMTCARD_RC_DATA,uiSMTCARD_RC_LEN,buf);
//	buf[uiSMTCARD_RC_LEN*2] = 0;
//	hxt_printf("0x%04X\t%s\r\n",res,ucSMTCARD_RC_DATA);
//	return 0;
//}

//int hxt_strtoi(const char *s)
//{
//	int res;
//	if(*s=='0' && (*(s+1)=='x'||*(s+1)=='X')){
//		sscanf(s+2,"%x",&res);
//	}else{
//		sscanf(s,"%d",&res);
//	}
//	return res;
//}
//#include "type.h"


//#include "LPC23xx.h"                        /* LPC23xx/24xx definitions */
//#include "type.h"
//#include "wtv.h"
//#include "wdt.h"

//#include "led.h"
//#include "rtc.h"
//#include "Disp.h"

//#include "tkHeaderFile.h"
//#define SST1_CS_H	    IOSET1|=(1<<14)
//#define SST1_CS_L	    IOCLR1|=(1<<14)//FLASH_CS1∆¨—°
//#define SST2_CS_H	    FIO4SET|=(1<<29)
//#define SST2_CS_L	    FIO4CLR|=(1<<29)//FLASH_CS2∆¨—°
//#define SST_HOLD_H	  IOSET1|=1<<15
//#define SST_HOLD_L	  IOCLR1|=1<<15//FLASH_RST±£≥÷”––ß
//#define SST_SCK_H	    IOSET1|=1<<16
//#define SST_SCK_L	    IOCLR1|=1<<16// ±÷”
//#define SST_SO        IOPIN1&(1<<10)// ‰»Î
//#define SST_SI_H	    IOSET1|=1<<17
//#define SST_SI_L	    IOCLR1|=1<<17// ‰≥ˆ




//#define	READ_CMD	0x03	//Read Memory
//#define	HIGH_READ_CMD	0x0b	//Read Memory at higher speed
//#define	ERASE_4K_CMD	0x20	//erase 4kbyte of memory array
//#define	ERASE_32K_CMD	0x52	//erase 32kbyte block of memory array
//#define	ERASE_64K_CMD	0xd8	//erase 64kbyte block of memory array
//#define	CHIP_ERASE_CMD	0x60	//erase full memory array
//#define	BYTE_PROGRAM_CMD	0x02	//to program one data byte
//#define	AAI_WORD_PROGRAM_CMD	0xad	//auto address increment programming
//#define	RDSR_CMD	0x05	//read status register
//#define	EWSR_CMD	0x50	//enable write status register
//#define	WRSR_CMD	0x01	//write status register
//#define	WREN_CMD	0x06	//write enable
//#define	WRDI_CMD	0x04	//write disable
//#define	RDID_CMD	0x90	//read ID
//#define	JEDEC_ID_CMD	0x9f	//JEDEC ID read
//#define	EBSY_CMD	0x70	//enable so as an output RY/BY# status during AAI programming
//#define	DBSY_CMD	0x80	//disable so as an output RY/BY# status during AAI programming
//extern unsigned char SSTSPI_HostReadByte(void);
//extern void SSTSPI_HostWriteByte(unsigned char wByte);
//extern void SSTWait_Busy(void);

//int flash_begain_read(int blk,int ofs)
//{
//    DWORD pageaddr;
//    pageaddr=blk<<12;
//    pageaddr+=ofs;
//    SPI_Flash_CS_Low();
//    SSTSPI_HostWriteByte(HIGH_READ_CMD);
//    SSTSPI_HostWriteByte((unsigned char)(pageaddr>>16));
//    SSTSPI_HostWriteByte((unsigned char)(pageaddr>>8));
//    SSTSPI_HostWriteByte((unsigned char) pageaddr);
//    SSTSPI_HostWriteByte(0xFF);			/*dummy byte*/
//	return 0;
//}
//int flash_read_data(int len,char *buf)
//{
//    for(int i=0; i<len; i++)
//    {
//        buf[i]=SSTSPI_HostReadByte();
//    }
//	return 0;
//}

//int flash_end_read(void)
//{
//    SPI_Flash_CS_High();
//    SSTWait_Busy();
//	return 0;
//}
//int flash_read_output(int blk,int ofs,int bs)
//{
//	if(blk>1024)
//		return -1;
//	if(ofs>1024)
//		return -2;
//	const int bytes_per_line = 16;
//	char buff[bytes_per_line];
//	flash_begain_read(blk,ofs);
//	for(int i=0;i<bs;i+=bytes_per_line){
//		flash_read_data(bytes_per_line,buff);
//		int n = bs - i;
//		if(n>bytes_per_line)
//			n = bytes_per_line;
//		for(int i=0;i<n;i++)
//			hxt_printf("%02X ",buff[i]);
//		hxt_printf("\r\n");
//	}
//	flash_end_read();
//	return 0;
//}

//extern void sstByte_Program(unsigned int PA,unsigned int BA, unsigned char byte);
//extern void sstFlashWrite(unsigned int PA,unsigned int BA,BYTE *pHEADER,WORD len);
//int flash_write_output(int blk,int ofs,const char *data)
//{
//	int l = strlen(data);
//	char c;
//	for(int i=0;i<l;i+=2){
//		asc_to_hex(data+i,2,&c);
//	}
//	sstFlashWrite(blk,ofs,(BYTE*)&c,1);
//	return 0;
//}

//extern void sstChip_Erase(void);
//extern void sstSector_Erase(unsigned int PA);
///*
//	blk = -1 is erase chip
//*/
//int flash_erase_output(int blk, int bs)
//{
//	if(blk<0){
//		sstChip_Erase();
//		return 0;
//	}
//	for(int i=0;i<bs&&blk<1024;i++)
//		sstSector_Erase(blk++);
//	return 0;
//}
///*
//	flash erase chip
//	flash e blk=0 blks=3
//	flash r blk=0 ofs=0 bs=128
//	flash w blk=0 ofs=0 00000000000000000000000
//*/

//int hxt_flash(int argc, char *argv[])
//{
//	int res;
//	int blk=-1,ofs=-1,bs=-1;
//	int rwe = -1;	//0r,1w,2e
//	char *data = 0;
//	int chip = 0;
//	char *argi;
//	if(argc<3) goto bad_arg;
//	
//	argi = argv[1];
//	switch(*argi){
//		case 'r':
//			if(*(argi+1) && strcmp(argi,"read")!=0) goto bad_arg;
//			rwe = 0;
//			break;
//		case 'w':
//			if(*(argi+1) && strcmp(argi,"write")!=0) goto bad_arg;
//			rwe = 1;
//			break;
//		case 'e':
//			if(*(argi+1) && strcmp(argi,"erase")!=0) goto bad_arg;
//			rwe = 2;
//			break;
//		default:
//			goto bad_arg;
//	}
//	for(int i=2;i<argc;i++){
//		argi = argv[i];
//		// arguement is data ? 01AB etc...
//		int c = *argi;
//		if(rwe==1 && !data){
//			if((c>='0' && c<='9') || (c>='A' && c<='F')){
//				data = argi;
//				continue;
//			}
//		}
//		
//		//arguement is chip
//		if(rwe==2 && strcmp(argi,"chip")==0){
//			chip = 1;
//			continue;
//		}
//		
//		//check is format: v=d
//		char *v = strtok(argi,"=");
//		char *d = strtok(NULL,"=");
//		if(v&&d){
//			if(strcmp(v,"blk")==0) blk = hxt_strtoi(d);
//			else if(strcmp(v,"ofs")==0) ofs = hxt_strtoi(d);
//			else if(strcmp(v,"bs")==0) bs = hxt_strtoi(d);
//			else goto bad_arg;
//			continue;
//		}
//		
//		//what arg???
//		goto bad_arg;
//	}
//	
//	switch(rwe){
//		case 0:	//read
//			if(blk<0||chip||data)
//				goto bad_arg;
//			if(ofs<0) ofs = 0;
//			if(bs<0) bs = 16;	//default
//			res = flash_read_output(blk,ofs,bs);
//			if(res) goto bad_arg;
//			return 0;
//		case 1:	//write
//			if(blk<0||data==0||chip)
//				goto bad_arg;
//			if(ofs<0) ofs = 0;
//			res = flash_write_output(blk,ofs,data);
//			if(res) goto bad_arg;
//			return 0;
//		case 2:	//erase
//			if(chip &&(blk>0||ofs>0||data))		//need chip or blk
//				goto bad_arg;
//			if(!chip && (blk<0||ofs>0||data))		//need blk of blk
//				goto bad_arg;
//			if(bs<0)
//				bs = 1;
//			// flash erase chip : blk=-1 is .
//			res = flash_erase_output(blk,bs);
//			if(res) goto bad_arg;
//			return 0;
//		default:
//			goto bad_arg;
//	}
//	
//bad_arg:	
//	hxt_puts(" Usage:");
//	hxt_puts("\t flash <r|read> <blk=value> [ofs=value] [bs=value]");
//	hxt_puts("\t flash <w|write> <blk=value> <ofs=value> <data>");
//	hxt_puts("\t flash <e|erase> <blk=value> [bs=value]");
//	hxt_puts("\t flash <e|erase> chip");
//	return -2;
//}

////==================================================
///*

/*
	demo a b=0 c=
*/
int hxt_demo(int argc, char *argv[])
{
//	paras_param(argc,argv,&param_tbl);
	return 0;
}

/*
	<fmt>,count,split-chars
	bcd n .		"\x20\x16"		<=>	"2016"		this not surpport 
												count ,split-chars .
	bin											like before
	asc 5		'houxd123'		<=>	"houxd"		
	i 4 .		[127][0][0][1]	<=> "127.0.0.1"
	%d			100	(DWORD)			<=>	"100"
	%hd/%hu		WORD
	%hhd/%hhu		BYTE
	%ld/%lu		QWORD
	msb 2/4/8
	lsb 2/4/8
*/

HXT_DEF_PROC(param)
{
	int res;
	res = HX_PARAM_LOADSTORE(0);
	if(res){
		hxt_printf("*** Load Params Return Error %d\r\n",res);
		return -1;
	}
	const DEV_PARAM_T *pm;
	foreach(g_param_tbl,g_param_count,pm){
		char *arg;
		hx_get_arg(argc,argv,pm->name,&arg);
		if(argc<=1){
			//show
			char buf[MAX_CHARS_LINE];
			hx_value2str(pm->data,pm->data_type,buf);
			hxt_printf("%s=%s\r\n",pm->name,buf);
		}else{
			if(arg){
				//change
				res = hx_str2value(arg,pm->data_type,pm->data);
				if(res){
					hxt_printf("Change Param Error: %s,type:%s\r\n",
						pm->name,pm->data_type);
					goto bad_arg;
				}
			}
			
		}
	}
	if(argc>1){
		res = HX_PARAM_LOADSTORE(1);
		if(res){
			hxt_printf("*** Load Params Return Error %d\r\n",res);
			return -1;
		}
	}
	return 0;
bad_arg:
	hxt_puts("Usage:");
	hxt_puts("\t param	: show target device params or change theam");
	return 0;
}




#define HXT_DEFAULT_UART_PARAM			"115200,8n1"
HXT_DEF_PROC(devcfg)
{
	int res;
	char *dev = NULL;
	char *uart_param = NULL;
	HX_GET_ARG(dev);
	HX_GET_ARG(uart_param);
	if(!dev || !uart_param)
		goto bad_arg;
	if(memcmp(dev,"uart",4)==0)
	{
		int uart_id;
		sscanf(dev,"uart%d",&uart_id);
		if(uart_id<0 || uart_id >= UART_PORTS_NUM){
			HXT_PARAM_ERROR(dev);
			hxt_puts("no this device.");
			goto bad_arg;
		}
		int bps;
		res = sscanf(uart_param,"%d,%*s",&bps);
		if(res == 1){
			hx_uart_init(uart_id,bps,hx_get_gpclk());
		}
	}
	return 0;
bad_arg:
	if(strcmp(argv[0],"devcfg"))
		return -1;
	hxt_puts("Usage:");
	hxt_puts("\t devcfg dev=<uart0|uart1|...etc>  uart_param=<bps>[,8n1]");
	return -1;
}
static void next_value(char **s, char *array)
{
	char *ss = *s;
	char *f = strtok(array,",");
	if(ss){
		do{
			if(strcmp(ss,f)==0){
				*s = strtok(NULL,",");
				return;
			}
			f = strtok(NULL,",");
		}while(f);
		*s = NULL;	
		return;
	}else{
		*s = f;
	}
}
HXT_DEF_PROC(link)
{
	int res;
	char *dev = NULL;
	int hexmode = 0;
	char *uart_param = NULL;
	int echo = 0;
	char *oc = NULL;
	char *ic = NULL;
	int linemode = 0;
	
	int dev_idx = HX_GET_ARG(dev);
	HX_GET_ARG_BOOL(hexmode);
	HX_GET_ARG(uart_param);
	HX_GET_ARG_BOOL(echo);
	HX_GET_ARG(oc);
	HX_GET_ARG(ic);
	HX_GET_ARG_BOOL(linemode);
	
	if(argc<2)
		goto bad_arg;
	
	if(!dev) goto bad_arg;
	if(uart_param)
		res = hxt_devcfg(argc,argv);
	else{
		uart_param = "115200,8n1";
		char *argv2[3] = {
			"link",argv[dev_idx],"uart_param=115200,8n1",};
		res = hxt_devcfg(3,argv2);
	}
	if(res){
		HXT_PARAM_ERROR(uart_param);
		goto bad_arg;
	}
	
	if(memcmp(dev,"uart",4)==0)
	{
		int uart_id;
		sscanf(dev,"uart%d",&uart_id);
		if(uart_id<0 || uart_id >= UART_PORTS_NUM){
			HXT_PARAM_ERROR(dev);
			hxt_puts("no this device.");
			goto bad_arg;
		}
		hxt_printf("link to %s , %s , %s \r\n",
			dev,uart_param?uart_param:("uart_param=" HXT_DEFAULT_UART_PARAM),
		hexmode?"hex mode":"bin mode");
		int c=0;
		char hex[3]={0};
		int n=0;
		while(1){
			//dev->console
			res = hx_uart_getc_noblock(uart_id,&c);
			if(res==0){
				if(hexmode)
					hxt_printf("%02X",c);
				else{
					hxt_putc(c);
					if(ic){
						if(c=='\r' && strcmp(ic,"crlf")==0)
							hxt_putc('\n');
						else if(c=='\n' && strcmp(ic,"lfcr")==0)
							hxt_putc('\r');
					}
				}
			}
			//console->dev
			if(linemode){
				char *line = 0;
				do{
					res = hxt_gets_echo_noblock(&line);
					if(IS_CTRL_CHAR(-res)){
						c = -res;
						goto config;
					}
					if(res>=0 && line){
						if(hexmode){
							int l = strlen(line);
							char buff[MAX_CHARS_LINE/2];
							hx_hexcode2bin(line,l,buff);
							hx_uart_send(uart_id,buff,l>>1);
							
						}else{
							hx_uart_put(uart_id,line);
						}
						if(oc){
							if(strcmp(oc,"cr")==0)
								hx_uart_put(uart_id,"\r");
							else if(strcmp(oc,"lf")==0)
								hx_uart_put(uart_id,"\n");
							else if(strcmp(oc,"crlf")==0)
								hx_uart_put(uart_id,"\r\n");
							else if(strcmp(oc,"null")==0){
								;
							}else ;
						}else{
							if(!hexmode)
								//∑«hexƒ£ Ωƒ¨»œWindows∏Ò Ω ‰≥ˆ
								hx_uart_put(uart_id,"\r\n");
						}
					}
				}while(res>=0);
				continue;
			}
			res = hxt_getc_noblock(&c);
			if(res==0){
config:
				if(c==''){
					hxt_puts("exit.");
					return 0;
				}else if(CTRL2CHAR(c)=='X'){//c==0x08){	//ctrl+x
					hexmode = hexmode?0:1;
					hxt_printf("HEX Mode %s\r\n",hexmode?"Enable":"Disable");
					continue;
				}else if(CTRL2CHAR(c)=='E'){//c==0x05){	//ctrl+e
					echo = echo?0:1;
					hxt_printf("ECHO Mode %s\r\n",echo?"Enable":"Disable");
					continue;
				}else if(CTRL2CHAR(c)=='O'){//c==0x0F){	//ctrl+o
					char ll[] = "cr,lf,crlf,null";
					next_value(&oc,ll);
					if(hexmode)
						hxt_printf("Output End CR/LF Mode : %s\r\n",
							oc?oc:"NULL");
					else
						hxt_printf("Output End CR/LF Mode : %s\r\n",
							oc?oc:"CRLR");
					continue;
				}else if(CTRL2CHAR(c)=='I'){//c==0x09){	//ctrl+i
					char ll[] = "lfcr,crlf";
					next_value(&ic,ll);
					hxt_printf("Input LF/CR Convert Mode : %s\r\n",
						ic?ic:"No Convert");
					continue;	
				}else if(CTRL2CHAR(c)=='L'){//c==0x0C){	//ctrl+l
					linemode = linemode?0:1;
					hxt_printf("Line Mode %s\r\n",linemode?"Enable":"Disable");
					continue;
				}else if(IS_CTRL_CHAR(c)){
					hxt_printf("SET: %s, %s, %s, Output:%s, Input:%s \r\n",
						linemode?"LineMode:On":"LineMode:Off",
						hexmode?"Disp:Hex":"Disp:Binary",
						echo?"Echo:On":"Echo:Off",
						oc?oc:"NotAdd",
						ic?ic:"NotConvert"
						);
//					hxt_printf("(Ctrl+X) Hex Mode: %s\r\n",hexmode?"Enable":"Disable");
//					hxt_printf("(Ctrl+E) Echo Mode: %s\r\n",echo?"Enable":"Disable");
//					hxt_printf("(Ctrl+O) Output Line End Mode: %s\r\n",oc?oc:"NULL");
//					hxt_printf("(Ctrl+I) Input CRLR Convert Mode: %s\r\n",ic?ic:"No Convert");
//					hxt_printf("(Ctrl+L) Line Mode: %s\r\n",linemode?"Enable":"Disable");
					continue;
				}
					
				if(hexmode){
					if((c>='0' && c<='9')||(c>='A' && c<='F')||
						(c>='a' && c<='f'))
					{
						if(echo) hxt_putc(c);
						hex[n] = (unsigned char)c;
						n++;
						if(n==2){
							n=0;
							res = sscanf(hex,"%X",&c);
							if(res == 1){
								hx_uart_putc(uart_id,c);
								if(echo)hxt_putc(' ');
							}
						}
						
					}
					
				}else{
					hx_uart_putc(uart_id,c);
					if(echo) hxt_putc(c);
				}
				
			}
		}
	}
	return 0;
bad_arg:
	hxt_puts("Usage:");
	hxt_puts("\t link <dev=devname>    —°‘Ò¡¨Ω”µΩµƒ…Ë±∏");
	hxt_puts("\t      [hexmode]                (Ctrl+X) π”√ Æ¡˘Ω¯÷∆ ‰»Î/œ‘ æƒ£ Ω");
	hxt_puts("\t      [echo]                   (Ctrl+E)ªÿœ‘∑¢ÀÕ◊÷∑˚");
	hxt_puts("\t      [oc=cr|lf|crlf|null]     (Ctrl+O) ‰≥ˆ◊™ªª,––ƒ©Œ≤ÃÌº”øÿ÷∆∑˚,Ωˆ––ƒ£ Ω”––ß");
	hxt_puts("\t      [ic=lfcr|crlf]           (Ctrl+I) ‰»Î◊™ªª,LF->LFCR|CR->CRLF,Ωˆ∂˛Ω¯÷∆ƒ£ Ω”––ß");
	hxt_puts("\t      [linemode]               (Ctrl+L) π”√––ƒ£ Ω, ‰»Îªª––¥•∑¢“ª¥Œ∑¢ÀÕ");
	hxt_puts("\t      [uart_param=115200,8n1]  …Ë÷√uart≤®Ãÿ¬ ");
	return -1;
}

struct DEV_FILE_T
{
	const char *name;
	int (*operate)(int rwc, int len, void* buf);
};

const struct DEV_FILE_T dev_file_list[] = 
{
	{"null",   NULL,},
	//{"ne4110", ne4110_operate,},
};
HXT_DEF_PROC(write)
{
	int res=0;
	char *dev = NULL;
	char *asc = NULL;
	char *hex = NULL;
	char *wc = NULL;
	char *rc = NULL;
	HX_GET_ARG(dev);
	HX_GET_ARG(hex);
	HX_GET_ARG(asc);
	HX_GET_ARG(wc);
	HX_GET_ARG(rc);
	int size = sizeof(dev_file_list)/sizeof(dev_file_list[0]);
	if(!dev || (!asc || (asc ==hex)))
		goto bad_arg;
	
	const struct DEV_FILE_T *p;
	foreach(dev_file_list,size,p){
		if(strcmp(p->name,dev)==0){
			char buff[1024];
			int l;
			if(asc){
				l = strlen(asc);
				memcpy(buff,asc,l);
			}else{
				l = strlen(hex);
				hx_hexcode2bin(hex,l,buff);
				l >>=1;
			}
			if(wc){
				if(strcmp(wc,"lf")==0){
					buff[l++] = '\n';
				}else if(strcmp(wc,"cr")==0){
					buff[l++] = '\r';
				} else if(strcmp(wc,"crlf")==0){
					buff[l++] = '\r';
					buff[l++] = '\n';
				}
			}
			res = p->operate(1,l,buff);
			hxt_printf("write %d bytes.\r\n",l);
			if(res>0){
				char sres[1024];
				if(rc && strcmp(rc,"hex")==0){
					hx_dumphex2str(buff,res,sres);
				}else{
					memcpy(sres,buff,res);
					sres[res] = 0;
				}
				hxt_printf("return %d Bytes [%s]\r\n",res,sres);
			}else{
				hxt_printf("return Null\r\n");
			}
			return 0;
		}
	}
	hxt_puts("no this device file.");
	return 0;
bad_arg:
	hxt_puts("Usage:");
	hxt_puts("\t write <dev=devname> <asc=data|hex=data>\\");
	hxt_puts("\t       <wc=cr|lf|crlf> <rc=asc|hex>");
	return -1;
}

HXT_DEF_PROC(list)
{
	int size = sizeof(dev_file_list)/sizeof(dev_file_list[0]);
	const struct DEV_FILE_T *p;
	foreach(dev_file_list,size,p){
		hxt_puts(p->name);
	}
	return 0;
}

HXT_DEF_PROC(reset)
{
	((void (*)(void))0)();
	return 0;
}

HXT_DEF_PROC(clear)
{
	hxt_put(VT100_CLR);
	return 0;
}
HXT_DEF_PROC(cr)
{
	hxt_put(VT100_CLR);
	hxt_putc(' ');	//wait send complete
	((void (*)(void))0)();
	return 0;
}
int io_mode_ctrl(int argc, char *argv[])
{
	int res;
	if(argc<3)
		goto bad;
	unsigned x,y,v;
	res = sscanf(argv[1],"p%u.%u",&x,&y);
	if(res!=2) goto bad;
	res = sscanf(argv[2],"%u",&v);
	if(res!=1) goto bad;
	if(strcmp(argv[0],"ioctrl")==0)
		brd_ioctrl(x,(unsigned)(1<<y),v);
	else if(strcmp(argv[0],"iomode")==0)
		brd_iomode(x,(unsigned)(1<<y),(IOMODE_T)v);
	else
		goto bad;
	return 0;
bad:
	hxt_puts("Usage:");
	hxt_printf("\t %s p<x>.<y>\r\n",argv[0]);
	return -1;
}
	
HXT_DEF_PROC(ioctrl)
{
	return io_mode_ctrl(argc,argv);
}
int io_set_clr(int argc, char *argv[], int v)
{
	int res;
	if(argc<2)
		goto bad;
	unsigned x,y;
	res = sscanf(argv[1],"p%u.%u",&x,&y);
	if(res!=2) goto bad;
	brd_ioctrl(x,(unsigned)(1<<y),v);
	return 0;
bad:
	hxt_puts("Usage:");
	hxt_printf("\t %s p<x>.<y>\r\n",argv[0]);
	return -1;
}
HXT_DEF_PROC(iomode)
{
	return io_mode_ctrl(argc,argv);
}
HXT_DEF_PROC(ioclr)
{
	return io_set_clr(argc,argv,0);
}
HXT_DEF_PROC(ioset)
{
	return io_set_clr(argc,argv,1);
}

HXT_DEF_PROC(ioval)
{
	int res;
	if(argc<2)
		goto bad;
	unsigned x,y,v;
	res = sscanf(argv[1],"p%u.%u",&x,&y);
	if(res==1){
		v = brd_ioval(x);
		hxt_printf("res=%08X\r\n",v);
	}else if(res==2){
		v = brd_ioval(x);
		v = (v&(1<<y))?1:0;
		hxt_printf("res=%x\r\n",v);
	}else{
		goto bad;
	}
	
	return 0;
bad:
	hxt_puts("Usage:");
	hxt_puts("\t ioval p<x>.[y]");
	return -1;
}
const struct HXT_CMD_T g_cmd_list[] = {
	{"help",	hxt_help,		"œ‘ æ∞Ô÷˙–≈œ¢"},
	{"?",		hxt_help,		"œ‘ æ∞Ô÷˙–≈œ¢"},
	{"clear",	hxt_clear,		"«Â∆¡"},
	{"cr",		hxt_cr,			"«Â∆¡≤¢÷ÿ∆Ù"},
	{"version",	hxt_version,	"œ‘ æ∞Ê±æ"},
	{"link",	hxt_link,		"¡¨Ω”µΩ◊÷∑˚…Ë±∏"},
	{"devcfg",	hxt_devcfg,		"∏¸∏ƒ…Ë±∏≤Œ ˝"},
	{"write",	hxt_write,		"–¥√¸¡Ó"},
//	{"read",	hxt_read,		"∂¡√¸¡Ó"},
	{"ls",		hxt_list,		"¡–≥ˆŒƒº˛¡–±Ì"},
	{"reset",	hxt_reset,		"∏¥Œª"},
	{"ioctrl",	hxt_ioctrl,		"I/O ‰≥ˆøÿ÷∆"},
	{"iomode",	hxt_iomode,		"I/Oƒ£ Ωøÿ÷∆"},
	{"ioval",	hxt_ioval,		"I/O∂¡»° ‰»Î"},
	{"ioclr",	hxt_ioclr,		"I/O ‰≥ˆ0"},
	{"ioset",	hxt_ioset,		"I/O ‰≥ˆ1"},
	
//	{"psam",	hxt_psam,		"∑¢ÀÕPSAMø®√¸¡Ó"},
//	{"cpu",		hxt_psam,		"∑¢ÀÕCPUCARDø®√¸¡Ó"},
//	{"flash",	hxt_flash,		"≤Ÿ◊˜falsh: r/w/e"},
	{"param",	hxt_param,		"∂¡»°…Ë±∏≤Œ ˝"},
};

int g_cmd_count = sizeof(g_cmd_list)/sizeof(g_cmd_list[0]);



