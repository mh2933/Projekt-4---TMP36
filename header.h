
#ifndef HEADER_H_
#define HEADER_H_

#include "misc.h"

struct tmp36 temp1;

#define LED1 PORTB0
#define LED2 PORTB1
#define LED3 PORTB2

#define BUTTON1 PORTB5

// s. 58 i datablad beskriver toggling av dataregistret
#define LED1_TOGGLE PINB = (1 << LED1)
#define LED2_TOGGLE PINB = (1 << LED2)
#define LED3_TOGGLE PINB = (1 << LED3)

/* för aktivering via tryckknapp */
#define BUTTON1_PRESSED (PINB & (1 << BUTTON1)) 

/* sätt tid för TIMER 0 */
#define TIMER0_TIME_MS 1
#define TIMER0_TIME_SEC (TIMER0_TIME_MS * 1000)
#define TIMER0_TIME_MIN (uint32_t)(TIMER0_TIME_SEC * 60UL)

/* tid mellan timer-avbrott beroende på prescaler*/
#define TIME_BETWEEN_INTERRUPTS_MS 0.128

/* Timer 0 uträkning, kan användas istället för inlinefunktion */
#define TIMER0_COUNTER_MAX_MIN (uint32_t)(TIMER0_TIME_MIN / TIME_BETWEEN_INTERRUPTS_MS)
#define TIMER0_COUNTER_MAX_SEC (uint32_t)(TIMER0_TIME_SEC / TIME_BETWEEN_INTERRUPTS_MS + 0.5)
#define TIMER0_COUNTER_MAX (uint32_t)(TIMER0_TIME_MS / TIME_BETWEEN_INTERRUPTS_MS + 0.5)

bool button_pressed_enabled;

/* en inline funktion som förenklar tidsintervall uträkning som sensorn skall skriva ut i terminalen */
static inline uint32_t timer_get_max_count_sec(uint32_t time_sec)
{
	return(uint32_t)(time_sec * 1000/ TIME_BETWEEN_INTERRUPTS_MS + 0.5);
}

/* samma som ovan fast millisekunder */
static inline uint32_t timer_get_max_count_ms(uint32_t time_sec)
{
	return(uint32_t)(time_sec/ TIME_BETWEEN_INTERRUPTS_MS + 0.5);
}


void setup(void);

#endif /* HEADER_H_ */