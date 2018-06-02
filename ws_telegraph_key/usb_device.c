#include "usb_device.h"

static usbd_device *usbd_dev;
static void (*device_connect_callback)(void);
static void (*device_disconnect_callback)(void);

usb_device_status_t usb_device_current_status;

static const struct usb_device_descriptor dev_descr =
{
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = USB_CLASS_HID,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 64,
    .idVendor = 0x0483,
    .idProduct = 0x5710,
    .bcdDevice = 0x0200,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1,
};

static const uint8_t hid_report_descriptor[] =
{
    /*
    0x05, 0x01,                  // USAGE_PAGE (Generic Desktop)
0x09, 0x06,                 // USAGE (Keyboard)
0xa1, 0x01,                 // COLLECTION (Application)
0x05, 0x07,                 // USAGE_PAGE (Key Codes)
0x09, 0x4c,                 // USAGE (Del Key)
0x09, 0x69,                 // USAGE (F14 Key)
0x09, 0x6a,                 // USAGE (F15 Key)
0x09, 0xe0,                 // USAGE (Left Ctrl Key)
0x09, 0xe2,                 // USAGE (Left Alt Key)
0x09, 0xe3,                 // USAGE (Left Windows Key)
0x15, 0x00,                 // LOGICAL_MINIMUM (0)
0x25, 0x01,                 // LOGICAL_MAXIMUM (1)
0x75, 0x01,                 // REPORT_SIZE (1)
0x95, 0x06,                 // REPORT_COUNT (5)
0x81, 0x02,                 // INPUT (Data, Var, Abs)
0x95, 0x1a,                 // REPORT_COUNT (27)
0x81, 0x03,                 // INPUT (Cnst, Var, Abs)
0xc0,                          // END_COLLECTION

0x05, 0x0C,          // USAGE_PAGE (Consumer devices)
0x09, 0x01,          // USAGE (Consumer Control)
0xa1, 0x01,          // COLLECTION (Application)
0x09, 0xe9,              // USAGE (Volume up)
0x09, 0xea,              // USAGE (Volume down)
0x15, 0x00,          // LOGICAL_MINIMUM (0)
0x25, 0x01,          // LOGICAL_MAXIMUM (1)
0x75, 0x01,              // REPORT_SIZE (1)
0x95, 0x02,              // REPORT_COUNT (2)
0x81, 0x02,              // INPUT (Data, Var, Abs)
0x95, 0x1e,              // REPORT_COUNT (30)
0x81, 0x03,          // INPUT (Cnst, Var, Abs)
0xc0,                   // END_COLLECTION

0x05, 0x01,             // USAGE_PAGE (Generic Desktop)
0x09, 0x80,             // USAGE (System Control)
0xa1, 0x01,             // COLLECTION (Application)
0x09, 0x81,             // USAGE (System power down)
0x09, 0x83,             // USAGE (System wake up)
0x15, 0x00,             // LOGICAL_MINIMUM (0)
0x25, 0x01,             // LOGICAL_MAXIMUM (1)
0x75, 0x01,             // REPORT_SIZE (1)
0x95, 0x02,             // REPORT_COUNT (2)
0x81, 0x02,             // INPUT (Data,Var,Abs)
0x95, 0x1e,             // REPORT_COUNT (30)
0x81, 0x03,             // INPUT (Cnst,Var,Abs)
0xc0,           // END_COLLECTION
*/

    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x05, 0x07,   // USAGE_PAGE (Keyboard)
    0x19, 0xe0,   // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,   // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,   // LOGICAL_MINIMUM (0)
    0x25, 0x01,   // LOGICAL_MAXIMUM (1)
    0x75, 0x01,   // REPORT_SIZE (1)
    0x95, 0x08,   // REPORT_COUNT (8)
    0x81, 0x02,   // INPUT (Data,Var,Abs) //1 byte
     
    0x95, 0x01,   // REPORT_COUNT (1)
    0x75, 0x08,   // REPORT_SIZE (8)
    0x81, 0x01,   // INPUT (Cnst,Var,Abs) //1 byte
     
    0x95, 0x06,   // REPORT_COUNT (6)
    0x75, 0x08,   // REPORT_SIZE (8)
    0x15, 0x00,   // LOGICAL_MINIMUM (0)
    0x25, 0x65,   // LOGICAL_MAXIMUM (101)
    0x05, 0x07,   // USAGE_PAGE (Keyboard)
    0x19, 0x00,   // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,   // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,   // INPUT (Data,Ary,Abs) //6 bytes

    0x09, 0x05,         // Usage (Vendor Defined)
    0x15, 0x00,         // Logical Minimum (0)
    0x26, 0xff, 0x00,   // Logical Maximum (255)
    0x75, 0x08,         // Report Count (2)
    0x95, 0x02,         // Report Size (8)
    0xb1, 0x02,         // Feature (Data, Variable, Absolute)

    0x95, 0x05,   //     Report Count (5)
    0x75, 0x01,   //     Report Size (1)
    0x05, 0x08,   //     Usage Page (Page# for LEDs)
    0x19, 0x01,   //     Usage Minimum (1)
    0x29, 0x05,   //     Usage Maximum (5)
    0x91, 0x02,   //     Output (Data, Variable, Absolute), Led report
    0x95, 0x01,   //     Report Count (1)
    0x75, 0x03,   //     Report Size (3)
    0x91, 0x01,   //     Output (Data, Variable, Absolute), Led report padding
     
    0xc0 // END_COLLECTION
/*
    
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)         
    0x09, 0x02, // USAGE (Mouse)                        
    0xa1, 0x01, // COLLECTION (Application)             
    0x09, 0x01, //   USAGE (Pointer)                    
    0xa1, 0x00, //   COLLECTION (Physical)              
    0x05, 0x09, //     USAGE_PAGE (Button)              
    0x19, 0x01, //     USAGE_MINIMUM (Button 1)         
    0x29, 0x03, //     USAGE_MAXIMUM (Button 3)         
    0x15, 0x00, //     LOGICAL_MINIMUM (0)              
    0x25, 0x01, //     LOGICAL_MAXIMUM (1)              
    0x95, 0x03, //     REPORT_COUNT (3)                 
    0x75, 0x01, //     REPORT_SIZE (1)                  
    0x81, 0x02, //     INPUT (Data,Var,Abs)             
    0x95, 0x01, //     REPORT_COUNT (1)                 
    0x75, 0x05, //     REPORT_SIZE (5)                  
    0x81, 0x01, //     INPUT (Cnst,Ary,Abs)             
    0x05, 0x01, //     USAGE_PAGE (Generic Desktop)     
    0x09, 0x30, //     USAGE (X)                        
    0x09, 0x31, //     USAGE (Y)                        
    0x09, 0x38, //     USAGE (Wheel)                    
    0x15, 0x81, //     LOGICAL_MINIMUM (-127)           
    0x25, 0x7f, //     LOGICAL_MAXIMUM (127)            
    0x75, 0x08, //     REPORT_SIZE (8)                  
    0x95, 0x03, //     REPORT_COUNT (3)                 
    0x81, 0x06, //     INPUT (Data,Var,Rel)             
    0xc0,       //   END_COLLECTION                     
    
    0x09, 0x3c, //   USAGE (Motion Wakeup)              
    0x05, 0xff, //   USAGE_PAGE (Vendor Defined Page 1) 
    0x09, 0x01, //   USAGE (Vendor Usage 1)             
    0x15, 0x00, //   LOGICAL_MINIMUM (0)                
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)                
    0x75, 0x01, //   REPORT_SIZE (1)                    
    0x95, 0x02, //   REPORT_COUNT (2)                   
    0xb1, 0x22, //   FEATURE (Data,Var,Abs,NPrf)        
    0x75, 0x06, //   REPORT_SIZE (6)                    
    0x95, 0x01, //   REPORT_COUNT (1)                   
    0xb1, 0x01, //   FEATURE (Cnst,Ary,Abs)
    0xc0        // END_COLLECTION
    */
};

