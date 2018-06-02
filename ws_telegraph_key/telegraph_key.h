#ifndef TELEGRAPH_KEY
#define TELEGRAPH_KEY

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "delay.h"
#include "morse.h"
#include "usb_device.h"

#define LED_GREEN  GPIO12
#define LED_ORANGE GPIO13
#define LED_RED    GPIO14
#define LED_BLUE   GPIO15

#endif /* TELEGRAPH_KEY */
