#include "header.h"
#include "tmp36.h"

/* counter variabel en f�r tidsr�kning i minuter */

static volatile uint32_t time_counter;

/******************************************************************************** 
  ISR-interrupt service routine, avbrottsrutin f�r timer 0.
  som skiver ut temperatur en g�ng i minuten.
   
  ++counter r�knar upp till antal avbrott. timer_get_max_count(sekunder) 
  funktionen r�knar ut antal sekunder som f�rdr�jning ska ske, efter uppr�kning 
  skrivs temperaturen ut i terminalen samnt tid i minuter. PCI-avbrott �ter 
  aktiveras inne i if satsen.
 ********************************************************************************/
ISR (TIMER0_OVF_vect)
{
	static volatile uint32_t counter = 0;
	
	if (++counter >= timer_get_max_count_sec(60)) 
	 {
		 time_counter++;
		 serial_print_new_line();
		 tmp36_print_temperature(&temp1);
		 serial_print_string(" Time in minutes: ");
		 serial_print_integer(time_counter);
		 serial_print_new_line();
		 counter = 0;
		 
		 asm("SEI");
		 PCICR = (1 << PCIE0);
		 PCMSK0 |= (1 << PCINT5);
	 }
	return;
}

/**************************************************************************************************** 
   Vid nedtryckning av tryckknapp togglas temperaturm�tning 1 g�ng i sekunden, annars 1 g�ng i minuten.
   PCI-avbrott �ter aktiveras i if satsen. 
 *****************************************************************************************************/
ISR (PCINT0_vect)
{
	PCMSK0 &= ~(1 << PCINT5);   // Inaktiverar PCI_avbrott
	TIMSK1 = (1 << OCIE1A);     // S�tter p� timer 1 i 300 ms, debounce.

	 if (BUTTON1_PRESSED)
	 {
		 button_pressed_enabled = !button_pressed_enabled;
	
		  while (button_pressed_enabled)
		  {
			 tmp36_print_temperature(&temp1);q
			 serial_print_new_line();
		     delay_ms(1000);
			 
		     asm("SEI");
		     PCICR = (1 << PCIE0);
		     PCMSK0 |= (1 << PCINT5);
		  }
		
	 }
	 return;
}


/**************************************************************************************************** 
   Timer 1 anv�nds f�r debounce i 300 ms. PCI-avbrott �ter aktiveras i if satsen. 
 *****************************************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	static volatile uint16_t counter = 0;
	
	if(++counter >= timer_get_max_count_ms(300))
	{
		 counter = 0;
		 asm("SEI");
		 PCICR = (1 << PCIE0);
		 PCMSK0 |= (1 << PCINT5);
	}
	return;
	
}

