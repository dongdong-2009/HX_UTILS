/*
 * hx_utils.h
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#ifndef __UTILS_H__
#define	__UTILS_H__

#include "hx_def.h"

#include "int.h"
#include "hx_target.h"

#include "hx_term.h"
#include "hx_serial.h"
#include "hx_atcmd.h"
#include "hx_debug.h"

#include "hx_dep_libs.h"
#include "hx_ver_def.h"

#define hx_do_timeout(tm_in_ms)						\
	for(int __t__ = hx_get_tick_count() + tm_in_ms;	\
		hx_get_tick_count()<__t__;)

#define hx_do_timeout_r(__t__,tm_in_ms)						\
	for(int __t__ = hx_get_tick_count() + tm_in_ms;	\
		hx_get_tick_count()<__t__;)

#define hx_trys(id,times)							\
	for(int id=times;id>0;id--)
		
#define foreach(array,count,pointer) \
	for((pointer)=(array);(pointer)-(array)<(count);(pointer)++)

extern uint32_t hx_get_tick_count(void);
extern void hx_tick_count_init(void);

extern int hx_delay_ms(int ms);
extern int hx_check_timeout3(int *tlist, int tid, uint32_t timeout);
extern int hx_check_timeout2(int *last,uint32_t timeout);
extern int hx_check_timeout(int last,uint32_t timeout);
extern void check_key_to_menu(void);

/*
	'\x19'	 <==>  19u
*/
extern int bcd2int(unsigned char bcd);
extern unsigned char int2bcd(int d);
/*
	"\xAA\x55"  <==>  "AA55"
*/	
extern char* hx_hexcode2bin(const void *hexcode, int len, void *bin);
extern char* hx_dumphex(const void* bin,int bin_len,void *asc);
extern char* hx_dumphex2str(const void* bin,int bin_len,void *asc);

extern int hx_str2value(const char *s, const char *value_type, void *vres);
extern int hx_value2str(const void* value,const char *format,
                 char *sres);
	
extern char *hx_strtrim(char *s);
extern char *hx_strtrim2(char *p, const char *trim_list);
#define strtrim 	hx_strtrim2
	
//==========================================================
// uint8_t ARRAY <==>  uint16_t(2Bytes) uint32_t(4Bytes) QWORD(8Bytes)
// MSB = Big Endian, LSB = Littie Endian
extern uint16_t HX_LSB_B2W(const uint8_t *data);
extern uint16_t HX_MSB_B2W(const uint8_t *data);
extern uint32_t HX_MSB_B2DW(const uint8_t *data);
extern uint32_t HX_LSB_B2DW(const uint8_t *data);
extern uint64_t HX_MSB_B2QW(const uint8_t *data);
extern uint64_t HX_LSB_B2QW(const uint8_t *data);
extern void HX_LSB_W2B(uint16_t v,uint8_t *p);
extern void HX_MSB_W2B(uint16_t v,uint8_t *p);
extern void HX_MSB_DW2B(uint32_t v,uint8_t *p);
extern void HX_LSB_DW2B(uint32_t v,uint8_t *p);
extern void HX_MSB_QW2B(uint64_t v,uint8_t *p);
extern void HX_LSB_QW2B(uint64_t v,uint8_t *p);

//==========================================================
// convert str <=> value with fmt
/*
	<fmt>,count,split-chars
	"bcd n ."		"\x20\x16"		<=>	"2016"		this not surpport 
												count ,split-chars .
	"bin"											like before
	"asc 5"			'houxd123'		<=>	"houxd"		
	"%i 4 ."		[127][0][0][1]	<=> "127.0.0.1"
	"%d"			100	(DWORD)			<=>	"100"
	"%hd/%hu"		WORD
	"%hhd/%hhu"		BYTE
	"%lld/%llu"		QWORD
	"msb 2/4/8"
	"lsb 2/4/8"
*/
extern int hx_value2str(
	const void *value,
	const char *format,
	char *sres);
extern int hx_str2value(
	char *str, 
	const char *format, 
	void *value);
	
extern byte make_bcc(byte init, const void *data, int len);
extern byte make_bcc2(const void *data, int len);
	
	
//==========================================================
//Debug info for packet add/get
#define PK_SHOW_TAB_SIZE			(8)
#define PK_SHOW_NAME_LEN			(PK_SHOW_TAB_SIZE*6)
#define  HX_DBG_PRINT(...)       fprintf((FILE*)'g',__VA_ARGS__)
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
#define PK_FILL(p,n,x)	\
{\
	p = pk_fill((p), (n), (x));	\
	PK_DEBUG_FILL((p),(n),(x));	\
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
void* pk_fill(void *to, int len, int d);
void* pk_add(void *to, int len, const void *from);
void* pk_get(void*from,int len,void* to);

#include "hx_undef.h"
	
#endif


