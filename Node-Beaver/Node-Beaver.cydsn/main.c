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
	CAN_1_TX_MSG message;
	message.id = 0x001;
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 8;
	message.irq = 1;
	message.msg->byte[0] = 0x01;

	int i;
	for(i=1;i<=7;i++)
		message.msg->byte[i] = 0x00;
   
	
	CyIntSetVector(CAN_1_ISR_NUMBER, ISR_CAN_1);
	CAN_1_GlobalIntEnable();
	CAN_1_Init();
	CAN_1_Start();

	
	for(;;)
	{
		CAN_1_SendMsg(&message);
		CyDelay(100);
	}

	return 0;
} /*main()*/
