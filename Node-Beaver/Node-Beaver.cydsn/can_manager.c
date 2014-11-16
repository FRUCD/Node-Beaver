#include "can_manager.h"



volatile uint8_t RX_DATA[8];
volatile uint8_t RX_ID;
const uint8_t test_message[]={0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};



uint8_t can_handler()
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

	return CYRET_SUCCESS;
} // can_handler()



uint8_t can_receive()
{
	return 0;
} // can_receive()
