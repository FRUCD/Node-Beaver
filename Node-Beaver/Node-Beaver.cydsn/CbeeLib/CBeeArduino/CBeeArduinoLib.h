#ifndef CBeeArduinoLib_H
#define CBeeArduinoLib_H


#include <inttypes.h>

typedef unsigned char Byte;



//*************************************************//
//					 Some constant
//*************************************************//
#define		START_CHAR		0x7E
#define		ESCAPE_CHAR		0x11
#define		ZISE_OF_UINT8_T	0xFF

//*************************************************//
//					API Frame names & ID
//*************************************************//
#define		AT_CMD			0x08		//AT command
#define		AT_CMD_QPV		0x09		//AT command--queue parameter value
#define		ZB_TX_RQ		0X10		//ZigBee Transmit Request	
#define 	ZB_ADD_CMD		0X11		//Explicit Add ZigBee command Frame
#define 	RMT_CMD_RQ		0X17		//Remote Command Remote request
#define		CRT_SRC_RUT		0X21		//Create Source Route
#define		AT_CMD_RSP		0X88		//AT command Response
#define		MOD_STA			0X8A		//Modem Status
#define 	ZB_TM_STA		0x8B		//ZigBee Transmit Status
#define		ZB_REC_PCT		0x90		//ZigBee Receive Packet(AO=0)
#define		ZB_RX_IND		0x91		//ZigBee Explicit Rx Indicator (AO=1)
#define		ZB_IO_RX_IND	0x92		//ZigBee IO data sample Rx indicator
#define		XB_SEN_IND		0x94		//XBee Sensor Read Indicator (AO=0)
#define		NODE_ID_IND		0x95		//Node Identification Indicator
#define		RMT_CMD_RSP		0x97		//Remote Command Response
#define		ON_AIR_UPDATE	0xA0		//Over the air Firmware Update status
#define		RUT_REC_IND		0xA1		//Route Record indicator
#define		MANY_TO_ONE_IND	0xA3		//Many to one route request indicator

//*************************************************//
//					Command reference table
//*************************************************//

//need a function read string and return byte*
uint8_t _get_char_len(char*);
uint8_t _get_Byte_len(Byte*);
Byte 	_checksum(Byte*,uint8_t);

typedef struct XBee_addr64_t{
	Byte api_frame_addr[8];
	void (*_set_addr)(struct XBee_addr64_t* , Byte*);
	void (*_copy_to)(struct XBee_addr64_t*, Byte*);
} XBee_addr64;
void XBee_addr64_init();
void (set_addr64)(struct XBee_addr64_t* , Byte*);
void (copy64_to)(struct XBee_addr64_t*, Byte*);

typedef struct XBee_addr16_t{
	Byte api_frame_addr[2];
	void (*_set_addr)(struct XBee_addr16_t* , Byte*);
	void (*_copy_to)(struct XBee_addr16_t*, Byte*);
} XBee_addr16;
void XBee_addr16_init();
void (set_addr16)(struct XBee_addr16_t* , Byte*);
void (copy16_to)(struct XBee_addr16_t*, Byte*);


typedef struct XBee_addr_t{
	Byte api_frame_addr64[8];
	Byte api_frame_addr16[2];
	void (*_set_addr)(struct XBee_addr_t* , Byte*,Byte*);
} XBee_addr;
void XBee_addr_init();
void (set_all_addr)(struct XBee_addr_t* , Byte*,Byte*);



//*************************************************//
//					main struct function
//*************************************************//





typedef struct XBee_t{
	char* name;
	Byte api_pan;
	Byte api_self64_addr[8];
	Byte api_self16_addr[2];
	Byte* api_all;
	uint8_t all_len;

	struct XBee_request_t* xbee_request;
	struct XBee_response_t* xbee_response;
	char* (*_get_all_char)(struct XBee_t*);
	uint8_t (*_frame_compile)(struct XBee_t*);
	void (*_set_XBee_addr64)(struct XBee_t* , char*);
	void (*_set_request)(struct XBee_t*, struct XBee_request_t*);
}XBee;  //need a initiator
void XBee_init();  
uint8_t (frame_compile)(struct XBee_t*);		//may not need this function, because get all char it self should do the compile job
char* (get_all_char)(struct XBee_t* this);
void (set_XBee_addr64)(struct XBee_t* , char*);
void (set_request)(struct XBee_t*, struct XBee_request_t*);



typedef struct XBee_request_t{
	Byte api_frame_len_H;
	Byte api_frame_len_L;
	Byte api_frame_type;
	Byte api_frame_id;
	Byte api_tar64_addr[8];
	Byte api_tar16_addr[2];
	Byte* api_content;
	uint8_t data_len;   //length of frame-specific
	
	//these functions are not neccessary but incase for using
	uint8_t (*_get_frame_length)(struct XBee_request_t*);
	
	void (*_set_target_addr)(struct XBee_request_t*, Byte*);
	void (*_append_content)(struct XBee_request_t*, Byte*);
	void (*_addn_content)(struct XBee_request_t*, Byte*);

	//functions below are tons of avaliable cmd: addr64,addr16,broadcast radius,options,data. ALSO DO LENGTH CHECK
	void (*_zb_tx_rq)(struct XBee_request_t*, struct XBee_addr64_t*, struct XBee_addr16_t*, Byte,Byte,char*);

}XBee_request;	//This struct receives target add,
void XBee_request_init();
uint8_t (get_frame_length)(struct XBee_request_t*);
void (set_target_addr)(struct XBee_request_t*, Byte*);
void (append_content)(struct XBee_request_t*, Byte*);
void (addn_content)(struct XBee_request_t*, Byte*);
//functions below are tons of avaliable cmd: addr64,addr16,broadcast radius,options,data. ALSO DO LENGTH CHECK
void (zb_tx_rq)(struct XBee_request_t*, struct XBee_addr64_t*, struct XBee_addr16_t*, Byte,Byte,char*);






#endif





