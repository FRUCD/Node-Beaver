#include "CBeeArduinoLib.h"
#include "stdio.h"
#include "stdlib.h"


int main(){
	char addr[2];
	addr[0]='a';
	addr[1]='b';
	Byte addr64_ar[8]={'t','h','i','s','i','s','a','d'};
	XBee coordinator;
	XBee_init(&coordinator);
	printf("This is addr64 for coordinator after init: %s\n",coordinator.api_self64_addr);
	XBee_addr16 addr16;
	XBee_addr16_init(&addr16);
	addr16._set_addr(&addr16,addr);
	printf("This is addr16 for addr16 after init: %c%c\n",addr16.api_frame_addr[0],addr16.api_frame_addr[1]);
	XBee_addr64 addr64;
	XBee_addr64_init(&addr64);
	addr64._set_addr(&addr64,addr64_ar);
	XBee_request TXReq;
	XBee_request_init(TXReq);
	TXReq._zb_tx_rq(&TXReq,&addr64,&addr16,0x00,0x00,"I wanna see if this msg can be sent.");
	coordinator._set_request(&coordinator,&TXReq);
	printf("This is after set request of xbee: %s\n",coordinator._get_all_char);

}
