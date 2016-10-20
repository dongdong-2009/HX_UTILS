/*
 * hx_term.h
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#ifndef _HX_TERM_H_
#define _HX_TERM_H_


//===========================================================
// configurations
//VT100 controls
/* http://www.termsys.demon.co.uk/vtansi.htm */
#define VT100_CLR		"[2J"
//#define VT100_COLOR		"[BB;FFm"		//BB:40ºÚ41Éîºì42ÂÌÉ43»Æ

//home [1~
//end  [4~
//ins  [4~
//ctrl-c \x03
//ctrl-z \x1a
//#define HXT_CTRL_ESC			("")
//#define HXT_CTRL_U				("[A")
//#define HXT_CTRL_D				("[B")
//#define HXT_CTRL_R				("[C")
//#define HXT_CTRL_L				("[D")
//#define HXT_CTRL_PAGE_UP		("\x1B\x35\x7E")
//#define HXT_CTRL_PAGE_DN		("\x1B\x36\x7E")
#define HXT_PROMPT					(">")
/*
 * commad line have chars sum
 */
#define MAX_CHARS_LINE				(512)

/*
 * cmd line buffer size
 */
#define SIZE_OF_LINE_BUFFER			(512)

/*
 * allow max of cmd arg string count
 */
#define MAX_ARGS_COUNT				(10)

//==========================================================
// defines
/*
 * define cmd function macs
 */
#define HXT_DEF_PROC(_cmd)	\
	int hxt_##_cmd (int argc, char **argv)

/*
 * cmd list item. adding macs
 */
#define HXT_DEF_CMD(_cmd,_info)				\
{#_cmd, _info, hxt_##_cmd, },

/*
 * cmd function define
 */
typedef int HXT_CMD_FUNC_T(int argc, char **argv);

//==========================================================
// apis
/*
 * block into terminal mode
 */
extern int hxt_term_run(int argc, char *argv[]);

extern int hxt_term_init(void);
/*
 * noblock,polling once from input stream
 */
extern int hxt_term_polling(void);

/*
 * execute cmd directly.
 */
extern int hxt_exec_cmd(const char *cmd);
extern int hxt_printf(const char *fmt, ...);
extern int hxt_getc_noblock(int *c);
/*
	only two case can return :
	<0:now is no char
	>=0:has a token
	-'' : is esc got
*/
extern int hxt_gets_echo_noblock(char **s);
extern int hxt_putc(int c);
extern int hxt_put(const char *s);
extern int hxt_puts(const char *s);
/*
	use for achive cmd func, get arguement.
*/
extern int hx_get_arg(int argc,char *argv[],const char *arg, char **res);
extern int hx_get_arg_bool(int argc,char *argv[],const char *arg,int* res);
extern int hx_get_arg_int(int argc,char *argv[],const char *arg, int *res);

#define HX_GET_ARG_INT(a)	hx_get_arg_int(argc,argv,#a,&a)
#define HX_GET_ARG_BOOL(a)	hx_get_arg_bool(argc,argv,#a,&a)
#define HX_GET_ARG(a)		hx_get_arg(argc,argv,#a,&a)
#define HXT_PARAM_ERROR(a)	\
	hxt_printf("param error: " #a "%s \r\n",uart_param);
/*
	use for achive cmd list define .
*/
struct HXT_CMD_T {
	const char *cmd_str;
	HXT_CMD_FUNC_T *p_main;
	const char *cmd_info;
};
extern HXT_DEF_PROC(help);
extern HXT_DEF_PROC(version);
//========================================================
// port
extern void hxt_uart_enable_interrupt(int b);
extern int __port_input_get_char(int *c);
extern int __port_output_put_char(int c);
extern void hxt_uart_rx_byte(unsigned int b);

#define CTRL_BS			(0x7F)
#define CTRL2CHAR(c)	(c+'@')	
#define IS_CTRL_CHAR(c)	(c>0 && c<=0x1F && c!='\r' \
	&& c!='\n' && c!='\t' && c!='\b')
/*
	because of keys tab(\r) enter(\r,\n) bs(\b) occupy ctrl-char
	so belows are cannot use for self defined ctrl-key
	ctrl+key		code	char	key
	Ctrl+H			08		\b		BS
	Ctrl+M			0D		\n		Enter
	Ctrl+I			09		\t		Tab
	Ctrl+J			0A		\r		Return
*/

//========================================================
//cmds
typedef struct _DEV_PARAM_T {
	const char *name;
	const char *data_type;	
	// <type>,<count>[,<spilt-chars>]
	// "bin","bcd", "asc", "i32","i64","f32","f64"
	// bin,2	AA55		"\xAA\x55"
	// bcd,2,- "11-22"		"\x11\x22"
	// asc,0	"houxd"		"houxd"
	// asc,3	"abc"		"abc"
	// i32		100			100
	// ...
	void *data;
} DEV_PARAM_T;

#endif /* SRC_HX_TERM_H_ */
