/*****************************************************************************
 *   timer.c:  Timer C file for NXP LPC230x Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC23xx.h"		/* LPC23xx/24xx Peripheral Registers	*/
//#include "type.h"
//#include "irq.h"
//#include "timer.h"
//#include "led.h"
//#include "disp.h"
//#include "rtc.h"
//#include "key.h"
//#include "tkHeaderFile.h"
#include "hx_port.h"

static void Timer0Handler (void) __irq
{
    T0IR=1;//中断寄存器，写入值清除中断

	TICK_COUNT_INC(10);

    VICVectAddr = 0;//向量地址寄存器，中断完成以后必须写入任意值
}


/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
static void enable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
        T0TCR = 1;
    }
    else if ( timer_num == 1 )
    {
        T1TCR = 1;
    }
    else if ( timer_num == 2 )
    {
        T2TCR = 1;
    }
    else if ( timer_num == 3 )
    {
        T3TCR = 1;
    }
    return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 o 1
** Returned value:		None
**
******************************************************************************/
static void disable_timer( BYTE timer_num )
{
    if ( timer_num == 0 )
    {
        T0TCR = 0;
    }
    else if(timer_num == 1)
    {
        T1TCR = 0;
    }
    else if(timer_num == 2)
    {
        T2TCR = 0;
    }
    else if(timer_num == 3)
    {
        T3TCR = 0;
    }
    return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0  1 2 3
** Returned value:		None
**
******************************************************************************/
static void reset_timer( BYTE timer_num )
{
    DWORD regVal;

    if ( timer_num == 0 )
    {
        T0TCR|=0x02;
        T0TCR&=0xfd;
    }
    else if(timer_num == 1)
    {
        regVal = T1TCR;
        regVal |= 0x02;
        T1TCR = regVal;
    }
    else if(timer_num == 2)
    {
        regVal = T2TCR;
        regVal |= 0x02;
        T2TCR = regVal;
    }
    else if(timer_num == 3)
    {
        regVal = T3TCR;
        regVal |= 0x02;
        T3TCR = regVal;
    }
    return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			None
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
**
******************************************************************************/
static DWORD init_timer ( DWORD TimerInterval,BYTE timer_num )
{
    if(timer_num==0)
    {
        T0TCR=0;
        T0PR=0;
        T0TC=0;
        T0MR0 = TimerInterval;
        T0MCR|= 3;/* Interrupt and Reset on MR0 */
        if ( install_irq( TIMER0_INT, (void *)Timer0Handler, 2 ) == FALSE )
        {
            return (FALSE);
        }
        else
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

void hx_tick_count_init(void)
{
	init_timer((Fpclk / 100 - 1),0);
	enable_timer(0);
}
