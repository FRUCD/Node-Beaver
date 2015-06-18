#include "usb_manager.h"
#include "can_manager.h"
#include <stdbool.h>

uint8_t test_usb_message[8] = {'P','S','o','C','!','!','\r','\n'};
static uint16_t* CANID_white_list = NULL;
static uint8_t CANID_length = 0;
static uint16_t CANID_list_default[] =
{
	0x000, // NMT message
	0x003, // Current Data
	0x0A6, // Emergency
	0x111, // Logger test message
	0x181, // TPDO1 Current Control
	0x188, // Pack Status
	0x1A6, // PDO1-MISO
	0x226, // PDO1-MOSI
	0x281, // TPDO5 Motor and Throttle
	0x2A6, // PDO2-MISO
	0x326, // PDO2-MOSI
	0x381, // TPDO4 Sensors
	0x388, // Voltage Data
	0x481, // TPDO2 Controls
	0x488, // Temperature
	0x501, // TPDO3 Operation Params
	0x5A6, // SDO-MISO
	0x626, // SDO-MOSI
	0x726  // Heartbeat
};
static uint8_t CANID_length_default = 19;


extern uint16_t can_head, can_tail;


/* usb_init()
	Takes nothing. Returns nothing.

	Initializes USB UART for the device 0. Disables voltage regulator and
	pass-through Vcc for pull-up.
*/
void usb_init(void)
{
	USBUART_1_Start(0, USBUART_1_3V_OPERATION);
	CANID_white_list = CANID_list_default;
	CANID_length = CANID_length_default;
} // usb_init()


void usb_filterResult(uint16_t CANIDs[], uint8_t length)
{
	CANID_white_list = CANIDs;
	CANID_length = length;
}

/* usb_put()
	Takes a DataPacket queue (data_queue) with a head and a tail index to the
	queue.
	Returns nothing.

	Checks if the USB exists and configured. If USB is configured, print each
	element of data_queue as a line on USB UART terminal.

	Terminal printing format:
	[timestamp in dec.]ms	0x[CAN ID in hex.]	Value [CAN message in hex.]
*/
void usb_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
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
			bool shouldDisplayMessage = false;
			uint8_t i = 0;
			for (i = 0; i < CANID_length; i++)
			{
				if (data_queue[pos].id == CANID_white_list[i])
				{
					shouldDisplayMessage = true;
					break;
				}
			}
			if (!shouldDisplayMessage)
			{
				continue;
			}

			num_char = sprintf(buffer,
				"%ums\t0x%03X\t"
				"Value %02X %02X %02X %02X  %02X %02X %02X %02X \r\n",
				(unsigned)data_queue[pos].millicounter, data_queue[pos].id,
				data_queue[pos].data[0],
				data_queue[pos].data[1],
				data_queue[pos].data[2],
				data_queue[pos].data[3],
				data_queue[pos].data[4],
				data_queue[pos].data[5],
				data_queue[pos].data[6],
				data_queue[pos].data[7]);

			while(USBUART_1_CDCIsReady() == 0);
			USBUART_1_PutData((uint8_t*)buffer, num_char);
		} // for all messages in data queue

		// Print lengths
		length_data = (int)data_tail - (int)data_head;
		if(length_data < 0)
			length_data = (int)data_tail + (int)DATA_QUEUE_LENGTH - (int)data_head;

/*
		length_can = (int)can_tail - (int)can_head;
		if(length_can < 0)
			length_can = (int)can_tail + (int)CAN_QUEUE_LENGTH - (int)can_head;

		num_char = sprintf(buffer, "DATA: %d/%d, CAN: %d/%d\r\n",
			length_data, DATA_QUEUE_LENGTH, length_can, CAN_QUEUE_LENGTH);
*/
		num_char = sprintf(buffer, "DATA QUEUE: %d/%d\r\n",
			length_data, DATA_QUEUE_LENGTH);

		while(USBUART_1_CDCIsReady() == 0);
		USBUART_1_PutData((uint8_t*)buffer, num_char);
		// End print lengths
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
/*
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
*/


/*
void usb_get()
{
	uint8_t count;
	uint8_t rx_buffer[64];

	if(USBUART_1_DataIsReady())
	{   
		count = USBUART_1_GetAll(rx_buffer); // maximum of 64 bits only
		if(count)
		{
			//LED_Write(1);
			//process user data
		} // data exists
		for(count=0;count<255;count++);
		//LED_Write(0);
	} // if data is to be received
} // usb_get()
*/
