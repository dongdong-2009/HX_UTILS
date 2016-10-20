/*
 * hx_debug.h
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#ifndef __HX_DEBUG_H__
#define __HX_DEBUG_H__


#ifdef __DEBUG__
#define __HX_ENABLE_DEBUG__
#endif

#ifdef __HX_ENABLE_DEBUG__	

/*
	Debug serial data.
*/
//#define HX_DEBUG_SERIAL_INPUT			(0)
//#define HX_DEBUG_SERIAL_OUTPUT		(0)

/*
	When the debug is enabled, 
	this control the output info that is show or not. on default state
*/
#define	HX_OUTPUT_STATE_DEFAULT			(1)

extern int HX_DBG_ENABLE(int en);
extern int  HX_DBG_PRINT(const char *format, ...);

#define  HX_DBG_PRINTLN(...)			\
	do{									\
		HX_DBG_PRINT(__VA_ARGS__);		\
		HX_DBG_PRINT("\r\n");			\
	}while(0)
#define  HX_DBG_ERROR(errCode)     								\
	hx_uart_printf(UART_DEBUG_PORT,								\
	"ERROR: In Function \"%s\" at Line (%u) - [" #errCode " = %d] \r\n", 		\
	__FUNCTION__, __LINE__, errCode)
#define HX_DBG_DUMP_HEX(buf,size)\
do{	\
	const char *p = (const char*)buf;\
	for(int i=0;i<size;i++){\
		HX_DBG_PRINT("%02X",(int)p[i]);\
	}\
}while(0)	

#define HX_ABORT(s)												\
do{																\
	hx_uart_printf(UART_DEBUG_PORT,								\
	"ABORT: In Function \"%s\" at Line (%u) - [%s] \r\n", 						\
	__FUNCTION__, __LINE__, s);									\
	while(1);													\
}while(0)
	
#define HX_ASSERT(n)			do{	if(!(n)){ABORT(#n);} }while(0)
#else

#define  HX_DBG_ENABLE(...)	
#define  HX_DBG_PRINT(...)    
#define  HX_DBG_PRINTLN(...)	
#define  HX_DBG_ERROR(...)   
#define  HX_DBG_DUMP_HEX(...)	
#define  HX_ABORT(...)		
#define  HX_ASSERT(...)
	
#endif


//==========================================================
//Debug info for packet add/get
#define PK_SHOW_TAB_SIZE			(8)
#define PK_SHOW_NAME_LEN			(PK_SHOW_TAB_SIZE*4)
#define PK_DEBUG_FILL(p,n,x)	\
{\
	const char *name = #x;\
	int len = strlen(name);\
	len = PK_SHOW_NAME_LEN - len;\
	HX_DBG_PRINT("\t%s",name);\
	for(int i=0;i<len;i+=PK_SHOW_TAB_SIZE)\
		HX_DBG_PRINT("\t");\
	HX_DBG_PRINT("%d\t ",(int)(n));\
	HX_DBG_PRINT("{%02X}",(int)(x));\
	HX_DBG_PRINT("\r\n");\
}
#define PK_DEBUG_ARRAY(p,n,x)	\
{\
	const char *name = #x;\
	int len = strlen(name);\
	len = PK_SHOW_NAME_LEN - len;\
	HX_DBG_PRINT("\t%s",name);\
	for(int i=0;i<len;i+=PK_SHOW_TAB_SIZE)\
		HX_DBG_PRINT("\t");\
	HX_DBG_PRINT("%d\t ",(int)(n));\
	for(int i=0;i<n;i++){\
		HX_DBG_PRINT("%02X",(int)(((char*)(x))[i]));\
	}\
	HX_DBG_PRINT("\r\n");\
}
#define PK_DEBUG_BYTE(p,x)	\
{\
	const char *name = #x;\
	int len = strlen(name);\
	len = PK_SHOW_NAME_LEN - len;\
	HX_DBG_PRINT("\t%s",name);\
	for(int i=0;i<len;i+=PK_SHOW_TAB_SIZE)\
		HX_DBG_PRINT("\t");\
	HX_DBG_PRINT("1\t %02X\r\n",(int)(x));\
}
#define PK_DEBUG_WORD(p,x)	\
{\
	const char *name = #x;\
	int len = strlen(name);\
	len = PK_SHOW_NAME_LEN - len;\
	HX_DBG_PRINT("\t%s",name);\
	for(int i=0;i<len;i+=PK_SHOW_TAB_SIZE)\
		HX_DBG_PRINT("\t");\
	HX_DBG_PRINT("2\t %04X\r\n",(int)(x));\
}
#define PK_DEBUG_DWORD(p,x)	\
{\
	const char *name = #x;\
	int len = strlen(name);\
	len = PK_SHOW_NAME_LEN - len;\
	HX_DBG_PRINT("\t%s",name);\
	for(int i=0;i<len;i+=PK_SHOW_TAB_SIZE)\
		HX_DBG_PRINT("\t");\
	HX_DBG_PRINT("4\t %08X\r\n",(int)(x));\
}

