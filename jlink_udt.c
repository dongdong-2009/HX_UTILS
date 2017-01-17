#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rt_sys.h>
#include <rt_misc.h>

/*
#include "D:/HX_UTILS/jlink_udt.h"
int main(void) 
{
	fprintf(stdin,"AAA\n");
	fprintf(stdout,"BBB\n");
	fprintf(stderr,"CCC\n");
	while(1)
	{
		int c = getchar();
		if(c!=EOF)
		{
			printf("getc: %c\n",(char)c);
		}
	}
}

*/

/*
	!!! Importent
	Dont use microlib.
	The macros _MICROLIB is not work!
	And :tt stdfile name cannot be echange
*/
//#ifdef _MICROLIB
//#pragma import(__use_full_stdio)
//#endif
#pragma import(__use_no_semihosting)

/*
surpport debug interface:
	JLINK_DCC
	JLINK_DMI_TERM
*/

//#ifdef __DEBUG__
//#define USE_JLINK_DCC
#define JLINK_DMI_TERM
//#endif

#define CONFIG_BUFSIZ		(128)

//#define STDIN_BUFSIZ		CONFIG_BUFSIZ
#define STDOUT_BUFSIZ		CONFIG_BUFSIZ
#define STDERR_BUFSIZ		CONFIG_BUFSIZ

#ifdef USE_JLINK_DCC
//#define STDIO_READ		jlink_dcc_read
#define STDIO_WRITE		jlink_dcc_write
#define STDIO_GETC		jlink_dcc_getc
#endif

#ifdef JLINK_DMI_TERM
//#define STDIO_READ		jlink_dmit_read
#define STDIO_WRITE		jlink_dmit_write
#define STDIO_GETC		jlink_dmit_getc
#endif

#if !defined(STDIO_WRITE) || !defined(STDIO_GETC)
//#define STDIO_READ		jlink_dcc_read
#define STDIO_WRITE		jlink_dcc_write
#define STDIO_GETC		jlink_dcc_getc
#endif

////=====================================================================

#define STDIN             (0)
#define STDOUT            (1)
#define STDERR            (2)

//static int STDIO_READ(int f, unsigned char *buf,unsigned len);
static int STDIO_WRITE(int f, const unsigned char *buf,unsigned len);
static int STDIO_GETC(int *c);

const char __stdin_name[] =  ":stdin";
const char __stdout_name[] =  ":stdout";
const char __stderr_name[] =  ":stderr";


//static char stdin_buff[STDIN_BUFSIZ];
static char stdout_buff[STDOUT_BUFSIZ];
static char stderr_buff[STDERR_BUFSIZ];

FILE* __UDT_BLUE;

FILEHANDLE _sys_open(const char *name, int openmode)
{
	(void)openmode;
	if(strcmp(name,__stdin_name)==0){
		//setvbuf(stdin,stdin_buff,_IOFBF,STDIN_BUFSIZ);
		setvbuf(stdin,0,_IONBF,0);
		return STDIN;
	}else if(strcmp(name,__stdout_name)==0){
		setvbuf(stdout,stdout_buff,_IOFBF,STDOUT_BUFSIZ);
		return STDOUT;
	}else if(strcmp(name,__stderr_name)==0){
		setvbuf(stderr,stderr_buff,_IOFBF,STDERR_BUFSIZ);
		__UDT_BLUE = fopen(":stdin","a+");
		return STDERR;
	}else{
		return -1;
	}
}
int _sys_close(FILEHANDLE fh)
{
	(void)fh;
	return 0;
}
int _sys_write(FILEHANDLE fh, const unsigned char *buf,
               unsigned len, int mode)
{
	int res;
	(void)mode;
	if(fh<0||fh>2)
		return -1;
	res = STDIO_WRITE((int)fh,buf,len);
	if(res == len)
		return 0;
	//return -1;
	return 0;
}
int _sys_read(FILEHANDLE fh, unsigned char *buf,
              unsigned len, int mode)
{
	(void)mode;
//	if(fh<0||fh>2)
//		return -1;
//	return STDIO_READ((int)fh,buf,len);
	return 0;
}
void _ttywrch(int ch)
{
  fputc(ch, stdout);
  fflush(stdout);
}
int _sys_istty(FILEHANDLE fh)
{
	return 1;
	//return 0; /* buffered output */
}
int _sys_seek(FILEHANDLE fh, long pos)
{
	(void)fh;
	(void)pos;
	return -1; /* not supported */
}
long _sys_flen(FILEHANDLE fh)
{
	(void)fh;
	return -1; /* not supported */
}
void _sys_exit(int rc) 
{
	(void)rc;
}


