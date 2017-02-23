/*
* hx_term_cmds.c
*
*  Created on: 2016��7��25��
*      Author: houxd
*/

#include "hx_utils.h"
#include "stdio.h"
#include "string.h"
#include "hxd_param.h"

static int _dev_open(char *dn, HX_DEV *d)
{
	int res;
	char *e;
    const char *dev = strtok_r(dn,":",&e);
    const char *param = strtok_r(NULL,":",&e);
    res = hx_open(dev,param,d);
	if(res){
		hxt_fprintf(hxerr,"open error,dev:%s,param:%s,res:%d\n",
			dev,param,res);
	}
	return res;
}
static int _dev_open_with_param(char *dev, char *pa, HX_DEV *d)
{
	int res;
    res = hx_open(dev,pa,d);
	if(res){
		hxt_fprintf(hxerr,"open error,dev:%s,param:%s,res:%d\n",
			dev,pa,res);
	}
	return res;
}
//static int _dev_open_no_param(char *dev, HX_DEV *d)
//{
//	int res;
//    res = hx_open(dev,"",d);
//	if(res){
//		hxt_fprintf(hxerr,"open error,dev:%s,res:%d\n",
//			dev,res);
//	}
//	return res;
//}

static void next_value(char **s, char *array)
{
    char *ss = *s;
    char *f = strtok(array,",");
    if(ss) {
        do {
            if(strcmp(ss,f)==0) {
                *s = strtok(NULL,",");
                return;
            }
            f = strtok(NULL,",");
        } while(f);
        *s = NULL;
        return;
    } else {
        *s = f;
    }
}
HXT_DEF_PROC(param)
{
    int res;
    HX_DEV dev;
	const PARAM_DEV_T *pdev;
    const PARAM_ITEM_T *g_param_tbl;
    int g_param_count;
    const PARAM_ITEM_T *pm;
	if(argc<2)
		goto bad_arg;
	res = _dev_open(argv[1],&dev);
    if(res)
		return res;
    pdev = (const PARAM_DEV_T *)dev.pdev;
    g_param_tbl = pdev->tbl;
    g_param_count = pdev->count;
    foreach(g_param_tbl,g_param_count,pm) {
        char *arg;
        hx_get_arg(argc-2,argv+2,pm->name,&arg);
        if(argc==2) {
            //show
            char buf[MAX_CHARS_LINE];
            hx_value2str(pm->data,pm->data_type,buf);
            hxt_printf("%s=%s\r\n",pm->name,buf);
        } else {
            if(arg) {
                //change
                res = hx_str2value(arg,pm->data_type,pm->data);
                if(res) {
                    hxt_printf("Change Param Error: %s,type:%s\r\n",
                               pm->name,pm->data_type);
                    goto bad_arg;
                }
            }

        }
    }
    if(argc>1) {
        res =hx_close(&dev);
        if(res) {
            hxt_fprintf(&hx_stderr,"close param device error %d\r\n",res);
            return -1;
        }
    }
    return 0;
bad_arg:
    hxt_puts("Usage:");
    hxt_puts("\t param	<dev>  [p=v]");
    return 0;
}
/*
	Normal Read for char devices
*/
HXT_DEF_PROC(rc)
{
	int c;
    int res;
    HX_DEV d;
//	char *e;
	unsigned timeout = 0;
    if(argc<2)
        goto bad_arg;
	if(argc>=3){
		sscanf(argv[2],"%u",&timeout);
	}
	res = _dev_open(argv[1],&d);
    if(res)
		return res;
	char buf[512];
	if(timeout==0){
		res = hx_read(&d,buf,512);
		if(res>0){
			hxt_fprintf(hxin, "Read %u Bytes ",res);
			for(int i=0;i<res;i++){
				hxt_fprintf(hxin, "%02hhX", buf[i]);
			}
		}else{
			hxt_fprintf(hxerr, "Read Nothing");
		}
	}else{
		hx_do_timeout(timeout){
			res = hxl_getc_noblock(&d,&c);
			if(res==0){
				hxt_fprintf(hxin, "%02X",c);
			}
		}while(c!='\x03' && c!='\x04');
	}
	hxt_puts("");
    return 0;
bad_arg:
    hxt_puts("Usage:");
    hxt_puts("\t rc   <char_device>[:<params>]  [timeout]  Read char device");
    return -1;
}
/*
	Normal write for char devices
*/
HXT_DEF_PROC(wc)
{
    int res;
    int l;
	HX_DEV d;
    char *dat;
//	char *e;
    if(argc<2)
        goto bad_arg;
    dat = argv[2];
	res = _dev_open(argv[1],&d);
    if(res)
		return res;
    char buff[512];
    l = strlen(dat);
	if(l<=0)
		goto bad_arg;
	if(l>=2){
		hx_hexcode2bin(dat,l,buff);
		l /=2;
	}else{
		buff[0] = dat[0]-'0';
		l=1;
	}
	char rxbuf[512];
	hx_ioctl(&d,IOCTL_CFG_BUFFER,rxbuf,512);
    res = hx_write(&d,buff,l);
	hxt_fprintf(hxout,"write return %d\r\n",res);
    res = hx_read(&d,buff,512);
    if(res>0) {
        hxt_fprintf(hxout,"ret %u Bytes ",res);
        for(int i=0; i<res; i++) {
            hxt_fprintf(hxin,"%02hhX",buff[i]);
        }
        hxt_puts("\n");
    }else{
		hxt_fprintf(hxout,"read return %d\r\n",res);
	}
    return 0;
bad_arg:
    hxt_puts("Usage:");
    hxt_puts("\t wc <char_device>[:<params>]   Write hex code to char device.");
    return -1;
}


