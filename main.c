#include <xc.h>

#pragma config FPLLIDIV=DIV_2
#pragma config FPLLMUL=MUL_20
#pragma config FPLLODIV=DIV_2
#pragma config FNOSC=FRCPLL
#pragma config FSOSCEN=OFF
#pragma config IESO=OFF
#pragma config FPBDIV=DIV_1
#pragma config FWDTEN=OFF
#pragma config JTAGEN=OFF

inline void delay_ms(unsigned char cyc) {
    while (cyc-- != 0) {
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
    }
}

inline void setPR1(unsigned long us) {
	if (us < 1598) {
		PR1 = us * 40 - 1;
		T1CONbits.TCKPS = 0;
	} else if (us < 13102) {
		PR1 = us * 5 - 1;
		T1CONbits.TCKPS = 1;
	} else if (us < 104856) {
		PR1 = us * 40 / 64 - 1;
		T1CONbits.TCKPS = 2;
	} else {
		PR1 = us * 40 / 256 - 1;
		T1CONbits.TCKPS = 3;
	}
    T1CONbits.ON = 1;
}

int main() {
    ANSELA = ANSELB = 0;
    TRISA = 0b1000;
    TRISB = 0b100;
    LATA = LATB = 0;

    setPR1(1000);

    while (1) {
        while (!IFS0bits.T1IF);
        IFS0bits.T1IF = 0;
        LATBbits.LATB4 = 1;
        delay_ms(PORTAbits.RA3 ? 1 : 2);
        LATBbits.LATB4 = 0;
        delay_ms(15 - 1);
    }
}
