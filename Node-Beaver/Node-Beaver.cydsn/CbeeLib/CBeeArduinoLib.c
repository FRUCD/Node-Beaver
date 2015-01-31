#include "CBeeArduinoLib.h"

#include "stdio.h"
#include "stdlib.h"
#include "string"

//				common function/tools
uint8_t _get_char_len(char* msg){
	uint8_t i=0;
	while (msg[i]!='\0' && i<ZISE_OF_UINT8_T){
		i++;
	}
	return i;
}

uint8_t _get_Byte_len(Byte* msg){
	uint8_t i=0;
	while (msg[i]!='\0' && i<ZISE_OF_UINT8_T){
		i++;
	}
	return i;
}


//				for xbee struct
void XBee_init(struct XBee_t* this){
	this->_get_all_char=get_all_char;
	this->_frame_compile=frame_compile;
	this->_set_XBee_addr64=set_Xbee_addr64;
}
uint8_t (*frame_compile)(struct XBee_t*);		//may not need this function, because get all char it self should do the compile job
char* (*get_all_char)(struct XBee_t* this);
void (*set_XBee_addr64)(struct XBee_t* this, char* addr64){
	// for (int i=0;i<8;i++){
	// 	this->api_self64_addr[i]=(Byte)addr64[i];
	// }
	this->api_self64_addr=addr64;
	return;
}
void (*set_request)(struct XBee_init* this, struct XBee_request_t* xbee_request){
	this->xbee_request=xbee_request;
	return;
}


//				for Xbee request struct 
void XBee_request_init(){
	this->_get_frame_length=get_frame_length;
	this->_set_target_addr=set_target_addr;
	this->_append_content=append_content;
	this->_addn_content=addn_content;
	this->_ZB_TX_RQ=ZB_TX_RQ;
	return;
}

uint8_t (*get_frame_length)(struct XBee_request_t* this){
	uint8_t len;
	len=1+1+1+1+8+2;
	len+=_get_Byte_len(this->api_content);
	return len;
}

void (*set_target_addr)(struct XBee_request_t*, Byte*){
	return;
}

void (*append_content)(struct XBee_request_t* this, Byte* msg){
	uint8_t old_len;
	uint8_t new_len;
	Byte*	temp;
	uint8_t i;
	old_len=_get_Byte_len(this->api_content);
	new_len=_get_Byte_len(msg)+old_len+1;
	if (this->api_content==NULL){
		// this->api_content=(Byte*)malloc(sizeof(char)*new_len);
		this->api_content=msg;
	}else{
		old_len=_get_Byte_len(this->api_content);
		new_len=old_len+msg_len+1;
		temp=(Byte*)malloc(sizeof(Byte)*(new_len));
		for(i=0;i<old_len;i++){
			temp[i]=this->api_content[i];
		}
		for (i=0;i<msg_len;i++){
			temp[i+old_len]=msg[i];
		}
		this->api_content=temp;
		temp=NULL;
	}
	return;
}

void (*addn_content)(struct XBee_request_t*, Byte*){
	return;
}
//functions below are tons of avaliable cmd: addr64,addr16,broadcast radius,options,data. ALSO DO LENGTH CHECK
void (*ZB_TX_RQ)(struct XBee_request_t*, struct XBee_addr64, struct XBee_addr16, Byte,Byte,char*);







//				for xbee addr64 struct



//				for xbee addr16 struct