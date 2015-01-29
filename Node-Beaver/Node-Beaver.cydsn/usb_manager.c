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


#ifdef USB_DEBUGGING


	uint16_t pos;
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
				case TYPE_THROTTLE_1:
					num_char = sprintf(buffer,
						"Type: Throttle 1\tID: %X\tValue: %d\r\n",
						data_queue[pos].id,
						(short)data_queue[pos].value);
					break;
				case TYPE_UNKNOWN:
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


#else // USB_DEBUGGING


	uint16_t pos;
	uint16_t buff_end = 0, prebuff_end = 0; // points to next empty space
	uint8_t buffer[512];
	uint8_t prebuffer[128];

  if(USBUART_1_GetConfiguration())
	{
		USBUART_1_CDC_Init(); // USB Initialization

		for(pos=data_head; pos!=data_tail; pos=(pos+1)%DATA_QUEUE_LENGTH)
		{
			prebuff_end = 0;

			// LSB is Sent first
			prebuffer[prebuff_end++] = data_queue[pos].type;
/*
			prebuffer[prebuff_end++] = data_queue[pos].time & 0x000000FF;
			prebuffer[prebuff_end++] = (data_queue[pos].time & 0x0000FF00) >> 8;
			prebuffer[prebuff_end++] = (data_queue[pos].time & 0x00FF0000) >> 16;
			prebuffer[prebuff_end++] = (data_queue[pos].time & 0xFF000000) >> 24;
*/
			prebuffer[prebuff_end++] = data_queue[pos].id & 0x00FF;
			prebuffer[prebuff_end++] = data_queue[pos].id & (0xFF00) >> 8;

			usb_prebuff_val(&data_queue[pos], prebuffer, &prebuff_end);
			

			usb_pack(buffer, &buff_end, prebuffer, &prebuff_end);

			while(USBUART_1_CDCIsReady() == 0);
			USBUART_1_PutData(buffer, buff_end);
		} // for all messages in data queue

	}	// if configuration successful


#endif // else USB_DEBUGGING


} // usb_send()



void usb_prebuff_val(const DataPacket* datum, uint8_t* prebuffer,
	uint16_t* prebuff_end)
{
	uint64_t value = datum->value;
	switch(datum->type)
	{
		case TYPE_UNKNOWN: case TYPE_THROTTLE_1:
			// 8 bytes
			prebuffer[(*prebuff_end)++] = value & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			prebuffer[(*prebuff_end)++] = (value <<=8) & 0x00000000000000FF;
			break;
	} // switch datum->type
} // usb_prebuff_val()



void usb_pack(uint8_t* buffer, uint16_t* buff_end, uint8_t* prebuffer,
	uint16_t* prebuff_end)
{
	uint16_t pre_ind;
	uint8_t upper, lower;

	*buff_end = 0;
	buffer[(*buff_end)++] = 0x7E; // Start byte

	for(pre_ind = 0; pre_ind < *prebuff_end; pre_ind++)
	{
		ham_byte(prebuffer[pre_ind], &upper, &lower);

		if(lower == 0x7E || lower == 0x7D)
		{
			buffer[(*buff_end)++] = 0x7D;
			buffer[(*buff_end)++] = lower ^ 0x20; // XOR 0x20 Escaped byte
		} // if need to escape lower
		else
			buffer[(*buff_end)++] = lower;

		if(upper == 0x7E || upper == 0x7D)
		{
			buffer[(*buff_end)++] = 0x7D;
			buffer[(*buff_end)++] = upper ^ 0x20; // XOR 0x20 Escaped byte
		} // if need to escape upper
		else
			buffer[(*buff_end)++] = upper;
	} // for all bytes in message
} // usb_hamming()



void usb_get()
{
	uint8_t count;
	uint8_t rx_buffer[64];

	if(USBUART_1_DataIsReady())
	{   
		count = USBUART_1_GetAll(rx_buffer); // maximum of 64 bits only
		if(count)
		{
			LED_Write(1);
			//process user data
		} // data exists
		for(count=0;count<255;count++);
		LED_Write(0);
	} // if data is to be received
} // usb_get()
