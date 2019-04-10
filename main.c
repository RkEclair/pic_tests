#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

unsigned char count_100ms = 0;

void __ISR(_TIMER_1_VECTOR, IPL3SOFT) Timer1Handler() {
    IFS0bits.T1IF = 0;
    ++count_100ms;
}

void _mon_putc(char x) {
    while(!U1STAbits.TRMT);
    U1TXREG = x;
}

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0;
    TRISB = 0b100;
    LATA = LATB = 0;

    RPB3R = 1;

    U1BRG = 1041;
    U1STAbits.UTXEN = 1;
    U1MODEbits.BRGH = 1;
    U1MODEbits.ON = 1;

    PR1 = 12499;
    T1CONbits.TCKPS = 1;
    T1CONbits.ON = 1;
    IPC1bits.T1IP = 3;
    IEC0bits.T1IE = 1;

    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();

    unsigned char count = 0, i = 10;
    while (1) {
        while(count_100ms != 10); count_100ms = 0;
        printf("%2d %#02x\r\n", count, count);
        ++count;
    }
}
