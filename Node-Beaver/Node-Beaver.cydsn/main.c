/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
 */

#include <project.h>
#include <FS.h>


uint8_t RX_DATA[8];
uint8_t RX_ID;
uint8_t test_message[]={0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};


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

	for(i=0;i<8;i++) payload.byte[i] = test_message[i];
	message.msg = &payload;

	state = CAN_1_SendMsg(&message);

	if(state != CYRET_SUCCESS) /*LED ON if message is sent succesfully*/
		return CAN_1_FAIL;

	LCD_Char_1_Position(0,1);
	LCD_Char_1_PrintString("CAN:TX");
	return CYRET_SUCCESS;
} // can_handler()


int main()
{
	CYGlobalIntEnable;

	CAN_1_GlobalIntEnable();
	CAN_1_Init();
	CAN_1_Start();

	LCD_Char_1_Start();
	LED_Write(0);
	LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	uint8_t state;
	for(;;)
	{
		state = can_handler();
		//state = CAN_1_SendMsg0();

		if(state == CYRET_SUCCESS) /*LED ON if message is sent succesfully*/
			LED_Write(1);
		else
			LED_Write(0);

		//LCD_Char_1_ClearDisplay();
		LCD_Char_1_Position(0,0);
		LCD_Char_1_PrintNumber(RX_DATA[0]);
		CyDelay(500);
		LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);
	} /*forever*/

	return 0;
} /*main()*/
