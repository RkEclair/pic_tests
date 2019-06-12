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
	for (; pace < 6; ++pace) {
		if (us < test) break;
		test <<= 1;
	}
	if (7 <= pace && us < 256) {
		pace += 2;
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
	TRISA = 0b1000; // RA3 is input
	TRISB = 0b100; // RB2 is input
	LATA = LATB = 0;

	/**
	 * |Ocx|     pins     |value to set|
	 * |---|--------------|------------|
	 * |  1|A0,B4,B7,B15  |5           |
	 * |  2|A1,B5,B8,B11  |5           |
	 * |  3|A3,B9,B10,B14 |5           |
	 * |4/5|A2,B6,B13     |5/6         |
   */
	RPB5R = 5; // use RB5 as output of OC2

	setPR2(15000);
	
	short short_width = 2000 / 4 * 5 - 1;
	short long_width = 4000 / 4 * 5 - 1;
	OC2R = OC1RS = short_width;
	OC2CONbits.OCM = 6;
	OC2CONbits.ON = 1;
	
	while (1) {
		OC2RS = (PORTAbits.RA3 ? short_width : long_width);
	}
}