static const struct
{
    struct usb_hid_descriptor hid_descriptor;
    struct
    {
        uint8_t bReportDescriptorType;
        uint16_t wDescriptorLength;
    } __attribute__((packed)) hid_report;
}
__attribute__((packed)) hid_function = 
{
    .hid_descriptor =
    {
        .bLength = sizeof(hid_function),
        .bDescriptorType = USB_DT_HID,
        .bcdHID = 0x0111,
        .bCountryCode = 0,
        .bNumDescriptors = 1,
    },
    .hid_report =
    {
        .bReportDescriptorType = USB_DT_REPORT,
        .wDescriptorLength = sizeof(hid_report_descriptor),
    }
};

static const struct usb_endpoint_descriptor hid_endpoint =
{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x81,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 8,
    .bInterval = 32,

};

static const struct usb_interface_descriptor hid_iface =
{
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 1,
    .bInterfaceClass = USB_CLASS_HID,
    .bInterfaceSubClass = 0, /* boot */
    .bInterfaceProtocol = 1, /* keyboard */
    .iInterface = 0,

    .endpoint = &hid_endpoint,

    .extra = &hid_function,
    .extralen = sizeof(hid_function),
};

static const struct usb_interface ifaces[] =
{
    {
        .num_altsetting = 1,
        .altsetting = &hid_iface
    }
};

