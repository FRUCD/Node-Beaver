#include "usb_manager.h"

//uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};



/* usb_init()
	Takes nothing. Returns nothing.

	Initializes USB UART for the device 0. Disables voltage regulator and
	pass-through Vcc for pull-up.
*/
void usb_init()
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
} // usb_init()



/* usb_put()
	Takes a DataPacket queue (data-queue) with a head and a tail index to the
	queue.  Returns nothing.

	Checks is the USB exists and configured. If USB is configured, frame and send
	all packets in the DataPacket queue. Each byte is checked and escaped by
	usb_escape.
*/
void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
	uint16_t pos;
	uint16_t buff_end = 0; // points to next empty space
	uint8_t buffer[UART_BUFF_LENGTH];
	int8_t byte;

  if(USBUART_1_GetConfiguration())
	{
		USBUART_1_CDC_Init(); // USB Initialization

		for(pos=data_head; pos!=data_tail; pos=(pos+1)%DATA_QUEUE_LENGTH)
		{
			buff_end = 0;
			buffer[buff_end++] = START_B;	

			usb_escape(buffer, &buff_end, data_queue[pos].id >> 8);
			usb_escape(buffer, &buff_end, data_queue[pos].id);
			usb_escape(buffer, &buff_end, data_queue[pos].millicounter >> 16);
			usb_escape(buffer, &buff_end, data_queue[pos].millicounter >> 8);
			usb_escape(buffer, &buff_end, data_queue[pos].millicounter);

			for(byte = data_queue[pos].length; byte >= 0; byte--)
				usb_escape(buffer, &buff_end, data_queue[pos].data[byte]);

			while(USBUART_1_CDCIsReady() == 0);
			USBUART_1_PutData(buffer, buff_end);
		} // for all messages in data queue
	}	// if configuration successful
} // usb_send()



/* usb_escape()
	Takes a character buffer (buffer) with a pointer pointing to the end of it
	(buff_end) and a byte to add (byte).
	Returns nothing.

	If the byte needs to be escaped, it is prefixed with an escape character
	(ESCAP_B) and XORed with the XOR character (XOR_B).
	Else, the byte is simple added to the end of the buffer.
	The buffer is incremented to point to the first empty element in the buffer by
	the end of the function.
*/
void usb_escape(uint8_t* buffer, uint16_t* buff_end, uint8_t byte)
{
	if(byte == START_B || byte == ESCAP_B)
	{
		buffer[(*buff_end)++] = ESCAP_B;
		buffer[(*buff_end)++] = byte ^ XOR_B; // XOR 0x20 Escaped byte
	} // if need to escape byte
	else
		buffer[(*buff_end)++] = byte;
} // usb_escape()



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
