#include "radio_manager.h"


void _putByte_escape(uint8_t* msg){
    uint8_t i=0;
    for(i=0;i<(DATA_LEN+1);i++){
        if (msg[i]==0x7E){
            UART_1_PutChar(ESCAPE);
            UART_1_PutChar(msg[i]);
    }else{
        UART_1_PutChar(msg[i]);
    }
    }
    return;

}


uint8_t checksum(uint8_t* msg){
    uint8_t cksum=0x00;
    uint8_t i=0;
    for (i=0;i<DATA_LEN;i++){
        cksum=cksum+msg[i];
    }
    cksum=0xff-cksum;
    return cksum;
}



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

void radio_get_bytes(const DataPacket* in_pkt,uint8_t* byte_ptr){
    uint8_t i=0;
    
    //bound is 8
    for (i=0;i<8;i++){    
        byte_ptr[7-i]=((in_pkt->time)>>(i*8))&0xFF;
    }
    
    byte_ptr[4]=(in_pkt->type);
    
    //bound is 2
    byte_ptr[5]=((in_pkt->id)&0xFF00)>>8;
    byte_ptr[6]=((in_pkt->id)&0xFF);
    
    //bound is 16
    
    for (i=0;i<16;i++){    
        byte_ptr[15-i]=((in_pkt->value)>>(i*8))&0xFF;
    }
    
    return;

}


void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
    uint8_t all_data[DATA_LEN+1];
    myUART_Start(RADIO_UART);       //start uart_1
    radio_get_bytes(data_queue,all_data);
    all_data[DATA_LEN]=checksum(all_data);
    
    
    //send byte by byte
    _putByte_escape(all_data);
    
} // radio_put()


