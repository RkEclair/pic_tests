#include <xc.h>
#include <stdio.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

int _mon_getc(int canblock) {
    while (!U1STAbits.URXDA);
    return U1RXREG;
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

    U1RXR = 4;
    RPB3R = 1;

    U1BRG = 1041;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
    U1MODEbits.BRGH = 1;
    U1MODEbits.ON = 1;

    unsigned char data;

    while (1) {
        scanf("%c", &data);
        if (!('0' <= data && data <= '9')) continue;
        switch (data - '0') {
            case 0:
                printf("");
                break;
            case 1:
                printf("");
                break;
            case 2:
                printf("");
                break;
            case 3:
                printf("");
                break;
            case 4:
                printf("");
                break;
            case 5:
                printf("");
                break;
            case 6:
                printf("");
                break;
            case 7:
                printf("");
                break;
            case 8:
                printf("");
                break;
            case 9:
                printf("");
                break;
        }
    }
}
