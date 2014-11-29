#include "usb_manager.h"



uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};
DataPacket usb_queue[USB_QUEUE_LENGTH];
uint8_t usb_pos = 0;



void usb_init()
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
} // usb_init()



void usb_put(const DataPacket* data_queue, uint8_t data_head,
	uint8_t data_tail)
{
	uint8_t pos;
	uint32_t num_char;
	char buffer[128];

  if(USBUART_1_GetConfiguration())
	{
		USBUART_1_CDC_Init(); // USB Initialization

		for(pos=data_head; pos!=data_tail; pos=(pos+1)%DATA_QUEUE_LENGTH)
		{
			switch(data_queue[pos].type)
			{
				case TYPE_THROTTLE:
					num_char = sprintf(buffer,
						"Type: Throttle\tID: %X\tValue: %d\r\n",
						data_queue[pos].id,
						(short)data_queue[pos].value);
					break;
				case TYPE_UNKNOWN:
				/*
					num_char = sprintf(buffer,
						"Type: Unknown\tID: %X\tValue: %08lX %08lX\r\n",
						data_queue[pos].id,
						(unsigned long)((data_queue[pos].value & 0xFFFFFFFF00000000) >> 32),
						(unsigned long)data_queue[pos].value);	
					*/
					num_char = sprintf(buffer,
						"Type: Unknown\tID: %X\t"
						"Value: %02X %02X %02X %02X  %02X %02X %02X %02X \r\n",
						data_queue[pos].id,
						(unsigned)((data_queue[pos].value & 0xFF00000000000000) >> 56),
						(unsigned)((data_queue[pos].value & 0x00FF000000000000) >> 48),
						(unsigned)((data_queue[pos].value & 0x0000FF0000000000) >> 40),
						(unsigned)((data_queue[pos].value & 0x000000FF00000000) >> 32),
						(unsigned)((data_queue[pos].value & 0x00000000FF000000) >> 24),
						(unsigned)((data_queue[pos].value & 0x0000000000FF0000) >> 16),
						(unsigned)((data_queue[pos].value & 0x000000000000FF00) >> 8),
						(unsigned)(data_queue[pos].value & 0x00000000000000FF));
					break;
				default:
					num_char = sprintf(buffer, "ERROR: Invalid data in queue!\r\n");
			}; // switch data ID

			while(USBUART_1_CDCIsReady() == 0);
			USBUART_1_PutData((uint8_t*)buffer, num_char);
		} // for all messages in data queue

		while(USBUART_1_CDCIsReady() == 0);
		USBUART_1_PutData(test_usb_message, 8); // Test message
	}	// if configuration successful
} // usb_send()