#define PK_ADD_ARRAY(p,n,x)	\
{\
	p = pk_add((p), (n), (x));	\
	PK_DEBUG_ARRAY((p),(n),(x));	\
}	
#define PK_ADD_BYTE(p,x)	\
{\
	*p++ = (x); \
	PK_DEBUG_BYTE((p),(x));	\
}	
#define PK_ADD_WORD(p,x)	\
{\
	HX_MSB_W2B(x,(p)); \
	p += 2; \
	PK_DEBUG_WORD((p),(x));	\
}	
#define PK_ADD_DWORD(p,x)	\
{\
	HX_MSB_DW2B((x),(p)); \
	p += 4; \
	PK_DEBUG_DWORD((p),(x));	\
}	





#define PK_DEBUG_BYTE_ERROR(p,x)\
{\
	const char *name = #x;\
	HX_DBG_PRINT("\t%s !!! want %02X, get %02X.\r\n",name,(int)(x),(int)*(p));\
}
#define PK_DEBUG_WORD_ERROR(v,x)\
{\
	const char *name = #x;\
	HX_DBG_PRINT("\t%s !!! want %04X, get %04X.\r\n",name,(int)(x),(int)(v));\
}
#define PK_DEBUG_DWORD_ERROR(v,x)\
{\
	const char *name = #x;\
	HX_DBG_PRINT("\t%s !!! want %08X, get %08X.\r\n",name,(int)(x),(int)(v));\
}
#define PK_DEBUG_ARRAY_ERROR(p,n,x)\
{\
	const char *name = #x;\
	HX_DBG_PRINT("\t%s !!! want ",name);\
	for(int i=0;i<(n);i++)\
		HX_DBG_PRINT("%02X", (int)(((char*)(x))[i]));\
	HX_DBG_PRINT(", get ");\
	for(int i=0;i<(n);i++)\
		HX_DBG_PRINT("%02X", (int)(((char*)(p))[i]));\
	HX_DBG_PRINT(".\r\n");\
}

#define PK_WANT_BYTE(p,x,callback)	\
{\
	if(*(p) != (x)){\
		PK_DEBUG_BYTE_ERROR((p),(x));\
		callback;\
	}else{\
		(p)++;\
		PK_DEBUG_BYTE((p),(x));\
	}\
}
#define PK_WANT_WORD(p,x,callback)	\
{\
	int v = HX_MSB_B2W(p);\
	if(v != (x)){\
		PK_DEBUG_WORD_ERROR((v),(x));\
		callback;\
	}else{\
		p+=2;\
		PK_DEBUG_WORD((v),(x));\
	}\
}

#define PK_WANT_DWORD(p,x,callback)	\
{\
	int v = HX_MSB_B2DW(p);\
	if(v != (x)){\
		PK_DEBUG_DWORD_ERROR(v,(x));\
		callback;\
	}else{\
		p+=4;\
		PK_DEBUG_DWORD(v,(x));\
	}\
}
#define PK_WANT_ARRAY(p,n,x,callback)	\
{\
	if(memcmp(p,(x),(n))){\
		PK_DEBUG_ARRAY_ERROR((p),(n),(x));\
		callback;\
	}else{\
		p+=n;\
		PK_DEBUG_ARRAY((p),(n),(x));\
	}\
}
#define PK_GET_BYTE(p,x)	\
{\
	(x) = *p++;\
	PK_DEBUG_BYTE(p,(x));\
}
#define PK_GET_WORD(p,x)	\
{\
	(x) = HX_MSB_B2W(p);\
	p+=2;\
	PK_DEBUG_WORD((p),(x));\
}
#define PK_GET_DWORD(p,x)	\
{\
	(x) = HX_MSB_B2DW(p);\
	(p)+=4;\
	PK_DEBUG_DWORD((p),(x));\
}
#define PK_COPY_ARRAY(p,n,x)	\
{\
	memcpy((x),(p),(n));\
	(p)+=(n);\
	PK_DEBUG_ARRAY((p),(n),(x));\
}
#define PK_REF_ARRAY(p,n,x)	\
{\
	(x)=p;\
	(p)+=(n);\
	PK_DEBUG_ARRAY((p),(n),(x));\
}
#define PK_GET_ARRAY(p,n,x)		PK_COPY_ARRAY(p,n,x)

#endif

