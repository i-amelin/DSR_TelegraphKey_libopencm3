#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include <stdlib.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/usb/dwc/otg_fs.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>

typedef enum usb_device_status
{
    USB_DEVICE_STATUS_ENABLED,
    USB_DEVICE_STATUS_DISABLED
} usb_device_status_t;

void usb_init(void (*turn_on_func_callback)(void), void (*turn_off_func_callback)(void));
void usb_send_data(uint8_t symbol_code);
void usb_release_key(void);
usb_device_status_t usb_get_status(void);

#endif /* USB_DEVICE_H */
