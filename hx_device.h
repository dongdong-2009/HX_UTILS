#ifndef __HX_DEVICE_H__
#define __HX_DEVICE_H__

#pragma anon_unions

#define MAX_DEVICE_COUNT				(64)


/*
	device instance
*/
typedef struct __DEV_T{
	const char *name;
	int devid;
	const void *driver;
} DEV_T;

//externs use, instance of opened device
typedef struct __HX_DEVICE {
	int type;		//type and flags
	int offset;
	const DEV_T *pdev;
} HX_DEV;

/*
	device driver 
*/
typedef struct __DEV_DRV_T{
	/*
	id	: device instans identifation
	s	: special params string for device	
	return : success is 0, others is error
	*/
	int (*open)(HX_DEV *dev,const char *s);
	/*
	buf : buffer of read out.
	size : 	>0 length of wangt read no-block. 
	return: >0 count of read chars
			<=0 is error
	*/
	int (*read)(HX_DEV *dev,void *buf,int size);
	/*
	size :	>0 length of want write
	return :>0 is success,others is error
	*/
	int (*write)(HX_DEV *dev,const void *buf,int size);
	/*
	return : =0 is success,others is error
	*/
	int (*close)(HX_DEV *dev);
	
	int (*poll)(HX_DEV *dev,void *vp,int ip);
	
} DEV_DRV_T;



//innner 
typedef struct __DEV_TBL_T{
	const DEV_T *dev;
	int dev_type;
} DEV_TBL_T;
extern const DEV_TBL_T *hx_get_devtbl(void);
extern int hx_devtbl_count(void);
extern void hx_device_init(void);

/*
	stdard input/output
*/
extern HX_DEV hx_stdin,hx_stdout,hx_stderr;
#define hxin	(&hx_stdin)
#define hxout	(&hx_stdout)
#define hxerr	(&hx_stderr)


extern int hx_open(const char *dev_name,const char *param, HX_DEV* dev);
extern int hx_read(HX_DEV *dev,void *buf,int size);
extern int hx_write(HX_DEV *dev,const void *buf,int size);
extern int hx_close(HX_DEV *dev);
extern int hx_poll(HX_DEV *dev,void *vp,int ip);

#define REG_CHAR 		0
#define REG_BLOCK		1
extern int hx_register_device(int type,const DEV_T *dev);
#define hx_register_char_device(d)	hx_register_device(REG_CHAR,d)
#define hx_register_block_device(d)	hx_register_device(REG_BLOCK,d)


#endif
