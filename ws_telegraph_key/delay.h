#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#define TIM6_PRESCALER 24000

void delay_init(void);
void delay(uint16_t ms);

#endif /* DELAY_H */