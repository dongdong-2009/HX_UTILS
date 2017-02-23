#include "hx_utils.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#ifdef __HX_ENABLE_DEBUG__

int __dummy_op(int p,...)
{
	return p;
}

int hx_vprintf(HX_DEV *d,const char *fmt,va_list va)
{	
	if(!g_enable_debug_output)
		return 0;
	return hxl_vprintf(d,fmt, va);
}

int hx_printf(const char *fmt,...)
{

	int res;
	va_list  va;
    va_start(va, fmt);
    res = hx_vprintf(hxout,fmt, va);
    va_end(va);
	return res;
}
int hx_dprintf(HX_DEV *d,const char *fmt,...)
{
	int res;
	if(!g_enable_debug_output)
		return 0;
	va_list  va;
    va_start(va, fmt);
    res = hx_vprintf(d,fmt, va);
    va_end(va);
	return res;
}
#define SHOW_TAG(...)	//hxl_printf(__VA_ARGS__)
void hx_dbg(char ch,const char *fmt,...)
{
	if(!ch || !strchr(DISABLE_DEBUG_TAG,ch)){
		va_list  va;
		va_start(va, fmt);
		SHOW_TAG(hxout,"[%u]",(unsigned int)ch);
		hx_vprintf(hxout,fmt,va);
		va_end(va);
	}
}
void hx_dbgi(char ch,const char *fmt,...)
{
	if(!ch || !strchr(DISABLE_DEBUG_TAG,ch)){
		va_list  va;
		va_start(va, fmt);
		SHOW_TAG(hxout,"[%u]",(unsigned int)ch);
		hx_vprintf(hxin,fmt,va);
		va_end(va);
	}
}
void hx_dbge(char ch,const char *fmt,...)
{
	if(!ch || !strchr(DISABLE_DEBUG_TAG,ch)){
		va_list  va;
		va_start(va, fmt);
		SHOW_TAG(hxout,"[%u]",(unsigned int)ch);
		hx_vprintf(hxerr,fmt,va);
		va_end(va);
	}
}
int hx_debug_enable(int en)
{
	int res = g_enable_debug_output;
	g_enable_debug_output = en;
	return res;
}
#endif
