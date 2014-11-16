/* Node-Beaver
	 This node logs data via CAN and presents the data to the outside world.
 */

#include <project.h>
#include <FS.h>

#include "data.h"
#include "can_manager.h"
#include "sd_manager.h"



int main()
{
	CYGlobalIntEnable;

	CAN_1_GlobalIntEnable(); // CAN Initialization
	CAN_1_Init();
	CAN_1_Start();

	LCD_Char_1_Start(); // Debug LCD Initialization
	LED_Write(0);
	LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);

	sd_init(); // sd card initialization
  
  USBUART_1_CDC_Init(); // USB Initialization


	uint8_t state;
	for(;;)
	{
		state = can_handler();
		//state = CAN_1_SendMsg0();

		if(state == CYRET_SUCCESS) /*LED ON if message is sent succesfully*/
			LED_Write(1);
		else
			LED_Write(0);
		//CyDelay(500);
		//LCD_Char_1_WriteControl(LCD_Char_1_CLEAR_DISPLAY);
    
    // Read and format CAN
    // Send to USB
    // Write to SD or SD queue
	} // main loop

	return 0;
} // main()
