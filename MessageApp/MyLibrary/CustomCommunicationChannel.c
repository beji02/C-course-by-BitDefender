#include "CustomCommunicationChannel.h"


CC_ERROR CreateCC(P_COMMUNICATION_CHANNEL* PCC, CM_SERVER* Server, CM_SERVER_CLIENT* Client)
{
    P_COMMUNICATION_CHANNEL newPCC = NULL;
    newPCC = (P_COMMUNICATION_CHANNEL) malloc(sizeof(COMMUNICATION_CHANNEL));
    if (newPCC == NULL)
        return CC_ERROR_MEMORY;
    newPCC->Client = Client;
    newPCC->Server = Server;

    CM_ERROR error;
    error = CreateDataBuffer(&newPCC->DataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        free(newPCC);
        return CC_ERROR_MEMORY;
    }
    *PCC = newPCC;
    return CC_SUCCES;
}
CC_ERROR SendMessageToClient(P_COMMUNICATION_CHANNEL PCC, const char* Message)
{
    const SIZE_T length = (strlen(Message) + 1) * sizeof(char);
    CM_ERROR error;
    error = CopyDataIntoBuffer(PCC->DataBuffer, (const CM_BYTE*)Message, (CM_SIZE)length);
    if (CM_IS_ERROR(error))
        return CC_ERROR_MEMORY;

    CM_SIZE sentByteCount = 0;
    error = SendDataToClient(PCC->Client, PCC->DataBuffer, &sentByteCount);
    
    CM_LOG_INFO(TEXT("SENT %d BYTES"), sentByteCount);
    if (CM_IS_ERROR(error) || sentByteCount != (CM_SIZE)length)
        return CC_ERROR_SENT;
    return CC_SUCCES;
}
CC_ERROR ReceiveMessageFromServer(P_COMMUNICATION_CHANNEL PCC)
{
    CM_SIZE receivedByteCount = 0;
    CM_ERROR error;
    error = ReceiveDataFormServer(PCC->Client, PCC->DataBuffer, &receivedByteCount);
    CM_LOG_INFO(TEXT("RECEIVED %d BYTES"), receivedByteCount);
    if (CM_IS_ERROR(error))
        return CC_ERROR_RECEIVED;
    return CC_SUCCES;
}
CM_BYTE* ReadMessage(P_COMMUNICATION_CHANNEL PCC)
{
    return PCC->DataBuffer->DataBuffer;
}
void DeleteCC(P_COMMUNICATION_CHANNEL PCC) 
{
    if(PCC != NULL && PCC->DataBuffer != NULL)
        DestroyDataBuffer(PCC->DataBuffer);
    if(PCC != NULL)
        free(PCC);
    return CC_SUCCES;
}