/*!
  \file
  \brief USB Device Header File
 
  Contains functions using for sending symbols to a computer.
*/

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

/*!
  STM device statuses.
*/
typedef enum usb_device_status
{
    USB_DEVICE_STATUS_ENABLED,
    USB_DEVICE_STATUS_DISABLED
} usb_device_status_t;

/*!
  Initializes the working with USB.
  \param turn_on_func_callback Function called when the device is plugged to a computer
  \param turn_off_func_callback Function called when the device is unplugged to a computer
*/
void usb_init(void (*turn_on_func_callback)(void), void (*turn_off_func_callback)(void));

/*!
  Sends the symbol's keyboard code to a computer.
  \param symbol_code Keyboard code of symbol
*/
void usb_send_data(uint8_t symbol_code);

/*!
  Sends a signal that no keys are pressed.
*/
void usb_release_key(void);

/*!
  Returns the current status of the STM device.
  \return Current status of the device
*/
usb_device_status_t usb_get_status(void);

#endif /* USB_DEVICE_H */
