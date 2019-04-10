#include <xc.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

inline void delay_ms(unsigned char cyc) {
    while (cyc-- != 0) {
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
    }
}

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0b1000;
    TRISB = 0b100;
    LATA = LATB = 0;

    PR1 = 999;
    T1CONbits.TCKPS = 0;
    T1CONbits.ON = 1;

    while (1) {
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
        LATBbits.LATB4 = 1;
        delay_ms(PORTAbits.RA3 ? 1 : 2);
        LATBbits.LATB4 = 0;
        delay_ms(15 - 1);
    }
}
