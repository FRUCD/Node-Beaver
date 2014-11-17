#include "usb_manager.h"



uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};



void usb_init()
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
  USBUART_1_CDC_Init(); // USB Initialization
  USBUART_1_GetConfiguration();
} // usb_init()



void usb_put(const DataPacket* data_queue, uint8_t data_pos)
{
  USBUART_1_GetConfiguration();
  if(USBUART_1_CDCIsReady())
	  USBUART_1_PutData(test_usb_message, 8);
} // usb_send()
