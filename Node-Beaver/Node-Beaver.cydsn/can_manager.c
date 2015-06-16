#include "can_manager.h"



volatile DataPacket can_queue[CAN_QUEUE_LENGTH];
volatile uint16_t can_head = 0, can_tail = 0;



/* can_get()
	Takes data_queue, data_head, and data_tail.
	Returns nothing.

	Clears data_queue first. Goes through can_queue, extracts information
	according to their type, and enqueues data to data_queue. Wraps data_queue if
	it is full. Finally, clears can_queue. 

	Note that global extern variables can_queue, can_head, and can_tail are used.
	Also, copies from volatile can_queue to non-volatile data_queue are atomic.


	can_queue is filled from a CAN interrupt function:

		CAN_1_ReceiveMsg()

	in line 632 of file

		"Generated\ Source/PSoC5/CAN_1_TX_RX_func.c"
*/
void can_get(DataPacket* data_queue, uint16_t* data_head, uint16_t* data_tail)
{
	*data_head = *data_tail = 0;

	uint8_t atomic_state = CyEnterCriticalSection(); // BEGIN ATOMIC

	while(can_head != can_tail) // move and convert can message queue to data queue
	{
		data_queue[*data_tail].id = can_queue[can_head].id;
		data_queue[*data_tail].length = can_queue[can_head].length;
		data_queue[*data_tail].millicounter = can_queue[can_head].millicounter;

		data_queue[*data_tail].data[0] = can_queue[can_head].data[0];
		data_queue[*data_tail].data[1] = can_queue[can_head].data[1];
		data_queue[*data_tail].data[2] = can_queue[can_head].data[2];
		data_queue[*data_tail].data[3] = can_queue[can_head].data[3];
		data_queue[*data_tail].data[4] = can_queue[can_head].data[4];
		data_queue[*data_tail].data[5] = can_queue[can_head].data[5];
		data_queue[*data_tail].data[6] = can_queue[can_head].data[6];
		data_queue[*data_tail].data[7] = can_queue[can_head].data[7];

		can_head = (can_head + 1) % CAN_QUEUE_LENGTH; // discard copied packet

		*data_tail = (*data_tail + 1) % DATA_QUEUE_LENGTH; // increment data tail

		if(*data_tail == *data_head) // if need to roll data queue
			*data_head = (*data_head + 1) % DATA_QUEUE_LENGTH;
	} // for all can messages in queue

	can_head = can_tail = 0;
	CyExitCriticalSection(atomic_state); // END ATOMIC
} // can_receive()



/* can_init()
	Takes nothing.
	Returns nothing.

	Initializes and start PSoC Can component
*/
void can_init(void)
{
	CAN_1_GlobalIntEnable(); // CAN Initialization
	CAN_1_Init();
	CAN_1_Start();
} // can_init()



/* can_test_send()
	Takes nothing.
	Returns Cypress' status number from CAN component

	Sends a test message to the CAN.
*/
/*
uint8_t can_test_send()
{
	const uint8_t test_message[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
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
	return state;
} // can_test_send()
*/
