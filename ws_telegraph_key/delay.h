/*!
  \file
  \brief Delay Header File
 
  Contains functions for delay.
*/

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/*! Prescaler for TIM6 Timer */
#define TIM6_PRESCALER 24000

/*! Initializes the TIM6 timer used to delay. */
void delay_init(void);

/*!
  Stops the running of code. 
  \param ms Number of milliseconds to delay
*/
void delay(uint16_t ms);

#endif /* DELAY_H */