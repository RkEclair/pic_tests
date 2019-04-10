#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

unsigned char count_100ms = 0;

void __ISR(_TIMER_1_VECTOR, IPL3SOFT) Timer1Handler() {
    IFS0bits.T1IF = 0;
    ++count_100ms;
}

unsigned char reset = 0;

void __ISR(_EXTERNAL_2_VECTOR, IPL3SOFT) INT2Handler() {
    IFS0bits.INT2IF = 0;
    reset = 1;
}

unsigned char key = 0;

void __ISR(_UART_1_VECTOR, IPL3SOFT) UART1Handler() {
    while (U1STAbits.URXDA) key = U1RXREG;
    IFS1bits.U1RXIF = 0;
}

void _mon_putc(char x) {
    while (!U1STAbits.TRMT);
    U1TXREG = x;
}

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0b100;
    TRISB = 0b100;
    LATA = LATB = 0;

    RPB3R = 1;

    U1BRG = 1041;
    U1STAbits.UTXEN = 1;
    U1MODEbits.BRGH = 1;
    U1MODEbits.ON = 1;

    INT2R = 0;
    IEC0bits.INT2IE = 1;
    IPC2bits.INT2IP = 3;

    IPC8bits.U1IP = 3;
    IEC1bits.U1RXIE = 1;

    PR1 = 12499;
    T1CONbits.TCKPS = 1;
    T1CONbits.ON = 1;
    IPC1bits.T1IP = 3;
    IEC0bits.T1IE = 1;

    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();

    unsigned char count = 0, i = 10, amount = 1;
    while (1) {
        while (count_100ms != 10) {
            if (reset) {
                count = reset = 0;
                printf("RESET\r\n");
            }
            if (key == 'd' || key == 'D') {
                amount = -1;
                printf("Counting down");
            } else if (key == 'u' || key == 'U') {
                amount = 1;
                printf("Counting up");
            }
        }

        count_100ms = 0;
        printf("%2d %#02x\r\n", count, count);
        count += amount;
    }
}
