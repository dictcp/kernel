#include <linux/types.h>
#include <linux/export.h>

#if 0
#include "boot.h"
#endif


/* From boot.h */
static inline void io_delay(void)
{
	const u16 DELAY_PORT = 0x80;
	asm volatile("outb %%al,%0" : : "dN" (DELAY_PORT));
}

/* Basic port I/O */
static inline void outb(u8 v, u16 port)
{
	asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}
static inline u8 inb(u16 port)
{
	u8 v;
	asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

/* End from boot.h */



static void udelay(int loops)
{
	while (loops--)
		io_delay();	/* Approximately 1 us */
}

static void beep(unsigned int hz)
{
	u8 enable;

	if (!hz) {
		enable = 0x00;		/* Turn off speaker */
	} else {
		u16 div = 1193181/hz;

		outb(0xb6, 0x43);	/* Ctr 2, squarewave, load, binary */
		io_delay();
		outb(div, 0x42);	/* LSB of counter */
		io_delay();
		outb(div >> 8, 0x42);	/* MSB of counter */
		io_delay();

		enable = 0x03;		/* Turn on speaker */
	}
	inb(0x61);		/* Dummy read of System Control Port B */
	io_delay();
	outb(enable, 0x61);	/* Enable timer 2 output to speaker */
	io_delay();
}

#define DOT_HZ		880
#define DASH_HZ		587
#define US_PER_DOT	125000

/* Okay, this is totally silly, but it's kind of fun. */
void send_morse(const char *pattern)
{
	char s;

	while ((s = *pattern++)) {
		switch (s) {
		case '.':
			beep(DOT_HZ);
			udelay(US_PER_DOT);
			beep(0);
			udelay(US_PER_DOT);
			break;
		case '-':
			beep(DASH_HZ);
			udelay(US_PER_DOT * 3);
			beep(0);
			udelay(US_PER_DOT);
			break;
		default:	/* Assume it's a space */
			udelay(US_PER_DOT * 3);
			break;
		}
	}
}
EXPORT_SYMBOL(send_morse);
