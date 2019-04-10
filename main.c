#include <xc.h>
#include <stdio.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

int _mon_getc(int canblock) {
    while (!U1STAbits.URXDA);
    return U1RXREG;
}

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0;
    TRISB = 0b100;
    LATA = LATB = 0;

    U1RXR = 4;

    U1BRG = 1041;
    U1STAbits.URXEN = 1;
    U1MODEbits.BRGH = 1;
    U1MODEbits.ON = 1;

    unsigned char data;

    while (1) {
        scanf("%c", &data);
        if ('0' <= data && data <= '9')
            PORTB = PORTB ^ (1 << data - '0');
    }
}
