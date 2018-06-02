#include "delay.h"

void delay_init(void)
{
    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, TIM6_PRESCALER - 1);
    timer_one_shot_mode(TIM6);
}

void delay(uint16_t ms)
{
    timer_set_period(TIM6, ms);
    timer_generate_event(TIM6, TIM_EGR_UG);
    timer_enable_counter(TIM6);

    while (TIM_CR1(TIM6) & TIM_CR1_CEN); 
}