#include <xc.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0;
    TRISB = 0b100;
    LATA = LATB = 0;
    
    PR1 = 14999;
    T1CONbits.TCKPS = 1;
    T1CONbits.ON = 1;

    while (1) {
        while(!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
        LATBbits.LATB4 ^= 1;
    }
}
