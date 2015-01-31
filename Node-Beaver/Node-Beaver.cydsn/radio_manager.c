#include "radio_manager.h"
/*
typedef struct
{
	uint32_t millicounter;
	uint16_t id; // id is for tracking CAN ID
	uint8_t length;
	uint8_t data[8];
} DataPacket;
*/


void radio_init(void)
{
	myUART_Start(0);
}



uint64_t addr[8];


uint8_t translator(uint8_t data){
    uint8_t Lside=data&0xf;
    uint8_t Hside=(data>>4)&0xf;
    UART_1_PutChar(Hside<0xa ? Hside+'0':Hside+'A'-0xa);
    UART_1_PutChar(Lside<0xa ? Lside+'0':Lside+'A'-0xa);
    UART_1_PutChar(' ');
    return 0;
}


void _set_des_addr(uint64_t newAddr){
    int i=0;
    for (i=0;i<8;i++){
        addr[7-i]=(newAddr>>(8*i))&0xff;
    }
    return;
}



void _XBee_tx_req_(const DataPacket* msg){
    int i=0;
    uint8_t send_msg[32];
    
    send_msg[0]=STARTER_DELIM;  //starter   
    send_msg[1]=0x00;           //MSB L
    send_msg[2]=0x1d;           //LSB L
    
    send_msg[3]=0x10;           //Frame type
    send_msg[4]=0x01;           //frame id
    
    send_msg[5]=addr[0];        //addr 8+2
    send_msg[6]=addr[1];
    send_msg[7]=addr[2];
    send_msg[8]=addr[3];
    send_msg[9]=addr[4];
    send_msg[10]=addr[5];
    send_msg[11]=addr[6];
    send_msg[12]=addr[7];
    send_msg[13]=0xff;
    send_msg[14]=0xfe;
    
    send_msg[15]=0x00;          //Broadcast
    send_msg[16]=0x00;          //opions
    
    send_msg[19]=(msg->millicounter)&0xff;      //time
    send_msg[18]=(msg->millicounter)>>8&0xff;
    send_msg[17]=(msg->millicounter)>>16&0xff;

    
    send_msg[21]=msg->id&0xff;      //id
    send_msg[20]=(msg->id>>8)&0xff;
    
    send_msg[22]=msg->length;      //length
    
    
    send_msg[23]=msg->data[0];     //value
    send_msg[24]=msg->data[1];
    send_msg[25]=msg->data[2];  
    send_msg[26]=msg->data[3];  
    send_msg[27]=msg->data[4];  
    send_msg[28]=msg->data[5];  
    send_msg[29]=msg->data[6];  
    send_msg[30]=msg->data[7];  
   
    

    
    send_msg[31]=checksum(send_msg,31);
    
    for (i=0;i<32;i++){
        UART_1_PutChar(send_msg[i]);
        //UART_1_PutChar(' ');
    //    translator(send_msg[i]);
        
    }
   // UART_1_PutChar('\r');
   // UART_1_PutChar('\n');
    return;
    
    
    
}




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


uint8_t checksum(uint8_t* msg,int len){
    uint8_t cksum=0x00;
    uint8_t i=0;
    for (i=3;i<len;i++){
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

/*
void radio_get_bytes(const DataPacket in_pkt,uint8_t* byte_ptr){
    uint8_t i=0;
    
    //bound is 4
    for (i=0;i<4;i++){    
        byte_ptr[7-i]=((in_pkt.time)>>(8))&0xFF;
    }
    
    byte_ptr[4]=(in_pkt.type);
    
    //bound is 2
    byte_ptr[6]=((in_pkt.id)&0xFF);
    byte_ptr[5]=((in_pkt.id)>>8)&0xFF;
    
    //bound is 8
    
    for (i=0;i<8;i++){    
        byte_ptr[15-i]=((in_pkt.value)>>(8))&0xFF;
    }
    
    return;

}

*/

void dummy_put(){
     int i=0;
    uint8_t send_msg[40]={0x7e,0x00,0x24,0x10,0x01,0x00,0x13,0xa2,0x00,0x40,0xc8,0x4f,0xbf,0xff,0xfe,0x00,0x00,0x73,0x65,0x6e,0x64,0x20,0x66,0x72,0x6f,0x6d,0x20,0x61,0x6c,0x69,0x65,0x6e,0x20,0x70,0x6c,0x61,0x6e,0x65,0x74,0xdb};
    for (i=0;i<40;i++){
        UART_1_PutChar(send_msg[i]);
    }
    return;
}


void radio_put(const DataPacket* data_queue, uint16_t data_head,
	uint16_t data_tail)
{
    
    _set_des_addr(_RECEIVER_ADDR);        //set default receiver addr
    
    
    
    uint16_t data_ptr;
    //_XBee_tx_req_(&(data_queue[0])); // test

		
    for(data_ptr=data_head; data_ptr!=data_tail; data_ptr=(data_ptr+1)%DATA_QUEUE_LENGTH){
        _XBee_tx_req_(&(data_queue[data_ptr]));

    
        //dummy_put();
    }
    

} // radio_put()


