#include "telegraph_key.h"

#define USB_DELAY 150
#define DEBOUNCE_DELAY 200
#define TIM2_PRESCALER 8400
#define TIM2_PERIOD 20000

static void init_clock(void);
static void init_button(void);
static void init_leds(void);
static void init_tim2(void);

void turn_on_leds_when_device_connected(void);
void turn_off_leds_when_device_not_connected(void);

volatile uint8_t count_of_pressing;
volatile uint16_t button_delay;
morse_symbol_t current_symbol;

int main(void)
{
    count_of_pressing = 0;
    morse_init_symbol(&current_symbol);

    init_clock();
    init_button();
    init_leds();
    init_tim2();

    delay_init();
    usb_init(&turn_on_leds_when_device_connected, &turn_off_leds_when_device_not_connected);

    while (1) {}

    return 0;
}

static void init_clock(void)
{
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_OTGFS);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_SYSCFG);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(rcc_ahb_frequency / 1000);
    systick_counter_enable();
    systick_interrupt_enable();

}

static void init_button(void)
{
    nvic_enable_irq(NVIC_EXTI0_IRQ);

    gpio_mode_setup(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO0);

    exti_select_source(EXTI0, GPIOE);
    exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
    exti_enable_request(EXTI0);
}

static void init_leds(void)
{
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);
    gpio_set(GPIOD, GPIO15);
}

static void init_tim2(void)
{
    rcc_periph_reset_pulse(RST_TIM2);

    timer_set_prescaler(TIM2, TIM2_PRESCALER /* / 2 */ - 1);
    timer_set_period(TIM2, TIM2_PERIOD - 1);

    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_enable_update_event(TIM2);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}

void tim2_isr(void)
{
    if (timer_get_flag(TIM2, TIM_SR_UIF))
    {
        timer_clear_flag(TIM2, TIM_SR_UIF);

        if (usb_get_status() == USB_DEVICE_STATUS_DISABLED)
        {
            return;
        }

        int old_count_of_pressing = count_of_pressing;
        count_of_pressing = 0;

        switch (old_count_of_pressing) {
            case 0:
                morse_set_char(&current_symbol);

                gpio_set(GPIOD, LED_ORANGE);
                usb_send_data(current_symbol.symbol_code);
                delay(USB_DELAY);
                usb_release_key();
                gpio_clear(GPIOD, LED_ORANGE);

                morse_init_symbol(&current_symbol);
                break;
            case 1:
                morse_add_dot(&current_symbol);
                break;
            case 3:
                morse_add_dash(&current_symbol);
                break;
        }
        gpio_toggle(GPIOD, LED_BLUE);
    }
}

void turn_on_leds_when_device_connected(void)
{
    gpio_set(GPIOD, LED_GREEN);
    gpio_clear(GPIOD, LED_RED);
}

void turn_off_leds_when_device_not_connected(void)
{
    gpio_set(GPIOD, LED_RED);
    gpio_clear(GPIOD, LED_GREEN);
    gpio_clear(GPIOD, LED_BLUE);
    gpio_clear(GPIOD, LED_ORANGE);
}

void exti0_isr(void)
{
    if (exti_get_flag_status(EXTI0))
    {
        if (button_delay > DEBOUNCE_DELAY) {
            button_delay = 0;

            count_of_pressing++;
        }
    }
    exti_reset_request(EXTI0);
}

void sys_tick_handler(void)
{
    button_delay++;
}
