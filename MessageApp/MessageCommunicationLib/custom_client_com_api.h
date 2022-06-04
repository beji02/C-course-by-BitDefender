#pragma once

#include "communication_api.h"
#include <stdio.h>
#include <windows.h>
#include "communication_logging.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE (1000 * sizeof(wchar_t))


typedef struct _SERVER_CONNECTION
{
    HANDLE ReceiveDataSemaphore;
    wchar_t IdClient[MAX_BUFFER_SIZE];
    CM_CLIENT* SendData;
    CM_CLIENT* ReceiveData;
    CM_DATA_BUFFER* SendDataBuffer;
    CM_DATA_BUFFER* ReceiveDataBuffer;
}SERVER_CONNECTION;

typedef long SV_CON_ERROR;
#define SV_CON_SUCCESS 0
#define SV_CON_ERROR_MEMORY 1
#define SV_CON_ERROR_MODULE 2
#define SV_CON_ERROR_SERVER_DOWN 3
#define SV_CON_ERROR_RECEIVED 4
#define SV_CON_ERROR_SENT 5


SV_CON_ERROR CreateServerConnection(SERVER_CONNECTION** PSvCon);
SV_CON_ERROR DestroyServerConnection(SERVER_CONNECTION* SvCon);
SV_CON_ERROR ReceiveMessageFromServer(SERVER_CONNECTION* SvCon);
wchar_t* ReadMessageFromServer(SERVER_CONNECTION* SvCon);
SV_CON_ERROR SendMessageToServer(SERVER_CONNECTION* SvCon, const wchar_t* Message);

