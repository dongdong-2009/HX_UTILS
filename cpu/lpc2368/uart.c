//#include "LPC23xx.h"                        /* LPC23xx/24xx definitions */
//#include "target.h"
//#include "irq.h"
//#include "type.h"
//#include "uart.h"
//#include "hx_port.h"

#include "LPC23XX.h"
#include "./cpu/lpc2368/uart.h"
#include "./cpu/lpc2368/irq.h"
#include "./cpu/lpc2368/target.h"


void UART1Handler (void) __irq
{
    tk_uint8 IIRValue, LSRValue;
    tk_uint8 Dummy = Dummy;
    //	  I_UC_UNION i_uc_Tmp;
    //  IENABLE;				/* handles nested interrupt */
    IIRValue = U1IIR;
    IIRValue >>= 1;			/* skip pending bit in IIR */
    IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
    if ( IIRValue == IIR_RLS )		/* Receive Line Status */
    {
        LSRValue = U1LSR;
        /* Receive Line Status */
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            /* There are errors or break interrupt */
            /* Read LSR will clear the interrupt */
            Dummy = U1RBR;		/* Dummy read on RX to clear interrupt, then bail out */
            //	  IDISABLE;
            VICVectAddr = 0;		/* Acknowledge Interrupt */
            return;
        }
        if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
        {
			UART_RX_BYTE(1,U1RBR);
            
        }
    }
    else if (IIRValue==IIR_RDA )	/* Receive Data Available */
    {
		UART_RX_BYTE(1,U1RBR);       
    }
    else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
    {
        /* Character Time-out indicator */
        U1FCR = 0x07;
        //Dummy=U0RBR;
    }
    else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
    {
        /* THRE interrupt */
        LSRValue = U1LSR;		/* Check status in the LSR to see if
		valid data in U0THR or not */
        if ( LSRValue & LSR_THRE )
        {
           UART_TX_BYTE(1);
        }
    }
    //  IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}

void UART0Handler (void) __irq
{
    tk_uint8 IIRValue, LSRValue;
    tk_uint8 Dummy = Dummy;
    //	  I_UC_UNION i_uc_Tmp;
    //  IENABLE;				/* handles nested interrupt */

    IIRValue = U0IIR;

    IIRValue >>= 1;			/* skip pending bit in IIR */
    IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
    if ( IIRValue == IIR_RLS )		/* Receive Line Status */
    {
        LSRValue = U0LSR;
        /* Receive Line Status */
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            /* There are errors or break interrupt */
            /* Read LSR will clear the interrupt */
            Dummy = U0RBR;		/* Dummy read on RX to clear
			interrupt, then bail out */
            //IDISABLE;   // by tiger 2012-12-08 注解
            VICVectAddr = 0;		/* Acknowledge Interrupt */
            return;
        }
        if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
        {
			UART_RX_BYTE(0,U0RBR);
        }
    }
    else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
    {
		UART_RX_BYTE(0,U0RBR);

    }
    else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
    {
        /* Character Time-out indicator */
        U0FCR = 0x07;
        //Dummy = U1RBR;
    }
    else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
    {
        /* THRE interrupt */
        LSRValue = U0LSR;		/* Check status in the LSR to see if
		valid data in U1THR or not */
        if ( LSRValue & LSR_THRE )
        {
			UART_TX_BYTE(0);

        }
    }
    //  IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}
void UART2Handler (void) __irq
{
    tk_uint8 IIRValue, LSRValue;
    tk_uint8 Dummy = Dummy;
    //	  I_UC_UNION i_uc_Tmp;
    //  IENABLE;				/* handles nested interrupt */

    IIRValue = U2IIR;

    IIRValue >>= 1;			/* skip pending bit in IIR */
    IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
    if ( IIRValue == IIR_RLS )		/* Receive Line Status */
    {
        LSRValue = U2LSR;
        /* Receive Line Status */
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            /* There are errors or break interrupt */
            /* Read LSR will clear the interrupt */
            Dummy = U2RBR;		/* Dummy read on RX to clear interrupt, then bail out */
            //	  IDISABLE;
            VICVectAddr = 0;		/* Acknowledge Interrupt */
            return;
        }
        if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
        {
            UART_RX_BYTE(2,U2RBR);
        }
    }
    else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
    {
		UART_RX_BYTE(2,U2RBR);
       
    }
    else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
    {
        /* Character Time-out indicator */
        //Dummy=U2RBR;
        U2FCR = 0x07;
    }
    else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
    {
        /* THRE interrupt */
        LSRValue = U2LSR;		/* Check status in the LSR to see if
		valid data in U0THR or not */
        if ( LSRValue & LSR_THRE )
        {
		UART_TX_BYTE(2);
        }
    }
    //  IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}

