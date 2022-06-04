#pragma once
#include "common.h"
#include "communication_api.h"
#include <stdio.h>
#include <windows.h>
#include "communication_logging.h"


#define MAX_BUFFER_SIZE (100 * sizeof(char))



typedef struct _CommunicationChannel	
{
	CM_SERVER* Server;
	CM_SERVER_CLIENT* Client;
	CM_DATA_BUFFER* DataBuffer;
}COMMUNICATION_CHANNEL,* P_COMMUNICATION_CHANNEL;



typedef long CC_ERROR;
#define CC_SUCCES 0
#define CC_ERROR_MEMORY 1
#define CC_ERROR_SENT 2
#define CC_ERROR_RECEIVED 3


CC_ERROR CreateCC(P_COMMUNICATION_CHANNEL* PCC, CM_SERVER* Server, CM_SERVER_CLIENT* Client);
CC_ERROR SendMessageToClient(P_COMMUNICATION_CHANNEL PCC, const char* Message);
CC_ERROR ReceiveMessageFromServer(P_COMMUNICATION_CHANNEL PCC);
CM_BYTE* ReadMessage(P_COMMUNICATION_CHANNEL PCC);
void DeleteCC(P_COMMUNICATION_CHANNEL PCC);