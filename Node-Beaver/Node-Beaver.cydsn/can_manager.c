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

	if(state != CYRET_SUCCESS)
	{
		//LED_Write(1);
		return CAN_1_FAIL;
	}

	//LED_Write(0);

	return CYRET_SUCCESS;
} // can_test_send()



uint8_t can_get(DataPacket* data_queue, uint8_t* data_pos)
{
	uint8_t index;
	*data_pos = 0;

	uint8_t atomic_state = CyEnterCriticalSection(); // begin ATOMIC
	while(can_head != can_tail) // move and convert can message queue to data queue
	{
		data_queue[*data_pos].id = can_queue[can_head].id; // CAN ID
		data_queue[*data_pos].value = 0; // wipe value

		switch(can_queue[can_head].id)
		{
			case CAN_THROTTLE:
				process_throttle(&data_queue[*data_pos]);
				break;
			default: // if CAN message unrecognized, value is the concatenated payload
				data_queue[*data_pos].type = TYPE_UNKNOWN;

				for(index = 0; index < can_queue[can_head].length; index++)
				{
					data_queue[*data_pos].value <<= 8; // Byte 0 is left-most
					data_queue[*data_pos].value |= can_queue[can_head].data[index];
				} // for all bytes in message
		}; // switch can id

		(*data_pos)++; // note: data_pos is also the size at the end of loop
		can_head = (can_head + 1) % CAN_QUEUE_LENGTH;
	} // for all can messages in queue

	CyExitCriticalSection(atomic_state); // end ATOMIC
	return 0;
} // can_receive()



void process_throttle(DataPacket* data_queue)
{
	data_queue->type = TYPE_THROTTLE;
	data_queue->value = can_queue[can_head].data[1]; // upper
	data_queue->value <<= 8;
	data_queue->value |= can_queue[can_head].data[0]; // lower
} // process_throttle()
