#include "radio_manager.h"

void myUART_Start(uint8_t option){
    switch (option){
        case 0:
             UART_1_Start();
            break;
        default:
            return;
    }
    return;
}


void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
    myUART_Start(RADIO_UART);
    
} // radio_put()


