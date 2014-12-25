#include <CBeeConnector.h>

int XBConnet(myXBeePort* passport){
	switch (*passport.XBType){
		case (XBSERIAL):
			break;
		case (XBI2C):
			break;
		case (XBSPI):
			break;
		case (XBVIRT):
			TranSimu_init(*passport.XBMqch,SENDERFLG);
			break;
		default:
			break;
	}
}


int XBSend()