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

	for(;;);

	return 0;
} /*main()*/
