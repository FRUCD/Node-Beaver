/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
 */

#include <project.h>

#include "data.h"
#include "time.h"
#include "can_manager.h"
#include "usb_manager.h"
#include "sd_manager.h"
#include "radio_manager.h"



int main()
{
	CYGlobalIntEnable;

	LCD_Char_1_Start(); // Debug LCD Initialization
	LED_Write(0);
	LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	DataPacket data_queue[DATA_QUEUE_LENGTH];
	uint16_t data_head, data_tail;
	data_head = data_tail = 0;

	time_init();
	can_init();
	usb_init();
	sd_init(time_get());

	for(;;)
	{
		can_test_send();
		can_get(data_queue, &data_head, &data_tail);
		usb_get();
		usb_put(data_queue, data_head, data_tail);
		sd_push(data_queue, data_head, data_tail);
		radio_put(data_queue, data_head, data_tail);
		data_head = data_tail = 0; // clear buffer

		CyDelay(100);
	} // main loop

	return 0;
} // main()