// WR 2013-05-22
void UART3Handler (void) __irq
{
    tk_uint8 IIRValue, LSRValue;
    tk_uint8 Dummy = Dummy;
    //  IENABLE;				/* handles nested interrupt */
    IIRValue = U3IIR;
    IIRValue >>= 1;			/* skip pending bit in IIR */
    IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
    if ( IIRValue == IIR_RLS )		/* Receive Line Status */
    {
        LSRValue = U3LSR;
        /* Receive Line Status */
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
        {
            /* There are errors or break interrupt */
            /* Read LSR will clear the interrupt */
            Dummy = U3RBR;		/* Dummy read on RX to clear interrupt, then bail out */
            //	  IDISABLE;
            VICVectAddr = 0;		/* Acknowledge Interrupt */
            return;
        }
        if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
        {
			int data = U3RBR;
			UART_RX_BYTE(3,data);
            
        }
    }
    else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
    {
		int data = U3RBR;
		UART_RX_BYTE(3,data);
       
    }
    else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
    {
        /* Character Time-out indicator */
        U3FCR = 0x07;
        //Dummy=U3RBR;
    }
    else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
    {
        /* THRE interrupt */
        LSRValue = U3LSR;		/* Check status in the LSR to see if
		valid data in U3THR or not */
        if ( LSRValue & LSR_THRE )
        {
			UART_TX_BYTE(3);
        }
    }
    //  IDISABLE;
    VICVectAddr = 0;		/* Acknowledge Interrupt */
}


tk_uint32 UART_Init( tk_uint32 PortNum, tk_uint32 baudrate )
{
    tk_uint32 Fdiv;
    tk_uint8 Dummy ;
    Dummy=Dummy;
    if(PortNum == 0 )//GPRS通信
    {
        //PCONP |= 1<<3;
        //PINSEL0 |= 0x00000050;     // zwjiang modify 2012-6-20  /* RxD0 and TxD0 */
        U0LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
        Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
        U0DLM = Fdiv / 256;
        U0DLL = Fdiv % 256;
        U0LCR = 0x03;		/* DLAB = 0 */
        U0FCR = 0x00;
        U0FCR = 0x06;		/* Enable and reset TX and RX FIFO. */
        Dummy = U0RBR;
        if(install_irq( UART0_INT, (void *)UART0Handler, HIGHEST_PRIORITY )==FALSE)
        {
            return (FALSE);
        }
        U0IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
        return (TRUE);
    }
    else if ( PortNum == 1 )
    {
       // PCONP |= 1<<4;
       // PINSEL4 |= 0x0000000a;       /*p2.0 p2.1 RxD1 and TxD1 */
        U1LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
        Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
        U1DLM = Fdiv / 256;
        U1DLL = Fdiv % 256;
        U1LCR = 0x03;		/* DLAB = 0 */
        U1FCR = 0x00;
        U1FCR = 0x06;//FIFO控制寄存器
        Dummy = U1RBR;
        if(install_irq(UART1_INT, (void *)UART1Handler, HIGHEST_PRIORITY )==FALSE )
        {
            return (FALSE);
        }
        U1IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
        return (TRUE);
    }
    else if(PortNum == 2)
    {
        //PCONP |= 1<<24;
       // PINSEL4 |= 0x000a0000;       /* RxD2 and TxD2 P2.8 P2.9*/
        U2LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
        Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
        U2DLM = Fdiv / 256;
        U2DLL = Fdiv % 256;
        U2LCR = 0x03;		/* DLAB = 0 */
        U2FCR = 0x00;
        U2FCR = 0x06;		/* Enable and reset TX and RX FIFO. */
        Dummy = U2RBR;
        if ( install_irq( UART2_INT, (void *)UART2Handler, HIGHEST_PRIORITY ) == FALSE )
        {
            return (FALSE);
        }
        U2IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART2 interrupt */
        return (TRUE);
    }
    else if (PortNum == 3)//232转接485
    {
      //  PCONP|= 1<<25;
      //  PINSEL0|=0x0000000A; /* Enable TxD3 P0.0 RxD3 P0.1 */
        U3LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
        Fdiv = ( Fpclk / 16 ) / baudrate ;	/*baud rate */
        U3DLM = Fdiv / 256;
        U3DLL = Fdiv % 256;
        U3LCR = 0x03;		/* DLAB = 0 */
        U3FCR = 0x00;
        U3FCR = 0x06;
        Dummy = U3RBR;
        if(install_irq( UART3_INT, (void *)UART3Handler, HIGHEST_PRIORITY )==FALSE)
        {
            return (FALSE);
        }
        U3IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
        return (TRUE);
    }                                     // zwjiang add begin 2012-5-8
    return( FALSE );
}

/************************使能串口******************/
void EnableUart(BYTE portnum)
{
    if(portnum==0)
    {
        VICIntEnable|=(1<<UART0_INT);
    }
    else if(portnum==1)
    {
        VICIntEnable|=(1<<UART1_INT);
    }
    else if(portnum==2)
    {
        VICIntEnable|=(1<< UART2_INT);

    }
    else if(portnum==3)
    {
        VICIntEnable|=(1<< UART3_INT);
    }
}
/********************禁止串口********************/
void DisableUart(BYTE portnum)
{
    if(portnum==0)
    {
        VICIntEnClr|=(1<<UART0_INT);
    }
    else if(portnum==1)
    {
        VICIntEnClr|=(1<<UART1_INT);
    }
    else if(portnum==2)
    {
        VICIntEnClr|=(1<<UART2_INT);
    }
    else if(portnum==3)
    {
        VICIntEnClr|=(1<<UART3_INT);
    }
}
