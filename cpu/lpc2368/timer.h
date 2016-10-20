/*****************************************************************************
 *   timer.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#define TIME_INTERVAL	(57600000/2-1)
/* depending on the CCLK and PCLK setting, CCLK = 57600000hz, PCLK = 1/2 CCLK
1Sec */


extern DWORD init_timer ( DWORD TimerInterval,BYTE timer_num ) ;
extern void enable_timer( BYTE timer_num );
extern void disable_timer( BYTE timer_num );
extern void reset_timer( BYTE timer_num );

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
