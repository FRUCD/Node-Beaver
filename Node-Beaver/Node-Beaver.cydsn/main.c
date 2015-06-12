/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
 */

#include <project.h>

#include "data.h"
#include "time_manager.h"
#include "can_manager.h"
#include "usb_manager.h"
#include "sd_manager.h"
#include "radio_manager.h"



int main(void)
{
	CYGlobalIntEnable;

    
	//LCD_Char_1_Start(); // Debug LCD Initialization
	LED_Write(0);
	//LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	DataPacket data_queue[DATA_QUEUE_LENGTH];
	uint16_t data_head, data_tail;
	data_head = data_tail = 0;
    
	time_init();
	can_init();
	usb_init();
	sd_init(time_get());
//	radio_init();

	for(;;)
	{
//	can_test_send();
//	can_get(data_queue, &data_head, &data_tail);
//	usb_get();
		time_announce(data_queue, &data_head, &data_tail);
		
		//inject message to test usb
		data_queue[data_tail].millicounter = millis_timer_ReadCounter();
        
		data_queue[data_tail].id = 0x111;
		data_queue[data_tail].length = 8;
		data_queue[data_tail].data[0]= 0;
		data_queue[data_tail].data[1]= 1;
		data_queue[data_tail].data[2]= 2;
		data_queue[data_tail].data[3]= 3;
		data_queue[data_tail].data[4]= 4;
		data_queue[data_tail].data[5]= 5;
		data_queue[data_tail].data[6]= 6;
		data_queue[data_tail].data[7]= 0x7E;
		data_tail++;
        
		usb_put(data_queue, data_head, data_tail);
		sd_push(data_queue, data_head, data_tail);
        
//		radio_put(data_queue, data_head, data_tail);
		data_head = data_tail = 0; // clear buffer

		CyDelay(1000);
	} // main loop

	return 0;
} // main()
