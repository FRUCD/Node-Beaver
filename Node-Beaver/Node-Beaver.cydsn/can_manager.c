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
		LED_Write(1);
		return CAN_1_FAIL;
	}
			LED_Write(0);

	LCD_Char_1_Position(0,1);
	LCD_Char_1_PrintString("CAN:TX");

	LCD_Char_1_Position(1,0);
	LCD_Char_1_PrintNumber(can_queue[can_head].data[7]);

/*
	if(state == CYRET_SUCCESS) // test
		LED_Write(1);
	else
		LED_Write(0);
*/
	//CyDelay(500);
	//LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

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
				data_queue[*data_pos].type = 1;
				data_queue[*data_pos].value = can_queue[can_head].data[1]; // upper
				data_queue[*data_pos].value <<= 8;
				data_queue[*data_pos].value |= can_queue[can_head].data[0]; // lower
				break;

			default:
				data_queue[*data_pos].type = CAN_UNKNOWN;

				for(index = 0; index < can_queue[can_head].length; index++)
				{
					data_queue[*data_pos].value <<= 8;
					data_queue[*data_pos].value |= can_queue[can_head].data[index];
				} // for all bytes in message
				break;
		}; // switch can id

		(*data_pos)++; // data_pos is also the size at the end
		can_head = (can_head + 1) % CAN_QUEUE_LENGTH;
	} // for all can messages in queue

	CyExitCriticalSection(atomic_state); // end ATOMIC
	return 0;
} // can_receive()
