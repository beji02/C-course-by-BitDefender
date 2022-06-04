#include "stdafx.h"
#include "custom_client_com_api.h"




SV_CON_ERROR CreateServerConnection(SERVER_CONNECTION** PSvCon)
{
    // alocam memorie pt com channel
    SERVER_CONNECTION* newPSvCon = NULL;
    newPSvCon = (SERVER_CONNECTION*)malloc(sizeof(SERVER_CONNECTION));
    if (newPSvCon == NULL)
    {
        return SV_CON_ERROR_MEMORY;
    }
    
     // initializez modulul de comunicare
    CM_ERROR error = InitCommunicationModule();
    if (CM_IS_ERROR(error))
    {
        free(newPSvCon);
        return SV_CON_ERROR_MODULE;
    }
    CM_LOG_INFO(TEXT("-CHILD- initializarea modulului de comunicare a reusit"));

    // creez un nou client(conexiune) pentru a primi informatia
    newPSvCon->ReceiveData = NULL;
    error = CreateClientConnectionToServer(&newPSvCon->ReceiveData);
    if (CM_IS_ERROR(error))
    {
        UninitCommunicationModule();
        free(newPSvCon);
        return SV_CON_ERROR_SERVER_DOWN;
    }
    CM_LOG_INFO(TEXT("-CHILD- conexiunea a reusit"));


    // creez un nou client(conexiune) pentru a trimite informatia
    newPSvCon->SendData = NULL;
    error = CreateClientConnectionToServer(&newPSvCon->SendData);
    if (CM_IS_ERROR(error))
    {
        DestroyClient(newPSvCon->ReceiveData);
        UninitCommunicationModule();
        free(newPSvCon);
        return SV_CON_ERROR_SERVER_DOWN;
    }
    CM_LOG_INFO(TEXT("-CHILD- conexiunea a reusit"));

    // creez un nou buffer pentru a primi informatia
    error = CreateDataBuffer(&newPSvCon->ReceiveDataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        DestroyClient(newPSvCon->SendData);
        DestroyClient(newPSvCon->ReceiveData);
        UninitCommunicationModule();
        free(newPSvCon);
        return SV_CON_ERROR_MEMORY;
    }
    
    // creez un nou buffer pentru a trimite informatia
    error = CreateDataBuffer(&newPSvCon->SendDataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        DestroyDataBuffer(newPSvCon->ReceiveDataBuffer);
        DestroyClient(newPSvCon->SendData);
        DestroyClient(newPSvCon->ReceiveData);
        UninitCommunicationModule();
        free(newPSvCon);
        return SV_CON_ERROR_MEMORY;
    }

    *PSvCon = newPSvCon;
    return SV_CON_SUCCESS;
}
SV_CON_ERROR DestroyServerConnection(SERVER_CONNECTION* SvCon)
{
    DestroyDataBuffer(SvCon->SendDataBuffer);
    DestroyDataBuffer(SvCon->ReceiveDataBuffer);
    DestroyClient(SvCon->SendData);
    DestroyClient(SvCon->ReceiveData);
    UninitCommunicationModule();
    free(SvCon);
    return SV_CON_SUCCESS;
}
SV_CON_ERROR ReceiveMessageFromServer(SERVER_CONNECTION* SvCon)
{
    CM_SIZE receivedByteCount = 0;
    CM_ERROR error;
    error = ReceiveDataFormServer(SvCon->ReceiveData, SvCon->ReceiveDataBuffer, &receivedByteCount);

    CM_LOG_INFO(TEXT("RECEIVED %d BYTES"), receivedByteCount);
    if (CM_IS_ERROR(error))
    {
        return SV_CON_ERROR_RECEIVED;
    }
    return SV_CON_SUCCESS;
}
wchar_t* ReadMessageFromServer(SERVER_CONNECTION* SvCon)
{
    return (wchar_t*)SvCon->ReceiveDataBuffer->DataBuffer;
}
SV_CON_ERROR SendMessageToServer(SERVER_CONNECTION* SvCon, const wchar_t* Message)
{
    CM_ERROR error;
    SIZE_T messageLength = (wcslen(Message) + 1) * sizeof(wchar_t);

    error = CopyDataIntoBuffer(SvCon->SendDataBuffer, (const CM_BYTE*)Message, (CM_SIZE)messageLength);
    if (CM_IS_ERROR(error))
    {
        CM_LOG_ERROR(TEXT("CopyDataIntoBuffer failed with err-code=0x%X!\n"), error);
        return SV_CON_ERROR_MEMORY;
    }

    // trimitem informatia clientului
    CM_SIZE sentByteCount = 0;
    error = SendDataToServer(SvCon->SendData, SvCon->SendDataBuffer, &sentByteCount);

    CM_LOG_INFO(TEXT("SENT %d BYTES"), sentByteCount);
    if (CM_IS_ERROR(error))
    {
        return SV_CON_ERROR_SENT;
    }

    return SV_CON_SUCCESS;
}
