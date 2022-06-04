#include <windows.h>
#include "stdafx.h"
#include "communication_logging.h"
#include "communication_api.h"
#include "custom_client_com_api.h"
#include <stdio.h>
#include <wchar.h>


SV_CON_ERROR SolveUserCommand(SERVER_CONNECTION* SvCon)
{
    wchar_t line[MAX_BUFFER_SIZE];
    _getws_s(line, MAX_BUFFER_SIZE);
    if (line[0] == '\0')
    {
        return !SV_CON_SUCCESS;
    }

    SV_CON_ERROR error;
    error = SendMessageToServer(SvCon, line);
    if (error == SV_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: no memory\n"));
        return error;
    }
    else if (error == SV_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: server closed\n"));
        return error;
    }
    
    return SV_CON_SUCCESS;
}

DWORD WINAPI ReceiveSpam(LPVOID lpParam)
{
    SERVER_CONNECTION* svCon = (SERVER_CONNECTION*)lpParam;
    wchar_t* messageUNEXPECTED = TEXT("S-a oprit\n");
   
    while (TRUE)
    {
        SV_CON_ERROR error = ReceiveMessageFromServer(svCon);
        if (error == SV_CON_ERROR_RECEIVED)
        {
            _tprintf_s(messageUNEXPECTED);
            return 0;
        }
        wchar_t* message = (wchar_t*)ReadMessageFromServer(svCon);
        _tprintf_s(message);
    }
    return 0;
}

DWORD WINAPI ReceiveResults(LPVOID lpParam)
{
    SERVER_CONNECTION* svCon = (SERVER_CONNECTION*)lpParam;
    //_tprintf_s(svCon->IdClient);
    //wchar_t* messageUNEXPECTED = TEXT("S-a oprit\n");

    while (TRUE)
    {
        SV_CON_ERROR error = ReceiveMessageFromServer(svCon);
        if (error == SV_CON_ERROR_RECEIVED)
        {
            break;
        }
        wchar_t* message = (wchar_t*)ReadMessageFromServer(svCon);
        _tprintf_s(message);
        ReleaseSemaphore(svCon->ReceiveDataSemaphore, 1, NULL);
    }

    CloseHandle(svCon->ReceiveDataSemaphore);
    DestroyServerConnection(svCon);
    ExitProcess(0);
}

int _tmain()
{
    //EnableCommunicationModuleLogger();

    // declaram niste mesaje comune
    wchar_t* messageUNEXPECTED = TEXT("Unexpected error: Could not connect to server\n");
    wchar_t* messageMAX = TEXT("Error: maximum concurrent connection count reached\n");
    wchar_t* messageDOWN = TEXT("Error: no running server found\n");
    wchar_t* messageSUCCES = TEXT("Successful connection\n");

    // cream conexiunea dubla (send/receive) client-server
    SERVER_CONNECTION* svCon = NULL;
    SV_CON_ERROR error;
    error = CreateServerConnection(&svCon);
    if (error == SV_CON_ERROR_MODULE || error == SV_CON_ERROR_MEMORY)
    {
        _tprintf_s(messageUNEXPECTED);
        return -1;
    }
    else if (error == SV_CON_ERROR_SERVER_DOWN)
    {
        _tprintf_s(messageDOWN);
        return -1;
    }
    
    // verificam transmiterea de mesaje intre server si client
    error = ReceiveMessageFromServer(svCon);
    if (error == SV_CON_ERROR_RECEIVED)
    {
        _tprintf_s(messageUNEXPECTED);
        DestroyServerConnection(svCon);
        return -1;
    }
    
    wchar_t* message = (wchar_t*)ReadMessageFromServer(svCon);
    if (wcscmp(message, messageMAX) == 0)
    {
        _tprintf_s(messageMAX);
        DestroyServerConnection(svCon);
        return -1;
    }
    
    wcscpy(svCon->IdClient, message);
    svCon->ReceiveDataSemaphore = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, 0, svCon->IdClient);
    if (svCon->ReceiveDataSemaphore == NULL)
    {
        _tprintf_s(messageUNEXPECTED);
        DestroyServerConnection(svCon);
        return -1;
    }
    _tprintf_s(messageSUCCES);
    ReleaseSemaphore(svCon->ReceiveDataSemaphore, 1, NULL);

    CreateThread(NULL, 0, ReceiveResults, (void*)svCon, 0, NULL);

    // loop in care preiau comenzile de la utilizator
    while (TRUE)
    {
        error = SolveUserCommand(svCon);
        if (error != SV_CON_SUCCESS)
        {
            break;
        }
    }
    CloseHandle(svCon->ReceiveDataSemaphore);
    DestroyServerConnection(svCon);
    return 0;
}