HXT_DEF_PROC(lk)
{
    int res;
    HX_DEV d;
    int hexmode = NULL;
    int echo = NULL;
    char *oc = NULL;
    char *ic = NULL;
    int linemode = NULL;
    char *dev = argv[1];
	int c=0;
    char hex[3]= {0};
    int n=0;
    if(argc<2)
        goto bad_arg;
    res = _dev_open(dev,&d);
    if(res)
        return -1;
    hx_get_arg_bool(argc-2,argv+2,"hexmode",&hexmode);
    hx_get_arg_bool(argc-2,argv+2,"echo",&echo);
    hx_get_arg(argc-2,argv+2,"ic",&ic);
    hx_get_arg(argc-2,argv+2,"oc",&oc);
    hx_get_arg_bool(argc-2,argv+2,"linemode",&linemode);

    while(1) {
//dev->console
        res = hxl_getc_noblock(&d,&c);
        if(res==0) {
            if(hexmode)
                hxt_printf("%02X",c);
            else {
                hxt_putc(c);
                if(ic) {
                    if(c=='\r' && strcmp(ic,"crlf")==0)
                        hxt_putc('\n');
                    else if(c=='\n' && strcmp(ic,"lfcr")==0)
                        hxt_putc('\r');
                }
            }
        }
//console->dev
        if(linemode) {
            char *line = 0;
            do {
                res = hxt_gets_echo_noblock(&line);
                if(IS_CTRL_CHAR(-res)) {
                    c = -res;
                    goto config;
                }
                if(res>=0 && line) {
                    if(hexmode) {
                        int l = strlen(line);
                        char buff[MAX_CHARS_LINE/2];
                        hx_hexcode2bin(line,l,buff);
                        hxl_send(&d,buff,l>>1);

                    } else {
                        hxl_put(&d,line);
                    }
                    if(oc) {
                        if(strcmp(oc,"cr")==0)
                            hxl_put(&d,"\r");
                        else if(strcmp(oc,"lf")==0)
                            hxl_put(&d,"\n");
                        else if(strcmp(oc,"crlf")==0)
                            hxl_put(&d,"\r\n");
                        else if(strcmp(oc,"null")==0) {
                            ;
                        } else ;
                    } else {
                        if(!hexmode)
                            //��hexģʽĬ��Windows��ʽ���
                            hxl_put(&d,"\r\n");
                    }
                }
            } while(res>=0);
            continue;
        }
        res = hxt_getc_noblock(&c);
        if(res==0) {
config:
            if(c=='') {
                hxt_puts("exit.");
                return 0;
            } else if(CTRL2CHAR(c)=='X') { //c==0x08){	//ctrl+x
                hexmode = hexmode?0:1;
                hxt_printf("HEX Mode %s\r\n",hexmode?"Enable":"Disable");
                continue;
            } else if(CTRL2CHAR(c)=='E') { //c==0x05){	//ctrl+e
                echo = echo?0:1;
                hxt_printf("ECHO Mode %s\r\n",echo?"Enable":"Disable");
                continue;
            } else if(CTRL2CHAR(c)=='O') { //c==0x0F){	//ctrl+o
                char ll[] = "cr,lf,crlf,null";
                next_value(&oc,ll);
                if(hexmode)
                    hxt_printf("Output End CR/LF Mode : %s\r\n",
                               oc?oc:"NULL");
                else
                    hxt_printf("Output End CR/LF Mode : %s\r\n",
                               oc?oc:"CRLR");
                continue;
            } else if(CTRL2CHAR(c)=='I') { //c==0x09){	//ctrl+i
                char ll[] = "lfcr,crlf";
                next_value(&ic,ll);
                hxt_printf("Input LF/CR Convert Mode : %s\r\n",
                           ic?ic:"No Convert");
                continue;
            } else if(CTRL2CHAR(c)=='L') { //c==0x0C){	//ctrl+l
                linemode = linemode?0:1;
                hxt_printf("Line Mode %s\r\n",linemode?"Enable":"Disable");
                continue;
            } else if(IS_CTRL_CHAR(c)) {
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

            if(hexmode) {
                if((c>='0' && c<='9')||(c>='A' && c<='F')||
                        (c>='a' && c<='f'))
                {
                    if(echo) hxt_putc(c);
                    hex[n] = (unsigned char)c;
                    n++;
                    if(n==2) {
                        n=0;
                        res = sscanf(hex,"%X",&c);
                        if(res == 1) {
                            hxl_putc(&d,c);
                            if(echo)hxt_putc(' ');
                        }
                    }

                }

            } else {
                hxl_putc(&d,c);
                if(echo) hxt_putc(c);
            }

        }
    }
    //return 0;
bad_arg:
    hxt_puts("Usage:");
    hxt_puts("\t lk   <char_device>[:<dev_param>]  [options ...]  ѡ�����ӵ����ַ��豸");
    hxt_puts("\t options: ");
    hxt_puts("\t      [hexmode]                (Ctrl+X)ʹ��ʮ����������/��ʾģʽ");
    hxt_puts("\t      [echo]                   (Ctrl+E)���Է����ַ�");
    hxt_puts("\t      [oc=cr|lf|crlf|null]     (Ctrl+O)���ת��,��ĩβ��ӿ��Ʒ�,����ģʽ��Ч");
    hxt_puts("\t      [ic=lfcr|crlf]           (Ctrl+I)����ת��,LF->LFCR|CR->CRLF,��������ģʽ��Ч");
    hxt_puts("\t      [linemode]               (Ctrl+L)ʹ����ģʽ,���뻻�д���һ�η���");
    return -1;
}
HXT_DEF_PROC(list)
{
	const DEV_T **p = hx_dev_next(NULL);
	do{
		if(p && *p){
			const DEV_T *device = *p;
			int type = device->devtype;
			char c;
			if(type>DT_INTERFACE) c = 'f';
			else if(type>DT_BLOCK) c = 'b';
			else if(type>DT_CHAR) c = 'c';
			else c = '?';
			hxt_printf("%c\t%u,%u\t%s\r\n",
				c,type,device->devid,device->name);
		}
		p = hx_dev_next(p);
	}while(p);
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

HXT_DEF_PROC(io_utils)
{
	int res;
	HX_DEV d;
	char cmd = argv[0][2];
	char *dev,*spin,*e;
	unsigned int pin;
	uint32_t val;
	char buf[32];
    if(argc<2)
        goto bad;
	if(cmd=='m' && argc<3)	//iomod need 2 args
		goto bad;
	dev = strtok_r(argv[1],".",&e);
	spin = strtok_r(NULL,".",&e);
	res = sscanf(spin,"%u",&pin);
	if(res!=1)
		goto bad;
	
	if(cmd=='m')
		sprintf(buf,"%s,%X",argv[2],1<<pin);
	else if(cmd=='v')
		buf[0] = 0;
	else
		sprintf(buf,"1,%X",1<<pin);
	res = _dev_open_with_param(dev,buf,&d);
    if(res)
		return res;
	
	if(cmd=='m'){
		hxt_fprintf(hxin,"I/O Mode %s.%u set : %s\n",dev,pin,argv[2]);
		return 0;
	}
	
	res = hx_read(&d,buf,4);
	if(res!=4){
		hxt_fprintf(hxerr,"read error,res=%d, is this not a gpio devices???\n",res);
		return -1;
	}
	val = HX_MSB_B2DW(buf);
	switch(argv[0][2]){
		case 's': 
			val |= (1<<pin); break;
		case 'c': 
			val &= ~(1<<pin); break;
		case 'v': 
		default:
			hxt_fprintf(hxin,"%s.%u = %s\n",dev,pin,(val&(1<<pin))?"1":"0");
			return 0;
	}
	HX_MSB_DW2B(val,buf);
	
	res = hx_write(&d,buf,4);
	if(res!=4){
		hxt_fprintf(hxerr,"write error,res=%d\n",res);
		return -1;
	}
	hxt_fprintf(hxin,"%s.%u = %s\n",dev,pin,(val&(1<<pin))?"1":"0");
    
	return 0;
bad:
    hxt_puts("Usage:");
	hxt_printf("\t %s <dev_name>.<pin_index> %s\n",argv[0],cmd=='m'?"<mode_val>":"");
    return -1;
}

const struct HXT_CMD_T g_cmd_list[] = {
    {"help",	hxt_help,		"��ʾ������Ϣ"},
    {"?",		hxt_help,			"��ʾ������Ϣ"},
    {"clear",	hxt_clear,		"����"},
    {"cr",		hxt_cr,			"����������"},
    {"version",	hxt_version,	"��ʾ�汾"},
    {"lk",		hxt_lk,			"���ӵ��ַ��豸"},
    {"ls",		hxt_list,		"�г��ļ��б�"},
    {"reset",	hxt_reset,		"��λ"},

    {"iomod",	hxt_io_utils,		"I/Oģʽ����"},
    {"ioval",	hxt_io_utils,		"I/O��ȡ����"},
    {"ioclr",	hxt_io_utils,		"I/O���0"},
    {"ioset",	hxt_io_utils,		"I/O���1"},

    {"wc",		hxt_wc,			"write char device"},
	{"rc",		hxt_rc,			"read char device"},
    {"param",	hxt_param,		"��ȡ�豸����"},
};

int g_cmd_count = sizeof(g_cmd_list)/sizeof(g_cmd_list[0]);
