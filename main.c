#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>

#pragma config FPLLIDIV=DIV_2,FPLLMUL=MUL_20,FPLLODIV=DIV_2,FNOSC=FRCPLL,FSOSCEN=OFF,IESO=OFF,FPBDIV=DIV_1,FWDTEN=OFF,JTAGEN=OFF

void _mon_putc(char x) {
    while (!U1STAbits.TRMT);
    U1TXREG = x;
}

int main() {
    ANSELA = 0;
    ANSELB = 0x1000;
    TRISA = 0;
    TRISB = 0x1004;
    LATA = LATB = 0;

    PR1 = 12499;
    T1CONbits.TCKPS = 1;
    T1CONbits.ON = 1;
    
    AD1CHSbits.CH0SA = 12;
    AD1CON3bits.ADCS = 3;
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.ON = 1;
    
    unsigned sampled = 0;
    const char type = '*', max_len = 10;
    char i = 0;
    
    while(1) {
        IFS0bits.T1IF = 0;
        while(!IFS0bits.T1IF);
        
        AD1CON1bits.SAMP = 0;
        IFS0bits.AD1IF = 0;
        while(!IFS0bits.AD1IF);
        sampled = ADC1BUF0;

        LATB = sampled << 2;
        printf("%f", sampled * 0.004883);
        char len = sampled * max_len / 1024.0;
        for (i = 0; i < len; ++i) {
            printf("%c", type);
        }
        printf("\r\n");
    }
}