int fgetc_noblock(FILE *f) {
	int res;
	int c;
	(void)f;
	res = STDIO_GETC(&c);
	if(res)
		return 0xffu & c;
    return EOF;
}
int fgetc_block(FILE *f) {
	int res;
	do{
		res = fgetc_noblock(f);
	}while(res == EOF);
	return res;
}
int fgetc(FILE *f) {
	return fgetc_noblock(f);
}


#ifndef uint
#define byte	unsigned char
#define uchar	unsigned char
#define uint	unsigned int
#define ushort	unsigned short
#endif


//=================================================================
#ifdef USE_JLINK_DCC

#ifdef __ICCARM__
	#define __ARM_FUNC  __arm    
	#define ATTRIBUTE_NAKED_NO_INLINE

	#if (defined (__ARM7A__) && (__CORE__ == __ARM7A__)) || (defined (__ARM7R__) && (__CORE__ == __ARM7R__))
		#define __ARMv7A__ 1
		#undef __ARMv5__
	#else
		#define __ARMv5__ 1
		#undef __ARMv7A__
	#endif
#endif


#ifdef __GNUC__
	#define __ARM_FUNC
	#define __INTERWORK
	#define ATTRIBUTE_NAKED_NO_INLINE __attribute__ ((naked, noinline)) /* parasoft-suppress MISRA2004-19_4 "Compiler specific keyword." */

	#if (defined (__ARM_ARCH_7A__) && (__ARM_ARCH_7A__ == 1)) || (defined (__ARM_ARCH_7R__) && (__ARM_ARCH_7R__ == 1))
		#define __ARMv7A__ 1
		#undef __ARMv5__
	#else
		#define __ARMv5__ 1
		#undef __ARMv7A__
	#endif

	#pragma GCC diagnostic ignored "-Wreturn-type"      // Avoid warning for missing return value with GCC
	#pragma GCC diagnostic ignored "-Wunused-parameter" // Avoid warning for unused parameter with GCC
#endif

#ifdef __CC_ARM
	#define __ARM_FUNC
	#define __INTERWORK
	#define ATTRIBUTE_NAKED_NO_INLINE __attribute__ ((noinline))        /* parasoft-suppress MISRA2004-19_4 "Compiler specific keyword." */
	#pragma arm

	#if (defined (__TARGET_ARCH_ARM) && (__TARGET_ARCH_ARM == 7))
		#define __ARMv7A__ 1
		#undef __ARMv5__
	#else
		#define __ARMv5__ 1
		#undef __ARMv7A__
	#endif
#endif

//
// The bit positions for DCC RX and TX ready are different for ARMv5 and ARMv7,
// so we have to distinguish here.
//
#if (defined (__ARMv7A__) && (__ARMv7A__ == 1))
	#define RX_FULL_FLAG_SHIFT 30
	#define TX_FULL_FLAG_SHIFT 29
#else
	#define RX_FULL_FLAG_SHIFT 0
	#define TX_FULL_FLAG_SHIFT 1
