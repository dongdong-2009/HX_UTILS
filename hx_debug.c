#include "hx_utils.h"
#include "hx_board.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#ifdef __HX_ENABLE_DEBUG__
extern int g_enable_debug_output;
int  HX_DBG_PRINT(const char *format, ...)
{
	int port_nr = UART_DEBUG_PORT;
	if(g_enable_debug_output==0) 
		return 0;

	
	int res;
	//use data or stack!
    //static  	
	char  buffer[VSPRINTF_BUFF_SIZE];
     
	va_list  vArgs;
    va_start(vArgs, format);
    res = vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    hx_uart_put(port_nr, buffer);
	hx_uart_send(port_nr,NULL,0);
	return res;
}
#endif


