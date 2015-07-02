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

int struct2ArrConverter(uint8_t *inpArr, int arrSize, uint8_t *outArr)
{
	for(int i = 0; i < arrSize; i++)
	{
		outArr[i] = inpArr[i];
	}
	return 0;
}
//put here array, and get message array that is 8-byte CAN message to send and value of next step pointer
int mesQueueProcedure(uint8_t *inpArr, int mesPointer, uint8_t *message, int arrLength)
{
//#ifdef INS
//	module = 1;
//#endif
	int counter=0;
	for(int i = mesPointer*8; i < (mesPointer+1)*8; i++)
	{
		message[counter++] = inpArr[i];
	}
	if(mesPointer==0)
	{
		prepareSTDID(3,module);
	}
	else if(mesPointer==arrLength-1)
	{
		prepareSTDID(2,module);
		return -1;
	}
	else
	{
		prepareSTDID(1,module);
	}
	return mesPointer+1;
}

int protocolMessageProcessor(uint8_t *inpArr, int arrSize, uint8_t *outArr)//makes protocol type array from incoming array
{
	//calculate estimated messages quantity
	int trueSize = 0,inpIndex = 0, outIndex = 0;
	if(arrSize%7)
		trueSize = arrSize/7 + 1;
	else
		trueSize = arrSize/7;
	//now making all that messages situating to protocol requirements
	for(int i = 0; i < trueSize; i++)
	{
//		inpIndex = i*7;
//		outIndex = i*7;
		for(int a = 0; a < 7; a++)
		{
			if(inpIndex < arrSize)//check if it's time to set zeros
			{
				outArr[outIndex++] = *inpArr;
				inpIndex++;
				inpArr++;
			}
			else
				outArr[outIndex++] = 0;

		}
		if(i == 0)
			outArr[outIndex++] = trueSize;
		else if(i==trueSize-1)
		{
			outArr[outIndex] = 0;
			outArr[outIndex] = calcCSofArr(outArr,outIndex);
		}
		else
		{
			outArr[outIndex++] = i;
		}

	}
}

int getArraySize(uint8_t *inpArr, int arrSize)
{
	int size = 0;
	if(arrSize%7!=0)
		size = arrSize/7 + 1;
	else
		size = arrSize/7;
	return size*8;
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

void setTxDataMessage(ModuleType module,uint8_t *inpArr)
{
	uint32_t *eid,*stdid;
//	eid  = (uint32_t *)EXTID;
	stdid = (uint32_t *)&STDID;
//	TxMessage.ExtId = *eid;
	TxMessage.StdId = *stdid;
	TxMessage.RTR = CAN_RTR_DATA;
//	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	for(int i = 0; i < 8; i++)
		TxMessage.Data[i] = inpArr[i];
//    TxMessage.Data[0] = (char)inpArr[0];
//	TxMessage.Data[1] = (char)(MDLUTransmitData.LAx>>8);
//	TxMessage.Data[2] = (char)MDLUTransmitData.LAy;
//	TxMessage.Data[3] = (char)(MDLUTransmitData.LAy>>8);
//	TxMessage.Data[4] = (char)MDLUTransmitData.LAz;
//	TxMessage.Data[5] = (char)(MDLUTransmitData.LAz>>8);
//	TxMessage.Data[6] = MDLUTransmitData.service;
//	TxMessage.Data[7] = 0;				//constant for a some time
//	if(module == MDLU)
//	{
//		TxMessage.Data[0] = (char)MDLUTransmitData.LAx;
//		TxMessage.Data[1] = (char)(MDLUTransmitData.LAx>>8);
//		TxMessage.Data[2] = (char)MDLUTransmitData.LAy;
//		TxMessage.Data[3] = (char)(MDLUTransmitData.LAy>>8);
//		TxMessage.Data[4] = (char)MDLUTransmitData.LAz;
//		TxMessage.Data[5] = (char)(MDLUTransmitData.LAz>>8);
//		TxMessage.Data[6] = MDLUTransmitData.service;
//		TxMessage.Data[7] = 0;				//constant for a some time
//
//	}
//	if(module == MDUS)
//	{
//		TxMessage.Data[0] = (char)MDUSTransmitData.ARx;
//		TxMessage.Data[1] = (char)(MDUSTransmitData.ARx>>8);
//		TxMessage.Data[2] = (char)MDUSTransmitData.ARy;
//		TxMessage.Data[3] = (char)(MDUSTransmitData.ARy>>8);
//		TxMessage.Data[4] = (char)MDUSTransmitData.ARz;
//		TxMessage.Data[5] = (char)(MDUSTransmitData.ARz>>8);
//		TxMessage.Data[6] = MDUSTransmitData.service;
//		TxMessage.Data[7] = 0;				//constant for a some time
//	}
//	if(module == MDAD)
//	{
//		TxMessage.Data[0] = (char)MDADTransmitData.pressure;
//		TxMessage.Data[1] = (char)(MDADTransmitData.pressure>>8);
//		TxMessage.Data[2] = (char)(MDADTransmitData.pressure>>16);
//		TxMessage.Data[3] = (char)MDADTransmitData.temperature;
//		TxMessage.Data[4] = (char)(MDADTransmitData.temperature>>8);
//		TxMessage.Data[5] = (char)(MDADTransmitData.temperature>>16);
//		TxMessage.Data[6] = MDADTransmitData.service;
//		TxMessage.Data[7] = 0;				//constant for a some time
//
//	}
//	if(module == SatteliteModule)
//	{
//
//		TxMessage.Data[0] = arrCANdata[0];
//		TxMessage.Data[1] = arrCANdata[1];
//		TxMessage.Data[2] = arrCANdata[2];
//		TxMessage.Data[3] = arrCANdata[3];
//		TxMessage.Data[4] = arrCANdata[4];
//		TxMessage.Data[5] = arrCANdata[5];
//		TxMessage.Data[6] = arrCANdata[6];
//		TxMessage.Data[7] = arrCANdata[7];
//	}
//	TxMessage.Data[7] = calcCSofArr(TxMessage.Data,8);

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
