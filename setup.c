#include "header.h"
#include "serial.h"
#include "tmp36.h"

/* statiska funktioner, inline funktioner f�r ej vara f�r stor */
static inline void init_ports(void);
static inline void init_timers(void);
static void init_interrupts();

/* initierar portar, timers, TMP36, seriell �verf�ring */
void setup(void)
{
    init_ports();
	init_timers();
	init_interrupts();
	tmp36_init(&temp1, A2);
	serial_init(9600);
	button_pressed_enabled = false;
	return;
}

/* portb button1 aktiveras, leds anv�nds ej */
static inline void init_ports(void)
{
	DDRB = (1 << LED1) | (1 << LED2) | (1 << LED3);
	PORTB = (1 << BUTTON1);
	return;
}

/************************************************************   
 aktiverar Timer 0
             1. Aktiverar globalt avbrott asm("SEI")
			 2. Timer 0, Normal mode, avbrott 0.128ms genom
			    ettst�llning av bit CS01 i kontroll-register 
				TCCROB.
			 3.	Maskregister TIMSK0, bit TOIE0.
************************************************************/
static inline void init_timers(void)
{
	asm("SEI"); // ettst�llning av I flaggan
	TCCR0B = (1 << CS01);
	TIMSK0 = (1 << TOIE0);
	
	TCCR1B = (1 << CS11) | (1 << WGM12); // timer 1, CTC mode WGM12 biten ettst�lls, prescaler clk/8 = avbrott var 0.128ms
	OCR1A = 256; // s�tter maxv�rdet f�r uppr�kning till 256, 8-bitar
	TIMSK1 = (1 << OCIE1A); // maskregister
	

	return;
}

/* PCI avbrott f�r knapptryck */

static void init_interrupts()
{
	asm("SEI");
	PCICR = (1 << PCIE0);
	PCMSK0 = (1 << PCINT5);
	return;
}

