#include "stdafx.h"
#include "custom_server_com_api.h"

int gIdCount = 9991;

SV_ERROR CreateServer(SERVER** PServer, int Argc, TCHAR* Argv[])
{
    // alocam memorie pentru serverul nostru nou 
    SERVER* newServer = NULL;
    newServer = (SERVER*)malloc(sizeof(SERVER));
    if (newServer == NULL)
    {
        return SV_ERROR_MEMORY;
    }

    // preluam argumentele si initializam numarul maxim de clienti 
    if (Argc < 2)
    {
        return SV_ERROR_INVALID;
    }

    newServer->NO_MAX_CLIENTS = (int)_tcstoi64(Argv[1], NULL, 10);
    if (newServer->NO_MAX_CLIENTS == _I64_MIN ||
        newServer->NO_MAX_CLIENTS == _I64_MAX ||
        newServer->NO_MAX_CLIENTS <= 0)
    {
        free(newServer);
        return SV_ERROR_INVALID;
    }
    newServer->NoClients = 0;
    CM_LOG_INFO(TEXT("Numarul maxim de useri ce se pot loga: %d"), newServer->NO_MAX_CLIENTS);

    // initializam modulul de comunicare
    CM_ERROR error = InitCommunicationModule();
    if (CM_IS_ERROR(error))
    {
        free(newServer);
        return SV_ERROR_MODULE;
    }

    // cream serverul cm
    newServer->CMServer = NULL;
    error = CreateCMServer(&newServer->CMServer);
    if (CM_IS_ERROR(error))
    {
        UninitCommunicationModule();
        free(newServer);
        return SV_ERROR_CM;
    }
    CM_LOG_INFO(TEXT("Server is Up & Running"));

    newServer->TableUsers = NULL;
    error = HtCreate(&newServer->TableUsers);
    if (error == -1)
    {
        DestroyCMServer(newServer->CMServer);
        UninitCommunicationModule();
        free(newServer);
        return SV_ERROR_MEMORY;
    }

    newServer->TableClients = NULL;
    error = HtCreate(&newServer->TableClients);
    if (error == -1)
    {
        HtDestroy(&newServer->TableUsers);
        DestroyCMServer(newServer->CMServer);
        UninitCommunicationModule();
        free(newServer);
        return SV_ERROR_MEMORY;
    }
    newServer->NoClientsMutex = CreateMutex(NULL, 0, TEXT("NoClientsMutex"));
    InitializeSRWLock(&newServer->TableUsersRWL);
    InitializeSRWLock(&newServer->TableClientsRWL);
    if (newServer->NoClientsMutex == NULL)
    {
        HtDestroy(&newServer->TableClients);
        HtDestroy(&newServer->TableUsers);
        DestroyCMServer(newServer->CMServer);
        UninitCommunicationModule();
        free(newServer);
        return SV_ERROR_MEMORY;
    }
    *PServer = newServer;

    return SV_SUCCESS;
}
SV_ERROR DestroyServer(SERVER* Server)
{
    CloseHandle(Server->NoClientsMutex);
    HtDestroy(&Server->TableClients);
    HtDestroy(&Server->TableUsers);
    DestroyCMServer(Server->CMServer);
    UninitCommunicationModule();
    free(Server);
    return SV_SUCCESS;
}

