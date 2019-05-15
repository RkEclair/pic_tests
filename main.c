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

inline void setPR1(unsigned long us)
{
	if (us < 1597) {
		PR1 = us * 40 - 1;
		T1CONbits.TCKPS = 0;
	} else if (us < 13101) {
		PR1 = us * 5 - 1;
		T1CONbits.TCKPS = 1;
	} else if (us < 104855) {
		PR1 = us * 40 / 64 - 1;
		T1CONbits.TCKPS = 2;
	} else {
		PR1 = us * 40 / 256 - 1;
		T1CONbits.TCKPS = 3;
	}
	T1CONbits.ON = 1;
}

inline void setPR2(unsigned long us)
{
	long test = 1638;
	char pace = 0;
	for (; pace < 7; ++pace) {
		if (us < test) break;
		test <<= 1;
	}
	us >>= pace;
	PR2 = us * 40 - 1;
	T2CONbits.TCKPS = pace;
	T2CONbits.ON = 1;
}

#define HAS_MASK(bits, val) ((bits & val) == val)

int main()
{
	ANSELA = ANSELB = 0;
	TRISA = 0b1000;
	TRISB = 0b100;
	LATA = LATB = 0;

	setPR1(1000);
	unsigned count = 0, count2 = 0;
	char width = 1;
	while (1) {
		if (15 - count == 0) {
			LATBbits.LATB4 = 1;
			count = 0;
		}
		if (3000 <= count2) {
			width = (width - 1 ? 1 : 2);
			count2 = 0;
		}
		if (width - count == 0) {
			LATBbits.LATB4 = 0;
		}
		while (!IFS0bits.T1IF);
		IFS0bits.T1IF = 0;
		++count;
		++count2;
	}
}
