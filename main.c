#include <xc.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

/*
inline void delay_ms(unsigned char cyc) {
    while (cyc-- != 0) {
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
    }
}
*/

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0b1000;
    TRISB = 0b100;
    LATA = LATB = 0;

    RPB4R = 5;

    PR2 = 14999;
    T2CONbits.TCKPS = 0;
    T2CONbits.ON = 1;
    
    OC1RS = 999;
    OC1R = OC1RS;
    OC1CONbits.OCM = 6;
    OC1CONbits.ON = 1;
    
    while (1) {
        OC1RS = PORTAbits.RA3 ? 999 : 1999;
    }
}