CL_CON_ERROR CreateClientConnection(CLIENT_CONNECTION** PClCon, SERVER* Server)
{
    CLIENT_CONNECTION* newClCon = (CLIENT_CONNECTION*)malloc(sizeof(CLIENT_CONNECTION));
    if (newClCon == NULL)
    {
        return CL_CON_ERROR_MEMORY;
    }

    // asteptam un nou client sa se conecteze (de 2 ori send/receive)
    newClCon->SendData = NULL;
    CM_ERROR error;
    error = AwaitNewClient(Server->CMServer, &newClCon->SendData);
    if (CM_IS_ERROR(error))
    {
        free(newClCon);
        return CL_CON_ERROR_AWAIT;
    }

    newClCon->ReceiveData = NULL;
    error = AwaitNewClient(Server->CMServer, &newClCon->ReceiveData);
    if (CM_IS_ERROR(error))
    {
        AbandonClient(newClCon->SendData);
        free(newClCon);
        return CL_CON_ERROR_AWAIT;
    }
    CM_LOG_INFO(TEXT("New client has connected"));

    newClCon->IsUserLoggedIn = 0;
    wcscpy(newClCon->Username, TEXT(""));

    error = CreateDataBuffer(&newClCon->SendDataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        AbandonClient(newClCon->ReceiveData);
        AbandonClient(newClCon->SendData);
        free(newClCon);
        return CL_CON_ERROR_MEMORY;
    }

    error = CreateDataBuffer(&newClCon->ReceiveDataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        DestroyDataBuffer(newClCon->SendDataBuffer);
        AbandonClient(newClCon->ReceiveData);
        AbandonClient(newClCon->SendData);
        free(newClCon);
        return CL_CON_ERROR_MEMORY;
    }

    _itow(gIdCount, newClCon->IdClient, 10);
    gIdCount++;
    error = HtSetKeyValue(Server->TableClients, newClCon->IdClient, newClCon);
    if (error == -1)
    {
        CM_LOG_INFO(TEXT("AM AJUNS AICI"));
        DestroyDataBuffer(newClCon->ReceiveDataBuffer);
        DestroyDataBuffer(newClCon->SendDataBuffer);
        AbandonClient(newClCon->ReceiveData);
        AbandonClient(newClCon->SendData);
        free(newClCon);
        return CL_CON_ERROR_MEMORY;
    }

    newClCon->SendDataSemaphore = CreateSemaphoreW(NULL, 1, 1, newClCon->IdClient);
    if (newClCon->SendDataSemaphore == NULL)
    {
        CM_LOG_INFO(TEXT("Probleme cu mutexul de trimis date"));

        DestroyDataBuffer(newClCon->ReceiveDataBuffer);
        DestroyDataBuffer(newClCon->SendDataBuffer);
        AbandonClient(newClCon->ReceiveData);
        AbandonClient(newClCon->SendData);
        free(newClCon);

        return CL_CON_ERROR_MEMORY;
    }

    InitializeSRWLock(&newClCon->IsUserLoggedInMutex);
    InitializeSRWLock(&newClCon->IdClientMutex);
    InitializeSRWLock(&newClCon->UsernameMutex);

    *PClCon = newClCon;
    return CL_CON_SUCCESS;
}
CL_CON_ERROR DestroyClientConnection(CLIENT_CONNECTION* ClCon)
{
    CloseHandle(ClCon->SendDataSemaphore);
    DestroyDataBuffer(ClCon->ReceiveDataBuffer);
    DestroyDataBuffer(ClCon->SendDataBuffer);
    AbandonClient(ClCon->ReceiveData);
    AbandonClient(ClCon->SendData);
    free(ClCon);
    return CL_CON_SUCCESS;
}
CL_CON_ERROR SendMessageToClient(CLIENT_CONNECTION* ClCon, const wchar_t* Message)
{
    // copiem textul in buffer
    CM_ERROR error;
    SIZE_T messageLength = (wcslen(Message) + 1) * sizeof(wchar_t);
    
    //_tprintf_s(TEXT("AICI iainte de wait\n"));
    //_tprintf_s(ClCon->IdClient);
    //WaitForSingleObject(ClCon->SendDataMutex, INFINITE);
    //_tprintf_s(TEXT("AICI dupa de wait\n"));

    error = CopyDataIntoBuffer(ClCon->SendDataBuffer, (const CM_BYTE*)Message, (CM_SIZE)messageLength);
    if (CM_IS_ERROR(error))
    {
        CM_LOG_ERROR(TEXT("CopyDataIntoBuffer failed with err-code=0x%X!\n"), error);
        //ReleaseMutex(ClCon->SendDataMutex);
        return CL_CON_ERROR_MEMORY;
    }

    // trimitem informatia clientului
    CM_SIZE sentByteCount = 0;
    WaitForSingleObject(ClCon->SendDataSemaphore, INFINITE);
    error = SendDataToClient(ClCon->SendData, ClCon->SendDataBuffer, &sentByteCount);
    //ReleaseMutex(ClCon->SendDataMutex);

    CM_LOG_INFO(TEXT("SENT %d BYTES"), sentByteCount);
    if (CM_IS_ERROR(error))
    {
        return CL_CON_ERROR_SENT;
    }
    //Sleep(10);
    return CL_CON_SUCCESS;
}
CL_CON_ERROR ReceiveMessageFromClient(CLIENT_CONNECTION* ClCon)
{
    CM_SIZE receivedByteCount = 0;
    CM_ERROR error;
    error = ReceiveDataFromClient(ClCon->ReceiveData, ClCon->ReceiveDataBuffer, &receivedByteCount);
    CM_LOG_INFO(TEXT("RECEIVED %d BYTES"), receivedByteCount);
    if (CM_IS_ERROR(error))
    {
        return CL_CON_ERROR_RECEIVED;
    }
    return CL_CON_SUCCESS;
}
wchar_t* ReadMessageFromClient(CLIENT_CONNECTION* ClCon)
{
    return (wchar_t*) ClCon->ReceiveDataBuffer->DataBuffer;
}





