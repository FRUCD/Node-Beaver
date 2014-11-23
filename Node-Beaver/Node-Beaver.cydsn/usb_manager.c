#include "usb_manager.h"



uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};
DataPacket usb_queue[USB_QUEUE_LENGTH];
uint8_t usb_pos = 0;



void usb_init()
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
  USBUART_1_GetConfiguration();
  USBUART_1_CDC_Init(); // USB Initialization
} // usb_init()



void usb_put(const DataPacket* data_queue, uint8_t data_pos)
{
	uint8_t i;
	uint32_t num_char;
	char buffer[128];

  if(USBUART_1_GetConfiguration() == 0)
		return;
  USBUART_1_CDC_Init(); // USB Initialization
	
  //if(USBUART_1_CDCIsReady())
	//{
	LCD_Char_1_Position(1,6);
	LCD_Char_1_PrintNumber(data_pos);
		
		for(i = 0; i < data_pos; i++)
		{
	LCD_Char_1_Position(1,9);
	LCD_Char_1_PrintNumber(data_pos);
			if(data_queue[i].type == TYPE_THROTTLE)
				num_char = sprintf(buffer,
					"Type: %X\tID: %X\tValue: %d\r\n",
					data_queue[i].type,
					data_queue[i].id,
					(short)data_queue[i].value);
			else
				num_char = sprintf(buffer,
					"Type: %X\tID: %X\tValue: %08lX %08lX\r\n",
					data_queue[i].type,
					data_queue[i].id,
					(unsigned long)((data_queue[i].value & 0xFFFFFFFF00000000) >> 32),
					(unsigned long)data_queue[i].value);	
			
	while(USBUART_1_CDCIsReady() == 0);
			USBUART_1_PutData((uint8_t*)buffer, num_char);
		} // for all messages in data queue
	//} // if ready to send USB

	while(USBUART_1_CDCIsReady() == 0);
  //if(USBUART_1_CDCIsReady())
		USBUART_1_PutData(test_usb_message, 8);
		
} // usb_send()
