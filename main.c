#include <xc.h>
#include <stdio.h>

#pragma config FPLLIDIV=DIV_2
#pragma config FPLLMUL=MUL_20
#pragma config FPLLODIV=DIV_2
#pragma config FNOSC=FRCPLL
#pragma config FSOSCEN=OFF
#pragma config IESO=OFF
#pragma config FPBDIV=DIV_1
#pragma config FWDTEN=OFF
#pragma config JTAGEN=OFF

int _mon_getc(int can_block) {
	while(!U1STAbits.URXDA);
	return U1RXREG;
}

void _mon_putc(char chr) {
	while(!U1STAbits.TRMT);
	U1TXREG = chr;
}

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
#define POW_2(n) (1 << n)

short const lit_table[10] = {
	POW_2(4),
	POW_2(5),
	POW_2(6),
	POW_2(7),
	POW_2(8),
	POW_2(9),
	POW_2(10),
	POW_2(11),
	0xff << 4,
	0,
};

char const* const mes_table[10] = {
	"I gotta 0",
	"You typed 1",
	"It's 2, OK?",
	"3 is my lucky number",
	"Party 4 u",
	"5 dives",
	"666",
	"7 is bad number",
	"8 8 8 8 8 8 8 8",
	"knight waits 9 o'night",
};

int main()
{
	ANSELA = ANSELB = 0;
	TRISA = 0;
	TRISB = 0b100; // RB2 is input
	LATA = LATB = 0;

	U1RXR = 4; // use rb2 to input U1RX
	RPB3R = 1; // use rb3 to output U1TX
	
	U1BRG = 1041;
	U1STAbits.URXEN = 1;
	U1STAbits.UTXEN = 1;
	U1MODEbits.BRGH = 1;
	U1MODEbits.ON = 1;
	
	unsigned char recieved;
	while (1) {
		scanf("%c", &recieved);
		char num = recieved - '0';
		LATB = lit_table[num];
		printf("%s\r\n", mes_table[num]);
	}
}
