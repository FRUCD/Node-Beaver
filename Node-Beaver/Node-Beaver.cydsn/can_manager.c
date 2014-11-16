#include "can_manager.h"



const uint8_t test_message[] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
volatile uint8_t RX_DATA[8];
volatile uint8_t RX_ID;
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

	message.id = 0x001;
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 0x08;
	message.irq = 1;
	message.msg = &payload;

	for(i=0;i<8;i++)
		payload.byte[i] = test_message[i];

	state = CAN_1_SendMsg(&message);

	if(state != CYRET_SUCCESS) /*LED ON if message is sent succesfully*/
		return CAN_1_FAIL;

	LCD_Char_1_Position(0,1);
	LCD_Char_1_PrintString("CAN:TX");

	LCD_Char_1_Position(1,0);
	LCD_Char_1_PrintNumber(RX_DATA[0]);

	if(state == CYRET_SUCCESS) /*LED ON if message is sent succesfully*/
		LED_Write(1);
	else
		LED_Write(0);

	//CyDelay(500);
	//LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	return CYRET_SUCCESS;
} // can_test_send()



uint8_t can_get(DataPacket* data_queue, uint8_t data_pos)
{
	uint8_t atomic_state = CyEnterCriticalSection();
	can_tail = data_pos;
	// move and convert can message queue to data queue
	can_head = can_tail = 0;
	CyExitCriticalSection(atomic_state);
	return 0;
} // can_receive()