#endif
/*********************************************************************
*
*       _ReadDCCStat()
*
*  IAR macro:     unsigned long __MRC(coproc, opcode_1, CRn, CRm, opcode_2);
*  Inst:          MRC<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move from coproc to ARM reg
*  ARMv4 / ARMv5: MRC p14,0,<Rt>,c0,c0,0
*  ARMv7-AR:      MRC p14,0,<Rt>,c0,c1,0 DBGDSCR
*/
static __INTERWORK __ARM_FUNC uint _ReadDCCStat(void) ATTRIBUTE_NAKED_NO_INLINE;
static __INTERWORK __ARM_FUNC uint _ReadDCCStat(void) {
#if (defined (__ARMv7A__) && (__ARMv7A__ == 1))
#ifdef __ICCARM__
    return __MRC(14u, 0u, 0u, 1u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mrc p14, 0, r0, c0, c1, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    register unsigned int r;
    r = 0;  // Set initial value to avoid compiler warning
    __asm
    {
        mrc p14, 0, r, c0, c1, 0
    }
    return r;
#endif
#else
#ifdef __ICCARM__
    return __MRC(14u, 0u, 0u, 0u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mrc p14, 0, r0, c0, c0, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    register unsigned int r;
    r = 0;  // Set initial value to avoid compiler warning
    __asm
    {
        mrc p14, 0, r, c0, c0, 0
    }
    return r;
#endif
#endif
}

/*********************************************************************
*
*       _ReadDCC()
*
*  IAR macro:     unsigned long __MRC(coproc, opcode_1, CRn, CRm, opcode_2);
*  Inst:          MRC<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move from coproc to ARM reg
*  ARMv4 / ARMv5: MRC p14,0,<Rt>,c1,c0,0
*  ARMv7-AR:      MRC p14,0,<Rt>,c0,c5,0 DTRRX
*/
static __INTERWORK __ARM_FUNC uint _ReadDCC(void) ATTRIBUTE_NAKED_NO_INLINE;
static __INTERWORK __ARM_FUNC uint _ReadDCC(void) {
#if (defined (__ARMv7A__) && (__ARMv7A__ == 1))
#ifdef __ICCARM__
    return __MRC(14u, 0u, 0u, 5u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mrc p14, 0, r0, c0, c5, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    register unsigned int r;
    r = 0;  // Set initial value to avoid compiler warning
    __asm
    {
        mrc p14, 0, r, c0, c5, 0
    }
    return r;
#endif
#else
#ifdef __ICCARM__
    return __MRC(14u, 0u, 1u, 0u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mrc p14, 0, r0, c1, c0, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    register unsigned int r;
    r = 0;  // Set initial value to avoid compiler warning
    __asm
    {
        mrc p14, 0, r, c1, c0, 0
    }
    return r;
#endif
#endif
}

/*********************************************************************
*
*       _WriteDCC()
*
*  IAR macro:     void __MCR(coproc, opcode_1, Data, CRn, CRm, opcode_2);
*  Inst:          MCR<c> <coproc>,<opc1>,<Rt>,<CRn>,<CRm>{,<opc2>}  Move to coproc
*  ARMv4 / ARMv5: MCR P14,0,<Rt>,C1,C0,0
*  ARMv7-AR:      MCR p14,0,<Rt>,c0,c5,0 DTRTX
*/
static __INTERWORK __ARM_FUNC void _WriteDCC(uint Data) ATTRIBUTE_NAKED_NO_INLINE;
static __INTERWORK __ARM_FUNC void _WriteDCC(uint Data) {
#if (defined (__ARMv7A__) && (__ARMv7A__ == 1))
#ifdef __ICCARM__
    __MCR(14u, 0u, Data, 0u, 5u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mcr p14, 0, r0, c0, c5, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    __asm
    {
        mcr p14, 0, Data, c0, c5, 0
    }
#endif
#else
#ifdef __ICCARM__
    __MCR(14u, 0u, Data, 1u, 0u, 0u);
#elif defined (__GNUC__)
    __asm(".arm\n\
           mcr p14, 0, r0, c1, c0, 0\n\
           bx  lr\n");
#elif defined (__CC_ARM)
    __asm
    {
        mcr p14, 0, Data, c1, c0, 0
    }
#endif
#endif
}

//=============================================================================
static void dcc_write_u32(uint d) {
    uint busy;
	do {
        busy = _ReadDCCStat();
    } while((busy & 2u));
    _WriteDCC(d);
}
//static int dcc_read_u32(uint *d)
//{
// uint Data;

//  if (((_ReadDCCStat() >> RX_FULL_FLAG_SHIFT)  & 1u) == 0u) {       // Data or command received ?
//    return 0;      // Nothing received
//  }
//  Data = _ReadDCC();
//  if((Data&0xFF000000u) == 0x55000000u){
//	*d = Data & 0xFFu;
//	return 1;
//  }
//  return 0;
//}
static uchar make_sum(uchar init,const uchar *buf,uint len)
{
	int i;
	uchar res = init;
	for(i=0;i<len;i++){
		res += buf[i];
	}
	return res;
}
static uint array2uint(const uchar *_data, uint len)
{
	const uchar *d = _data;
    uint res = 0L;
	if(len==1){
		res = ((uint)d[0])<<24;
	}else if(len==2){
		res = ((uint)d[0])<<24 | ((uint)d[1])<<16;
	}else if(len==3){
		res = ((uint)d[0])<<24 | ((uint)d[1])<<16 | ((uint)d[2])<<8;
	}else {//if(len==4){
		res = ((uint)d[0])<<24 | ((uint)d[1])<<16 | ((uint)d[2])<<8 | ((uint)d[3]);
	}
    return res;
}
static uchar jlink_dcc_send_frame(uchar color, const uchar *buf, uchar len)
{
	/*
	proctol:
	StartFrame              DataFrame        EndFrame
	---------------------   -------------    ------------------  
	sof cmd1  cmd2   dlen   [data...] ...    eof sum1 rfs1 rfs2
	FE  C1/C2 color  <256                    F0  
	*/
	if(buf==NULL || len>128)
		return 0;
	uint dlen = len & 0x0FFu;
	uint StartFrame = 0xFEC10000u | (color<<8) | dlen;
	uint sum = make_sum(dlen,buf,len);
	uint EndFrame = 0xF0000000u | (sum<<16);
	dcc_write_u32(StartFrame);
	uint l = len>>2<<2;
	for(int i=0;i<l;i+=4){
		uint d = array2uint(&buf[i],4);
		dcc_write_u32(d);
	}
	if(len>l){
		dcc_write_u32(array2uint(&buf[l],len-l));
	}
	dcc_write_u32(EndFrame);
	return len;
}
static int jlink_dcc_putc(int ch, int d) {
    uint packet;
    uint busy;
	//54 <reverse> <color8> <d8>
    packet = 0x54000000 | ((ch&0xFFu) << 8) | (d&0xFFu);
    do {
        busy = _ReadDCCStat();
    } while((busy & 2u));
    _WriteDCC(packet);
	return d;
}
static int jlink_dcc_write(int f, const unsigned char *buf, unsigned len)
{
	uint ch = 'r';
	if(f==0) ch='g';	//green
	if(f==1) ch='w';	//white
	if(f==2) ch='r';	//red
	if(len==1){
		int c = *buf;
		if(c == '\n'){
		jlink_dcc_putc(ch,'\r');
			return jlink_dcc_putc(ch,'\n');
		}else{
			return jlink_dcc_putc(ch,c);
		}
	}
	uint l = len>>7<<7;
	for(uint i=0;i<l;i+=128){
		jlink_dcc_send_frame(ch,&buf[i],128);
	}
	if(len>l){
		jlink_dcc_send_frame(ch,&buf[l],len-l);
	}
	return len;
}
static int jlink_dcc_getc(int *d)
{
  uint Data;

  if (((_ReadDCCStat() >> RX_FULL_FLAG_SHIFT)  & 1u) == 0u) {       // Data or command received ?
    return 0;      // Nothing received
  }
  Data = _ReadDCC();
  if((Data&0xFF000000u) == 0x55000000u){
	*d = Data & 0xFFu;
	return 1;
  }
  return 0;
}
#endif
//=======================================================================
#ifdef JLINK_DMI_TERM

//must be in ram
#define TRAD_CTRLBLK_ADDR			(0x10001000)		

//terminal use ram access directlly  
typedef __packed struct __TRAD
{
	unsigned int flag;
	unsigned int data;
} TRAD_T;

static TRAD_T trad_tx __attribute__((at(TRAD_CTRLBLK_ADDR))) = {0};
static TRAD_T trad_rx __attribute__((at(TRAD_CTRLBLK_ADDR+sizeof(TRAD_T)))) = {0};


static int jlink_dmit_putc(int ch,int c)
{
	int res;
	do{
		res = trad_tx.flag;
	}while(res);	
	trad_tx.data = c | 0x54000000|(ch<<8);
	trad_tx.flag = 1;
	return 0;
}
static int jlink_dmit_getc(int *c)
{
	int res;
	res = trad_rx.flag;
	if(res){
		res = trad_rx.data;
		if((res&0xFF000000u) == 0x55000000u ){
			trad_rx.data = 0x00000000;
			*c = res & 0x0FFu;
			return 1;
		}
		trad_rx.flag = 0x00000000;
	}
	return 0;
}
static int jlink_dmit_write(int f, const unsigned char *buf, unsigned len)
{
	int i;
	uint ch = 'r';
	if(f==0) ch='g';	//green
	if(f==1) ch='w';	//white
	if(f==2) ch='r';	//red
	for(i=0;i<len;i++)
		jlink_dmit_putc(ch,buf[i]);
	return len;
}

#endif




