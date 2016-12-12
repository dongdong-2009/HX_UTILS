/*
 * hxd_ioport.c
 *
 *  Created on: 2016Äê11ÔÂ17ÈÕ
 *      Author: houxd
 */
#include "stdio.h"
#include "hx_utils.h"
#include "hxd_ioport.h"
#include "string.h"

/*
	s is param whit asc 
*/
int hxd_ioport_open(HX_DEV *dev,const char *s)
{
	const DEV_T *pdev = dev->pdev;
	const IOPORT_DRV_T *drv = (const IOPORT_DRV_T *)pdev->driver;
	uint port = pdev->devid;
	int mode = 1;
	if(s || s[0])
		mode = s[0] - '0';
	drv->iodesc->iomode(port,0xFFFFFFFFu,mode);
	return 0;
}
/*
	size must be 4
	buf convert to DWORD*
*/
int hxd_ioport_read(HX_DEV *dev,void *buf,int size)
{
	const DEV_T *pdev = dev->pdev;
	const IOPORT_DRV_T *drv = (const IOPORT_DRV_T *)pdev->driver;
	uint port = pdev->devid;
	*(unsigned int*)buf = drv->iodesc->ioval(port);
	return 4;
}
int hxd_ioport_write(HX_DEV *dev,const void *buf, int size)
{
	const DEV_T *pdev = dev->pdev;
	const IOPORT_DRV_T *drv = (const IOPORT_DRV_T *)pdev->driver;
	uint port = pdev->devid;
	drv->iodesc->ioctrl(port,*(const unsigned int*)buf);
	return 4;
}
int hxd_ioport_close(HX_DEV *dev)
{
	return hxd_ioport_open(dev,"0");
}

