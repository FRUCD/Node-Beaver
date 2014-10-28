/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
*/

#include <project.h>



CAN_1_TX_MSG message;
CAN_1_DATA_BYTES_MSG payload;



/*CY_ISR(ISR_CAN_1)
	{
	} CAN Interupt*/



int main()
{
	message.id = 0x001;
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 8;
	message.irq = 1;
	message.msg = &payload;

	/*Fill with zeroes and insert message "1"*/
	uint8_t i;
	for(i=1;i<8;i++) payload.byte[i] = 0x00;
	payload.byte[0] = 0x01;

	CYGlobalIntEnable;

	/*CyIntSetVector(CAN_1_ISR_NUMBER, ISR_CAN_1);
	CAN_1_GlobalIntEnable();*/
	CAN_1_Init();
	CAN_1_Start();

	uint8_t state;
	LED_Write(0);

	for(;;)
	{
		state = CAN_1_SendMsg(&message);

		if(state != CAN_1_FAIL) /*LED ON if message is sent succesfully*/
			LED_Write(1);
		else
			LED_Write(0);
	} /*forever*/

	return 0;
} /*main()*/
