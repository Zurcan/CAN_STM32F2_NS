/*This is CAN_protocoooooool!
 *
 */

#include <CAN_protocol.h>


uint8_t arrCANdata[8];
int var2ArrConverter(char * inpArr, int arrSize, char* outArr)
{
	for(int i = 0; i < arrSize; i++)
	{
		outArr[i] = inpArr[i];
	}
	return 0;
}

void prepareSTDID(char mestype, char ptype)
{
	STDID.reserved02 = 3;
	STDID.packetType = ptype;
	STDID.messageType = mestype;
	STDID.reserved910 = 3;
	STDID.unused = 0;
}

void prepareEXTID(short serial, char msgtype,char devtype,char priority, char msgMode)
{

	EXTID->deviceSerialNumber =serial&0x0fff;//
	EXTID->messageType = msgtype&0x0f;
	EXTID->deviceType = devtype&0x7f;
	EXTID->priorityLevel = priority&0x0f;
	EXTID->messageMode = msgMode&0x03;
	EXTID->unused = 0;

}

void setTxDataMessage(IIMmoduleType module)
{
	uint32_t *eid,*stdid;
	eid  = (uint32_t *)EXTID;
	stdid = (uint32_t *)&STDID;
	TxMessage.ExtId = *eid;
	TxMessage.StdId = *stdid;
	TxMessage.RTR = CAN_RTR_DATA;
//	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	if(module == MDLU)
	{
		TxMessage.Data[0] = (char)MDLUTransmitData.LAx;
		TxMessage.Data[1] = (char)(MDLUTransmitData.LAx>>8);
		TxMessage.Data[2] = (char)MDLUTransmitData.LAy;
		TxMessage.Data[3] = (char)(MDLUTransmitData.LAy>>8);
		TxMessage.Data[4] = (char)MDLUTransmitData.LAz;
		TxMessage.Data[5] = (char)(MDLUTransmitData.LAz>>8);
		TxMessage.Data[6] = MDLUTransmitData.service;
		TxMessage.Data[7] = 0;				//constant for a some time

	}
	if(module == MDUS)
	{
		TxMessage.Data[0] = (char)MDUSTransmitData.ARx;
		TxMessage.Data[1] = (char)(MDUSTransmitData.ARx>>8);
		TxMessage.Data[2] = (char)MDUSTransmitData.ARy;
		TxMessage.Data[3] = (char)(MDUSTransmitData.ARy>>8);
		TxMessage.Data[4] = (char)MDUSTransmitData.ARz;
		TxMessage.Data[5] = (char)(MDUSTransmitData.ARz>>8);
		TxMessage.Data[6] = MDUSTransmitData.service;
		TxMessage.Data[7] = 0;				//constant for a some time
	}
	if(module == MDAD)
	{
		TxMessage.Data[0] = (char)MDADTransmitData.pressure;
		TxMessage.Data[1] = (char)(MDADTransmitData.pressure>>8);
		TxMessage.Data[2] = (char)(MDADTransmitData.pressure>>16);
		TxMessage.Data[3] = (char)MDADTransmitData.temperature;
		TxMessage.Data[4] = (char)(MDADTransmitData.temperature>>8);
		TxMessage.Data[5] = (char)(MDADTransmitData.temperature>>16);
		TxMessage.Data[6] = MDADTransmitData.service;
		TxMessage.Data[7] = 0;				//constant for a some time

	}
	if(module == SatteliteModule)
	{

		TxMessage.Data[0] = arrCANdata[0];
		TxMessage.Data[1] = arrCANdata[1];
		TxMessage.Data[2] = arrCANdata[2];
		TxMessage.Data[3] = arrCANdata[3];
		TxMessage.Data[4] = arrCANdata[4];
		TxMessage.Data[5] = arrCANdata[5];
		TxMessage.Data[6] = arrCANdata[6];
		TxMessage.Data[7] = arrCANdata[7];
	}
	TxMessage.Data[7] = calcCSofArr(TxMessage.Data,8);

}

uint8_t calcCSofArr(uint8_t *arr, uint8_t arrSize)
{
	uint16_t tmpVal=0;
//	tmpVal = *arr;
	for(uint8_t i = 0; i < arrSize; i++)
	{
		tmpVal+=*arr;
		arr++;
	}
	return tmpVal&0x00ff;
}
