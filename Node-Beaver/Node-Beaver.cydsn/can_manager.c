#include "can_manager.h"



const uint8_t test_message[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
volatile CanMessage can_queue[CAN_QUEUE_LENGTH];
volatile uint8_t can_head = 0;
volatile uint8_t can_tail = 0;



void can_init()
{
	CAN_1_GlobalIntEnable(); // CAN Initialization
	CAN_1_Init();
	CAN_1_Start();
} // can_init()




uint8_t can_test_send()
{
	uint8_t i, state;
	CAN_1_TX_MSG message;
	CAN_1_DATA_BYTES_MSG payload;

	message.id = 0x001; // edit for testing
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 0x08;
	message.irq = 1;
	message.msg = &payload;

	for(i=0;i<8;i++)
		payload.byte[i] = test_message[i];

	state = CAN_1_SendMsg(&message);

/*
	if(state != CYRET_SUCCESS)
	{
		LED_Write(1);
		return CAN_1_FAIL;
	}

	LED_Write(0);
	return CYRET_SUCCESS;
*/
	
	return state;
} // can_test_send()


/* can_get()
	Takes data_queue, data_head, and data_tail.
	Returns nothing.

	Clears data_queue first. Goes through can_queue, extracts information
	according to their type, and enqueues data to data_queue. Wraps data_queue if
	it is full. Finally, clears can_queue. 

	Note that global variables can_queue, can_head, and can_tail are used.
*/
void can_get(DataPacket* data_queue, uint8_t* data_head, uint8_t* data_tail)
{
	*data_head = *data_tail = 0;

	uint8_t atomic_state = CyEnterCriticalSection(); // BEGIN ATOMIC

	while(can_head != can_tail) // move and convert can message queue to data queue
	{
		data_queue[*data_tail].id = can_queue[can_head].id; // CAN ID
		// remember that data_queue.value is unclean

		switch(can_queue[can_head].id)
		{
			case CAN_THROTTLE:
				process_throttle(data_queue, data_head, data_tail);
				break;
			default: // if CAN message unrecognized, value is the concatenated payload
				process_default(data_queue, data_head, data_tail);
		}; // switch can id

		can_head = (can_head + 1) % CAN_QUEUE_LENGTH;
	} // for all can messages in queue

	can_head = can_tail = 0;

	CyExitCriticalSection(atomic_state); // END ATOMIC
	//return 0;
} // can_receive()



void wrap_data_queue(uint8_t* data_head, uint8_t* data_tail)
{
	*data_tail = (*data_tail + 1) % DATA_QUEUE_LENGTH;

	if(*data_tail == *data_head) // if need to roll data queue
		*data_head = (*data_head + 1) % DATA_QUEUE_LENGTH;
} // wrap_data_queue()



inline void process_default(DataPacket* data_queue, uint8_t* data_head,
	uint8_t* data_tail)
{
	uint8_t index;
	data_queue[*data_tail].type = TYPE_UNKNOWN;
	data_queue[*data_tail].value = 0; // wipe value

	for(index = 0; index < can_queue[can_head].length; index++)
	{
		data_queue[*data_tail].value <<= 8; // Byte 0 is left-most
		data_queue[*data_tail].value |= can_queue[can_head].data[index];
	} // for all bytes in message

	wrap_data_queue(data_head, data_tail);
} // process_default()



inline void process_throttle(DataPacket* data_queue, uint8_t* data_head,
	uint8_t* data_tail)
{
	data_queue[*data_tail].type = TYPE_THROTTLE;
	data_queue[*data_tail].value = can_queue[can_head].data[1]; // upper
	data_queue[*data_tail].value <<= 8;
	data_queue[*data_tail].value |= can_queue[can_head].data[0]; // lower

	wrap_data_queue(data_head, data_tail);
} // process_throttle()
