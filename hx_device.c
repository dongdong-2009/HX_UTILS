/*
 * hx_device.c
 *
 *  Created on: 2016Äê7ÔÂ25ÈÕ
 *      Author: houxd
 */

#include "hx_utils.h"
#include "stdio.h"
#include "string.h"
#include "stddef.h"


static DEV_TBL_T devtbl[MAX_DEVICE_COUNT] = {0};

const DEV_TBL_T *hx_get_devtbl(void)
{
	return devtbl;
}

int hx_devtbl_count(void)
{
	int i=0;
	for(i=0;i<MAX_DEVICE_COUNT;i++){
		if(devtbl[i].dev == NULL)
			break;
	}
	return i;
}

int hx_register_device(int type,const DEV_T *dev)
{
	int pos = hx_devtbl_count();
	if(pos==MAX_DEVICE_COUNT)
		return pos;
	devtbl[pos].dev = dev;
	devtbl[pos].dev_type = type;
	return 0;
}

//--------------------------------------------------------------

static int stdio_open(HX_DEV *dev,const char *s)
{
	return 0;
}
static int stdio_read(HX_DEV *dev,void *buf,int size)
{
	FILE *stdtbl[] = {stdin,stdout,stderr};
	return fread(buf,1,size,stdtbl[dev->pdev->devid]);
}
static int stdio_write(HX_DEV *dev,const void *buf, int size)
{
	FILE *stdtbl[] = {stdin,stdout,stderr};
	return fwrite(buf,1,size,stdtbl[dev->pdev->devid]);
}
static int stdio_close(HX_DEV *dev)
{
	return 0;
}
const DEV_DRV_T stdio_drv = {
	.open = stdio_open,
	.read = stdio_read,
	.write = stdio_write,
	.close = stdio_close,
};

//----------------------------------------------------------------
static const DEV_T cdev_stdin = {"stdin", 0,&stdio_drv};
static const DEV_T cdev_stdout = {"stdout",1,&stdio_drv};
static const DEV_T cdev_stderr = {"stderr",2,&stdio_drv};

HX_DEV hx_stdin,hx_stdout,hx_stderr;

void hx_device_init(void)
{
	hx_register_device(REG_CHAR,&cdev_stdin);
	hx_register_device(REG_CHAR,&cdev_stdout);
	hx_register_device(REG_CHAR,&cdev_stderr);
	
	hx_open("stdin","",&hx_stdin);
	hx_open("stdout","",&hx_stdout);
	hx_open("stderr","",&hx_stderr);
}

//--------------------------------------------------------------------

int hx_open(const char *dev_name,const char *param, HX_DEV* dev)
{
	int i;
	if(!dev) return -1;
	for(i=0;i<MAX_DEVICE_COUNT;i++){
		const DEV_T *device = devtbl[i].dev;
		if(device){
			if(strcmp(device->name,dev_name)==0){
				dev->pdev = device;
				const DEV_DRV_T *driver = (const DEV_DRV_T*)(device->driver);
				if(driver->open)
						return driver->open(dev,param);
				return 0;
			}
		}
	}
	hxt_printf("device not support.%s\n",dev_name);
	return -1;
}
int hx_read(HX_DEV *dev,void *buf,int size)
{
	if(!dev) 
		return -1;
	if(!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver->read)
		return driver->read(dev,buf,size);
	return 0;
}
int hx_poll(HX_DEV *dev,void *vp,int ip)
{
	if(!dev) 
		return -1;
	if(!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver->read)
		return driver->poll(dev,vp,ip);
	return 0;
}
int hx_write(HX_DEV *dev,const void *buf,int size)
{
	if(!dev) 
		return -1;
	if(!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver->write)
		return driver->write(dev,buf,size);
	return 0;
}
int hx_close(HX_DEV *dev)
{
	if(!dev) 
		return -1;
	if(!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver->close)
		return driver->close(dev);
	return 0;
}

