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


static const DEV_T *devtbl[MAX_DEVICE_COUNT] = {0};

const DEV_T **hx_get_devtbl(void)
{
	return devtbl;
}

int hx_devtbl_count(void)
{
	int i=0;
	for(i=0;i<MAX_DEVICE_COUNT;i++){
		if(devtbl[i] == NULL)
			break;
	}
	return i;
}
const void *hx_find_device(int devtype,int devid)
{
	for(int i=0;i<MAX_DEVICE_COUNT;i++){
		if(devtbl[i] == NULL)
			break;
		if(devtbl[i]->devtype==devtype && devtbl[i]->devid==devid)
			return devtbl[i];
	}
	return NULL;
}
const void *hx_interface_get_member(const INF_T *pinf,int m_label)
{
	int count = pinf->count;
	for(int i=0;i<count;i++){
		if(pinf->members[i].label == m_label)
			return pinf->members[i].dev;
	}
	return 0;
}
int hx_register_device(const DEV_T *dev)
{
	//int pos = hx_devtbl_count();
	for(int i=0;i<MAX_DEVICE_COUNT;i++){
		if(!devtbl[i]
			|| strcmp(devtbl[i]->name,dev->name)==0){
			devtbl[i] = dev;
			return 0;
		}
	}
	return hx_devtbl_count();
}
/*
	int pos = hx_devtbl_count();
	if(pos==MAX_DEVICE_COUNT)
		return pos;
	devtbl[pos].dev = dev;
	devtbl[pos].dev_type = type;
	return 0;
*/
//--------------------------------------------------------------
static FILE* __stdin_for_out = NULL;
static int stdio_open(HX_DEV *dev,const char *s)
{
	if(dev->pdev->devid == 0){
		__stdin_for_out = fopen("g","a+");
	}
	return 0;
}
static int stdio_read(HX_DEV *dev,void *buf,int size)
{
	int id = dev->pdev->devid;
	FILE *stdtbl[] = {stdin,stdout,stderr};
	return fread(buf,1,size,stdtbl[id]);
}
static int stdio_write(HX_DEV *dev,const void *buf, int size)
{
	FILE * stdtbl[] = {stdin,stdout,stderr};
	int res;
	int id = dev->pdev->devid;
	if(id==0 && __stdin_for_out){
		res = fwrite(buf,1,size,__stdin_for_out);
		fflush(stdtbl[id]);
	}else{
		res = fwrite(buf,1,size,stdtbl[id]);
		fflush(stdtbl[id]);
	}
	return res;
}
static int stdio_close(HX_DEV *dev)
{
	return 0;
}
static const DEV_DRV_T stdio_drv = {
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
	hx_register_device(&cdev_stdin);
	hx_register_device(&cdev_stdout);
	hx_register_device(&cdev_stderr);
	
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
		const DEV_T *device = devtbl[i];
		if(device){
			if(strcmp(device->name,dev_name)==0){
				dev->pdev = device;
				const DEV_DRV_T *driver = (const DEV_DRV_T*)(device->driver);
				if(driver && driver->open)
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
	if(driver && driver->read)
		return driver->read(dev,buf,size);
	return 0;
}

int hx_ioctl(HX_DEV *dev,int cmd,...)
{
	int res = -1;
	va_list va;
	va_start(va,cmd);
	if(!dev ||!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver && driver->ioctl)
		return driver->ioctl(dev,cmd,va);
	va_end(va);
	return res;
}
int hx_write(HX_DEV *dev,const void *buf,int size)
{
	if(!dev) 
		return -1;
	if(!dev->pdev)
		return -2;
	const DEV_DRV_T *driver = (DEV_DRV_T*)(dev->pdev->driver);
	if(driver && driver->write)
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
	if(driver && driver->close)
		return driver->close(dev);
	return 0;
}