/*
CONS_ERROR CreateConnectionServer(CONNECTION_SERVER** ConS, int argc, TCHAR* argv[])
{
    // preluam argumentele
    if (argc < 2)
        return CONS_ERROR_INVALID;
    
    const int NO_MAX_USERS = (int)_tcstoi64(argv[1], NULL, 10);
    if (NO_MAX_USERS == _I64_MIN || NO_MAX_USERS == _I64_MAX || NO_MAX_USERS <= 0)
        return CONS_ERROR_INVALID;
    
    CM_LOG_INFO(TEXT("Numarul maxim de useri ce se pot loga: %d"), NO_MAX_USERS);



    // initializam modulul de comunicare
    CM_ERROR error = InitCommunicationModule();
    if (CM_IS_ERROR(error))
        return CONS_ERROR_MODULE;



    // cream serverul
    CM_SERVER* server = NULL;
    error = CreateServer(&server);
    if (CM_IS_ERROR(error))
    {
        UninitCommunicationModule();
        return CONS_ERROR_SERVER;
    }
    CM_LOG_INFO(TEXT("Server is Up & Running"));


    
    // alocam memorie pentru conexiune
	CONNECTION_SERVER* newConS = NULL;
	newConS = (CONNECTION_SERVER*)malloc(sizeof(CONNECTION_SERVER));
    if (newConS == NULL)
    {
        DestroyServer(server);
        UninitCommunicationModule();
		return CONS_ERROR_MEMORY;
    }
    newConS->NO_MAX_USERS = NO_MAX_USERS;
    newConS->NoActiveUsers = 0;
    newConS->Server = server;
    HtCreate(&newConS->TableUsers);

    *ConS = newConS;

    return CONS_SUCCESS;
}
CONS_ERROR DeleteConnectionServer(CONNECTION_SERVER* ConS)
{
    if (ConS != NULL)
    {
        DestroyServer(ConS->Server);
        HtDestroy(&ConS->TableUsers);
    }
    free(ConS);
    UninitCommunicationModule();

    return CONS_SUCCESS;

}
*/
/*
CCS_ERROR CreateCCS(COMMUNICATION_CHANNEL_SERVER** PCCS, CM_SERVER* Server, CM_SERVER_CLIENT* Client)
{
    COMMUNICATION_CHANNEL_SERVER* newCCS = NULL;
    newCCS = (COMMUNICATION_CHANNEL_SERVER*)malloc(sizeof(COMMUNICATION_CHANNEL_SERVER));
    if (newCCS == NULL)
        return CCS_ERROR_MEMORY;
    newCCS->Client = Client;
    newCCS->Server = Server;
    newCCS->IsLogged = 0;
    wcscpy(newCCS->username, TEXT(""));

    CM_ERROR error;
    error = CreateDataBuffer(&newCCS->DataBuffer, MAX_BUFFER_SIZE);
    if (CM_IS_ERROR(error))
    {
        free(newCCS);
        return CCS_ERROR_MEMORY;
    }
    *PCCS = newCCS;

    return CCS_SUCCESS;
}
CCS_ERROR SendMessageToClient(COMMUNICATION_CHANNEL_SERVER* CCS, const wchar_t* Message)
{
    // copiem textul in buffer
    CM_ERROR error;
    SIZE_T messageLength = (wcslen(Message) + 1) * sizeof(wchar_t);
    
    error = CopyDataIntoBuffer(CCS->DataBuffer, (const CM_BYTE*)Message, (CM_SIZE)messageLength);
    if (CM_IS_ERROR(error))
    {
        CM_LOG_ERROR(TEXT("CopyDataIntoBuffer failed with err-code=0x%X!\n"), error);
        return CCS_ERROR_MEMORY;
    }
    
    
    
    // trimitem informatia clientului
    CM_SIZE sentByteCount = 0;
    error = SendDataToClient(CCS->Client, CCS->DataBuffer, &sentByteCount);
    
    CM_LOG_INFO(TEXT("SENT %d BYTES"), sentByteCount);
    if (CM_IS_ERROR(error))
        return CCS_ERROR_SENT;



    return CCS_SUCCESS;
}
CCS_ERROR ReceiveMessageFromClient(COMMUNICATION_CHANNEL_SERVER* CCS)
{
    CM_SIZE receivedByteCount = 0;
    CM_ERROR error;
    error = ReceiveDataFromClient(CCS->Client, CCS->DataBuffer, &receivedByteCount);
    CM_LOG_INFO(TEXT("RECEIVED %d BYTES"), receivedByteCount);
    if (CM_IS_ERROR(error))
        return CCS_ERROR_RECEIVED;
    return CCS_SUCCESS;
}
CM_BYTE* ReadMessage(COMMUNICATION_CHANNEL_SERVER* CCS)
{
    return CCS->DataBuffer->DataBuffer;
}
CCS_ERROR DeleteCCS(COMMUNICATION_CHANNEL_SERVER* CCS)
{
    if (CCS != NULL && CCS->DataBuffer != NULL)
        DestroyDataBuffer(CCS->DataBuffer);
    if (CCS != NULL)
        free(CCS);
    return CCS_SUCCESS;
}
*/