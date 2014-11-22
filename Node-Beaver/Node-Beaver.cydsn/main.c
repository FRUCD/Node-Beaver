/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
 */

#include <project.h>

#include "data.h"
#include "time.h"
#include "can_manager.h"
#include "usb_manager.h"
#include "sd_manager.h"



int main()
{
	CYGlobalIntEnable;

	LCD_Char_1_Start(); // Debug LCD Initialization
	LED_Write(0);
	LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	DataPacket data_queue[CAN_QUEUE_LENGTH];
	uint8_t data_pos = 0;

	time_init();
	can_init();
	usb_init();
	sd_init();

	for(;;)
	{
		can_test_send();
		can_get(data_queue, &data_pos);
		//usb_put(data_queue, data_pos);
		sd_push(data_queue, data_pos);
		data_pos = 0; // clear buffer
		CyDelay(100);
	} // main loop

	return 0;
} // main()
