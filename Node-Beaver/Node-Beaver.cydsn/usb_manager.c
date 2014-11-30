#include "usb_manager.h"

extern uint16_t can_head, can_tail; // for probing length



uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};
//DataPacket usb_queue[USB_QUEUE_LENGTH];
//uint8_t usb_pos = 0;



void usb_init()
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
} // usb_init()



void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
	uint8_t pos;
	uint32_t num_char;
	char buffer[128];
	int length_data, length_can; // Testing
	length_data = length_can = 0; // Testing

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

		// Print lengths
		length_data = (int)data_tail - (int)data_head;
		if(length_data < 0)
			length_data = (int)data_tail + (int)DATA_QUEUE_LENGTH - (int)data_head;

		length_can = (int)can_tail - (int)can_head;
		if(length_can < 0)
			length_can = (int)can_tail + (int)CAN_QUEUE_LENGTH - (int)can_head;

		num_char = sprintf(buffer, "DATA: %d/%d, CAN: %d/%d\r\n",
			length_data, DATA_QUEUE_LENGTH, length_can, CAN_QUEUE_LENGTH);

		while(USBUART_1_CDCIsReady() == 0);
		USBUART_1_PutData((uint8_t*)buffer, num_char);
		// End print lengths

		
		while(USBUART_1_CDCIsReady() == 0);
		USBUART_1_PutData(test_usb_message, 8); // Test message "PSoC!!!"
	}	// if configuration successful
} // usb_send()
