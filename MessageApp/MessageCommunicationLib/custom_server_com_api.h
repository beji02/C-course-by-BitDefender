#pragma once

#include "communication_api.h"
#include <stdio.h>
#include <windows.h>
#include "communication_logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "communication_logging.h"
#include "cchashtable.h"
#define MAX_BUFFER_SIZE (1000 * sizeof(wchar_t))


typedef struct _SERVER
{
	int NO_MAX_CLIENTS;
	// 1. we need to protect atribute NoClients with read/write (within proccess)
	// 2. we need to protect atribute TableUsers with read/write (within proccess)
	// 3. we need to protect atribute TableClients with read/write (within proccess)
	HANDLE NoClientsMutex;
	SRWLOCK TableUsersRWL;
	SRWLOCK TableClientsRWL;
	int NoClients;
	CC_HASH_TABLE* TableUsers;
	CC_HASH_TABLE* TableClients;
	CM_SERVER* CMServer;
}SERVER;

typedef long SV_ERROR;
#define SV_SUCCESS 0
#define SV_ERROR_MODULE 1
#define SV_ERROR_INVALID 2
#define SV_ERROR_MEMORY 3
#define SV_ERROR_CM 4

SV_ERROR CreateServer(SERVER** PServer, int Argc, TCHAR* Argv[]);
SV_ERROR DestroyServer(SERVER* Server);


typedef struct _CLIENT_CONNECTION
{
	// 4. we need to protect atribute SendDataBuffer and SendData with mutex (within proccess)
	// 5. we need to protect atribute IdClient with mutex (within proccess)
	// 6. we need to protect atribute Username with mutex (within proccess)
	CM_SERVER_CLIENT* SendData;
	CM_SERVER_CLIENT* ReceiveData;
	CM_DATA_BUFFER* SendDataBuffer;
	CM_DATA_BUFFER* ReceiveDataBuffer;
	HANDLE SendDataSemaphore;
	SRWLOCK IsUserLoggedInMutex;
	SRWLOCK IdClientMutex;
	SRWLOCK UsernameMutex;
	int IsUserLoggedIn;
	wchar_t IdClient[MAX_BUFFER_SIZE];
	wchar_t Username[MAX_BUFFER_SIZE];
}CLIENT_CONNECTION;


typedef long CL_CON_ERROR;
#define CL_CON_SUCCESS 0
#define CL_CON_ERROR_MEMORY 1
#define CL_CON_ERROR_SENT 2
#define CL_CON_ERROR_RECEIVED 3
#define CL_CON_ERROR_AWAIT 4


CL_CON_ERROR CreateClientConnection(CLIENT_CONNECTION** PClCon, SERVER* Server);
CL_CON_ERROR SendMessageToClient(CLIENT_CONNECTION* ClCon, const wchar_t* Message);
CL_CON_ERROR ReceiveMessageFromClient(CLIENT_CONNECTION* ClCon);
wchar_t* ReadMessageFromClient(CLIENT_CONNECTION* ClCon);
CL_CON_ERROR DestroyClientConnection(CLIENT_CONNECTION* ClCon);




