#include "usb_manager.h"



void usb_init()
{
  USBUART_1_CDC_Init(); // USB Initialization
} // usb_init()



void usb_put(const DataPacket* data_queue, const uint8_t data_pos)
{

} // usb_send()