static const struct usb_config_descriptor config =
{
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0xC0,
    .bMaxPower = 0x32,

    .interface = ifaces,
};

static const char *usb_strings[] =
{
    "DSR Corporation",
    "STM32 Telegraph Key",
    "DEMO",
};

/* Buffer used for control requests. */
uint8_t usbd_control_buffer[128];

static enum usbd_request_return_codes hid_control_request(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
            void (**complete)(usbd_device *dev, struct usb_setup_data *req))
{
    (void)complete;
    (void)dev;

    if((req->bmRequestType != 0x81) ||
       (req->bRequest != USB_REQ_GET_DESCRIPTOR) ||
       (req->wValue != 0x2200))
    {
        return USBD_REQ_NOTSUPP;
    }

    *buf = (uint8_t *)hid_report_descriptor;
    *len = sizeof(hid_report_descriptor);
    return USBD_REQ_HANDLED;
}

static void hid_set_config(usbd_device *dev, uint16_t wValue)
{

    (void)wValue;

    usbd_ep_setup(dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 8, NULL);

    usbd_register_control_callback(
        dev,
        USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
        USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
        hid_control_request);

    usb_device_current_status = USB_DEVICE_STATUS_ENABLED;
    device_connect_callback();
}

static void hid_reset(void)
{
    usb_device_current_status = USB_DEVICE_STATUS_DISABLED;
    device_disconnect_callback();
}

static void hid_suspend(void)
{
    usb_device_current_status = USB_DEVICE_STATUS_DISABLED;
    device_disconnect_callback();
}

static void hid_resume(void)
{
    usb_device_current_status = USB_DEVICE_STATUS_DISABLED;
    device_disconnect_callback();
}

void usb_init(void (*turn_on_func_callback)(void), void (*turn_off_func_callback)(void))
{
    usb_device_current_status = USB_DEVICE_STATUS_DISABLED;

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO11 | GPIO12);
    gpio_set_af(GPIOA, GPIO_AF10, GPIO9 | GPIO11 | GPIO12);

    usbd_dev = usbd_init(
        &otgfs_usb_driver,
        &dev_descr,
        &config,
        usb_strings,
        3,
        usbd_control_buffer,
        sizeof(usbd_control_buffer));

    device_connect_callback = turn_on_func_callback;
    device_disconnect_callback = turn_off_func_callback;
    device_disconnect_callback();

    usbd_register_set_config_callback(usbd_dev, hid_set_config);
    usbd_register_reset_callback(usbd_dev, hid_reset);
    usbd_register_suspend_callback(usbd_dev, hid_suspend);
    usbd_register_resume_callback(usbd_dev, hid_resume);
    
    nvic_enable_irq(NVIC_OTG_FS_IRQ);
}

void usb_send_data(uint8_t symbol_code)
{
    uint8_t buf[8] = {0, 0, symbol_code, 0, 0, 0, 0, 0};
    usbd_ep_write_packet(usbd_dev, 0x81, buf, 8);
}

void usb_release_key(void)
{
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    usbd_ep_write_packet(usbd_dev, 0x81, buf, 8);
}

usb_device_status_t usb_get_status(void)
{
    return usb_device_current_status;
}

void otg_fs_isr(void)
{
    if (usbd_dev) {
        usbd_poll(usbd_dev);
    }
}
