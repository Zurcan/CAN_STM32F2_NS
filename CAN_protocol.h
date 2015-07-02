#include <stdint.h>
#include <can.h>

typedef struct
{
	short LAx;			//linearic acceleration on X-axis
	short LAy;			//linearic acceleration on Y-axis
	short LAz;			//linearic acceleration on Z-axis
	uint8_t service;			//sevice byte
	uint8_t CSL;				//checksum
}MDLUdataType;

typedef struct
{
	short ARx;			//angle rate on X-axis
	short ARy;			//angle rate on Y-axis
	short ARz;			//angle rate on Z-axis
	uint8_t service;			//service byte
	uint8_t CSL;				//checksum
}MDUSdataType;



typedef struct
{
	int pressure;
	int temperature;
	uint8_t service;
	uint8_t CSL;
	uint16_t offset;
}MDADdataType;


typedef struct
{

	unsigned reserved02:3;			//ID0-ID2 - reserved 011
	unsigned packetType:4;			//ID3-ID6  module type inertial 0001, sattelite 0010, interface module 0011, radio modem 0100, UHF 0101
	unsigned messageType:2;		//ID7-ID8  0 - single packet; 3 - 1st packet, has packet amount; 2 - CS of all packets, last frame; 1 - not 1s nor last packet, has number of packet;
	unsigned reserved910:2;		    //ID9-ID10 - reserved 11
	unsigned unused:21;

} standardID;



typedef struct
{
	unsigned deviceSerialNumber:12;				//EID11-EID0 serial number of device
	unsigned messageMode:2; 					//EID13, EID12 0 - single packet; 3 - 1st packet, has packet amount; 2 - CS of all packets, last frame; 1 - not 1s nor last packet, has number of packet;
	unsigned messageType:4; 					//EID17-EID14   0b0000 command; 0b0101 answer; 0b1010 data;
	unsigned deviceType:7;						//ID6-ID0  0b00000000 MDLU; 0b00101010 MDUS; 0b01010101 MDAD;
	unsigned priorityLevel:4;					//ID10-ID7  by default priority level is 0111
	unsigned unused:3;

}extendedID;

//typedef INT12U type = (aRecord->bytes[0] & 0xF0) >> 4;
/*
 *  ICL:ICM:ICH 		0xE7E7E7 - disable MDLU polling, 0xAAAAAA - enable MDLU polling
 *						0xC3C3C3 - disable MDUS polling, 0xA5A5A5 - enable MDUS polling
 *						0xDBDBDB - disable MDAD polling, 0xBDBDBD - enable MDAD polling
 *	IDL:IDH:DIDL:DIDH 	0x0001..0x01f4 - frequency of MDLU polling, 0x0001..0x0050 - MDUS, 0x0001..0x0019 - MDAD
 *
 */
typedef struct
{
	char instructionCode[3];     			// ICL:ICM:ICH
	char configData[2]; 					// IDL:IDH
	char dConfigData[2];					// DIDL:DIDH
	char CSL;							// low byte of checksum CSL = DATA0+DATA1+...+DATA6

}configIIM;

typedef struct
{
	float XSAT;
	float YSAT;
	float ZSAT;
	float LattitudeB;//rads
	float LongtitudeL;//rads
	float AltitudeH;//meters
}SNSCoordMessage;

typedef struct
{
	float XIN;
	float YIN;
	float ZIN;
	float xDIS;//axial displacement by x;
	float yDIS;//axial displacement by y;
	float zDIS;//axial displacement by z;
	float aX;  //acceleration by x;
	float aY;  //acceleration by y;
	float aZ;  //acceleration by z;
	float wX;  //angle speed by x;
	float wY;  //angle speed by y;
	float wZ;  //angle speed by z;
	float angleX;
	float angleY;
	float angleZ;
}INSCoordMessage;

typedef struct
{
	float XKLN;
	float YKLN;
	float ZKLN;
	float xShift;
	float yShift;
	float zShift;
	float dist;
	float azimuth;
}KLNCoordMessage;

typedef enum {
	INScoords = 1,
	SNScoords = 2,
	reserved1 = 3,
	reserved2 = 4,
	KLNcoords = 5
}CANpackets;

typedef enum{
	SingleMessage = 0,
	MultyMessageMiddle = 1,
	MultyMessageLast = 2,
	MultyMessageFirst = 3
}MessageTypes;
typedef enum {
				BasicModule = 0,
				MDLU=0x01,
				MDUS = 0x02,
				MDAD = 0x03,
				RadioTransmitter = 0x04,
				SatteliteModule = 5,
				NetworkModule = 6,
				RadioModem = 7
			  }ModuleType;
typedef enum {
				Command=0x00,
				Answer = 0x5,
				Data = 0xA
			  }IIMmessageType;

typedef enum {
				IIM_enablePolling_MDLU = 0xAA,
				IIM_disablePolling_MDLU = 0xE7,
				IIM_enablePolling_MDUS = 0xA5,
				IIM_disablePolling_MDUS = 0xC3,
				IIM_enablePolling_MDAD = 0xBD,
				IIM_disablePolling_MDAD = 0xDB
			 }IIMInstructionCode;



ModuleType IIMTyp, IIMTransmitType;
IIMInstructionCode IIMinstr;
extendedID *EXTID;
standardID STDID;
MDLUdataType MDLUdata;
MDUSdataType MDUSdata;
MDADdataType MDADdata;
MDLUdataType MDLUTransmitData;
MDUSdataType MDUSTransmitData;
MDADdataType MDADTransmitData;
MessageTypes mTypes;
CANpackets pTypes;
CanTxMsgTypeDef TxMessage;
CanTxMsgTypeDef framedMessagesArr[86];
CanRxMsgTypeDef RxMessage;
configIIM IIMConf;
uint8_t module;
//INSCoordMessage insdata;

int getArraySize(uint8_t *inpArr,int arrSize);
void setTxDataMessage(ModuleType module, uint8_t* inpArr);
void prepareEXTID(short serial, char msgtype,char devtype,char priority, char msgMode);
void prepareSTDID(char mestype, char ptype);
int var2ArrConverter(char * inpArr, int arrSize, char* outArr);
int struct2ArrConverter(uint8_t *inpArr, int arrSize, uint8_t *outArr);
int protocolMessageProcessor(uint8_t *inpArr, int arrSize, uint8_t *outArr);
int mesQueueProcedure(uint8_t *inpArr, int mesPointer, uint8_t *message, int arrLength);
uint8_t calcCSofArr(uint8_t *arr, uint8_t arrSize);
