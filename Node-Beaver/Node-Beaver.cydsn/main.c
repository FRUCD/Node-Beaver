/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
*/

#include <project.h>


volatile CAN_1_TX_MSG message;



CY_ISR(ISR_CAN_1)
{
} /*CAN Interupt*/



int main()
{
	CAN_1_Start();
	CyIntSetVector(CAN_1_ISR_NUMBER, ISR_CAN_1);
	CAN_1_GlobalIntEnable();

	CAN_1_TX_MSG message;
	message.id = 0x001;
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 8;
	message.irq = 0;
	message.msg->byte[0] = 0x01;
	
	for(;;)
	{
		CAN_1_SendMsg(&message);
		CyDelay(500);
	}

	return 0;
} /*main()*/
