/*
 * hx_utils.h
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#ifndef __HX_UTILS_H__
#define	__HX_UTILS_H__

#define __HX_UTILS__

#include "int.h"

#include "hx_device.h"
#include "hx_term.h"
#include "hxl_serial.h"
#include "hxl_poll.h"
#include "hx_debug.h"
#include "hx_ver_def.h"
#include "hx_clcd.h"
#include "hx_debug.h"
#include "hxd_atc.h"

extern int hx_utils_init(void);
extern uint32_t hx_get_gpclk(void);

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
extern void hx_init_tickcount(int pclk);

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
extern uint16_t HX_LSB_B2W(const void *data);
extern uint16_t HX_MSB_B2W(const void *data);
extern uint32_t HX_MSB_B2DW(const void *data);
extern uint32_t HX_LSB_B2DW(const void *data);
extern uint64_t HX_MSB_B2QW(const void *data);
extern uint64_t HX_LSB_B2QW(const void *data);
extern void HX_LSB_W2B(uint16_t v,void *p);
extern void HX_MSB_W2B(uint16_t v,void *p);
extern void HX_MSB_DW2B(uint32_t v,void *p);
extern void HX_LSB_DW2B(uint32_t v,void *p);
extern void HX_MSB_QW2B(uint64_t v,void *p);
extern void HX_LSB_QW2B(uint64_t v,void *p);

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
extern int hx_str2value(const char *s, const char *value_type, void *vres);
extern int hx_value2str(const void* value,const char *format,
                 char *sres);
	
extern unsigned char make_bcc(unsigned char init, const void *data, int len);
extern unsigned char make_bcc2(const void *data, int len);
	

void* pk_fill(void *to, int len, int d);
void* pk_add(void *to, int len, const void *from);
void* pk_get(void*from,int len,void* to);

//------------------------------------------------------------------------------
extern int ymd2days(int y,int m,int d);
extern long long ymdhms2sec(int y,int m,int d,int H,int M,int S);
extern int ymdbcd2days(uint8_t *yyyymmdd);
extern long long ymdhmsbcd2sec(uint8_t *yyyymmddHHMMSS);

#endif


