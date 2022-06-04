// MessageServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

// communication library
#include "communication_api.h"
#include <stdio.h>
#include <windows.h>
#include "communication_logging.h"
#include "custom_server_com_api.h"

#define PATH_TO_REGISTRATION "C:\\registration.txt"
#define PATH_TO_INBOX "C:\\Inbox"

SERVER* gServer = NULL;

CL_CON_ERROR SolveClientEcho(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageServer[MAX_BUFFER_SIZE];
    wchar_t messageClient[MAX_BUFFER_SIZE];

    wcscpy(messageServer, Text);
    wcscat(messageServer, TEXT("\n"));
  
    wcscpy(messageClient, messageServer);

    CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }

    _tprintf_s(messageServer);
    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientRegister(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];

    wchar_t* buffer = NULL;
    wchar_t* token = NULL;
    
    token = wcstok(Text, TEXT(" \n"), &buffer);
    wchar_t username[MAX_BUFFER_SIZE];
    wcscpy(username, token);

    token = wcstok(NULL, TEXT("\n"), &buffer);

    wchar_t password[MAX_BUFFER_SIZE];
    wcscpy(password, token);
    

    for (int i = 0; username[i] != '\0'; i++)
    {
        if (!((username[i] >= 'a' && username[i] <= 'z') ||
            (username[i] >= 'A' && username[i] <= 'Z') ||
            (username[i] >= '0' && username[i] <= '9')))
        {
            wcscpy(messageClient, TEXT("Error: Invalid username\n"));

            CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
            if (error == CL_CON_ERROR_SENT)
            {
                _tprintf_s(TEXT("Unexpected error: client closed\n"));
                return error;
            }
            else if (error == CL_CON_ERROR_MEMORY)
            {
                _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
                return error;
            }

            return CL_CON_SUCCESS;
        }
    }

    int condPassLitMare = 0;
    int condPassSymbolNealfa = 0;
    int condPassSpatiu = 1;
    int condPassVirgula = 1;
    int condPassLength = wcslen(password) >= 5 ? 1 : 0;
    
    for (int i = 0; password[i] != '\0'; i++)
    {
        if (password[i] >= 'A' && password[i] <= 'Z')
        {
            condPassLitMare = 1;
        }
        if (!iswalnum(password[i]))
        {
            condPassSymbolNealfa = 1;
        }
        if (password[i] == ' ')
        {
            condPassSpatiu = 0;
        }
        if (password[i] == ',')
        {
            condPassVirgula = 0;
        }
    }
    if (!(condPassSpatiu && condPassVirgula))
    {
        wcscpy(messageClient, TEXT("Error: Invalid password\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }
    if(!(condPassLitMare && condPassSymbolNealfa && condPassLength))
    {
        wcscpy(messageClient, TEXT("Error: Password too weak\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }
    

    FILE* file;

    // TREBUIE ADMIN RIGHTS TO DO IT IN C:
    // NOT THREAD SAFE
    
    HANDLE aux = CreateMutex(NULL, 1, TEXT(PATH_TO_REGISTRATION));
    file = fopen(PATH_TO_REGISTRATION, "a+");

    if (file == NULL)
    {
        ReleaseMutex(aux);
        wcscpy(messageClient, TEXT("Unexpected error: Cannot open registration.txt\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    wchar_t line[MAX_BUFFER_SIZE];
    int condExistsAlready = 0;
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        buffer = NULL;
        token = NULL;

        token = wcstok(line, TEXT(","), &buffer);

        if (wcscmp(token, username) == 0)
        {
            condExistsAlready = 1;
            break;
        }
    }

    if (condExistsAlready)
    {
        fclose(file);
        ReleaseMutex(aux);

        wcscpy(messageClient, TEXT("Error: Username already registered\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    
    if (ClCon->IsUserLoggedIn == 1)
    {
        fclose(file);
        ReleaseMutex(aux);

        wcscpy(messageClient, TEXT("Error: User already logged in\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }


    wchar_t newRegistryLine[MAX_BUFFER_SIZE];
    wcscpy(newRegistryLine, username);
    wcscat(newRegistryLine, TEXT(","));
    wcscat(newRegistryLine, password);
    wcscat(newRegistryLine, TEXT("\n"));
    fputws(newRegistryLine, file);
    fclose(file);
    ReleaseMutex(aux);
    
    wcscpy(messageClient, TEXT("Success\n"));

    CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }

    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientLogin(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];

    wchar_t* buffer = NULL;
    wchar_t* token = NULL;

    token = wcstok(Text, TEXT(" \n"), &buffer);
    wchar_t username[MAX_BUFFER_SIZE];
    wcscpy(username, token);

    token = wcstok(NULL, TEXT("\n"), &buffer);

    wchar_t password[MAX_BUFFER_SIZE];
    wcscpy(password, token);


       
    FILE* file;

    // TREBUIE ADMIN RIGHTS TO DO IT IN C:
    // NOT THREAD SAFE

    HANDLE aux = CreateMutex(NULL, 1, TEXT(PATH_TO_REGISTRATION));
    file = fopen(PATH_TO_REGISTRATION, "a+");

    if (file == NULL)
    {
        ReleaseMutex(aux);
        wcscpy(messageClient, TEXT("Unexpected error: Cannot open registration.txt\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    wchar_t line[MAX_BUFFER_SIZE];
    int condExists = 0;
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        buffer = NULL;
        token = NULL;

        token = wcstok(line, TEXT(","), &buffer);

        if (wcscmp(token, username) == 0)
        {
            token = wcstok(NULL, TEXT("\n"), &buffer);
            if (wcscmp(token, password) == 0)
            {
                condExists = 1;
                break;
            }
        }
    }
    fclose(file);
    ReleaseMutex(aux);

    if (condExists == 0)
    {
        wcscpy(messageClient, TEXT("Error: Invalid username/password combination\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 1)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

        wcscpy(messageClient, TEXT("Error: Another user already logged in\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }    
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

    AcquireSRWLockShared(&gServer->TableUsersRWL);
    if (HtHasKey(gServer->TableUsers, username) == 1)
    {
        ReleaseSRWLockShared(&gServer->TableUsersRWL);
        wcscpy(messageClient, TEXT("Error: User already logged in\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    ReleaseSRWLockShared(&gServer->TableUsersRWL);

    wcscpy(ClCon->Username, username);
    
    AcquireSRWLockExclusive(&gServer->TableUsersRWL);
    AcquireSRWLockShared(&ClCon->UsernameMutex);
    HtSetKeyValue(gServer->TableUsers, ClCon->Username, ClCon);
    ReleaseSRWLockExclusive(&gServer->TableUsersRWL);
    ReleaseSRWLockShared(&ClCon->UsernameMutex);
    
    AcquireSRWLockExclusive(&ClCon->IsUserLoggedInMutex);
    ClCon->IsUserLoggedIn = 1;
    ReleaseSRWLockExclusive(&ClCon->IsUserLoggedInMutex);
    
    wcscpy(messageClient, TEXT("Success\n"));

    CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }


    wchar_t pathInbox[MAX_BUFFER_SIZE];
    wchar_t pathCurrentUser[MAX_BUFFER_SIZE];

    wcscpy(pathInbox, TEXT(PATH_TO_INBOX));
    CreateDirectoryW(pathInbox, NULL);
    
    wcscpy(pathCurrentUser, pathInbox);
    wcscat(pathCurrentUser, TEXT("\\"));
    wcscat(pathCurrentUser, ClCon->Username);
    CreateDirectoryW(pathCurrentUser, NULL);


    // deschis fisierul .txt 
    char pathToFile[MAX_BUFFER_SIZE];
    wcstombs(pathToFile, pathCurrentUser, sizeof(pathToFile));

    strcat(pathToFile, "\\Unread.txt");
    HANDLE pathToFileMutex = CreateMutex(NULL, 1, pathCurrentUser);
    file = fopen(pathToFile, "a+");
    
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        error = SendMessageToClient(ClCon, line);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
    }
    fclose(file);
    file = fopen(pathToFile, "w");
    fclose(file);
    ReleaseMutex(pathToFileMutex);

    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientLogout(CLIENT_CONNECTION* ClCon)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];

    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 0)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

        wcscpy(messageClient, TEXT("Error: No user currently logged in\n"));

        CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);
    


    /*if (HtHasKey(ConS->TableUsers, CCS->username) < 1)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return !CCS_SUCCESS;
    }*/
    //_tprintf_s(TEXT("AM AJUNS AICI\n"));
    AcquireSRWLockExclusive(&gServer->TableUsersRWL);
    //AcquireSRWLockExclusive(&ClCon->UsernameMutex);
    HtRemoveKey(gServer->TableUsers, ClCon->Username);
    //ReleaseSRWLockExclusive(&ClCon->UsernameMutex);
    ReleaseSRWLockExclusive(&gServer->TableUsersRWL);
   
    //_tprintf_s(TEXT("AM AJUNS AICI2\n"));

    AcquireSRWLockExclusive(&ClCon->IsUserLoggedInMutex);
    ClCon->IsUserLoggedIn = 0;
    ReleaseSRWLockExclusive(&ClCon->IsUserLoggedInMutex);
    
    AcquireSRWLockExclusive(&ClCon->UsernameMutex);
    wcscpy(ClCon->Username, TEXT(""));
    ReleaseSRWLockExclusive(&ClCon->UsernameMutex);
    wcscpy(messageClient, TEXT("Success\n"));
    
    CL_CON_ERROR error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }
    return CL_CON_SUCCESS;

}
CL_CON_ERROR SolveClientMsg(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];
    CL_CON_ERROR error;
    
    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 0)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

        wcscpy(messageClient, TEXT("Error: No user currently logged in\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }        
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

    wchar_t* buffer = NULL;
    wchar_t* token = NULL;

    token = wcstok(Text, TEXT(" \n"), &buffer);
    wchar_t username[MAX_BUFFER_SIZE];
    wcscpy(username, token);

    token = wcstok(NULL, TEXT("\n"), &buffer);
    wchar_t messageInFile[MAX_BUFFER_SIZE];
    wchar_t message[MAX_BUFFER_SIZE];

    wcscpy(message, TEXT("Message from "));
    wcscat(message, ClCon->Username);
    wcscat(message, TEXT(": "));
    wcscat(message, token);
    wcscat(message, TEXT("\n"));

    wcscpy(messageInFile, TEXT("From "));
    wcscat(messageInFile, ClCon->Username);
    wcscat(messageInFile, TEXT(": "));
    wcscat(messageInFile, token);
    wcscat(messageInFile, TEXT("\n"));


    FILE* file;

    // TREBUIE ADMIN RIGHTS TO DO IT IN C:
    // NOT THREAD SAFE
    HANDLE aux = CreateMutex(NULL, 1, TEXT(PATH_TO_REGISTRATION));
    file = fopen(PATH_TO_REGISTRATION, "a+");

    if (file == NULL)
    {
        ReleaseMutex(aux);
        wcscpy(messageClient, TEXT("Unexpected error: Cannot open registration.txt\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    wchar_t line[MAX_BUFFER_SIZE];
    int condExists = 0;
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        buffer = NULL;
        token = NULL;

        token = wcstok(line, TEXT(","), &buffer);

        if (wcscmp(token, username) == 0)
        {
            condExists = 1;
            break;
        }
    }
    fclose(file);
    ReleaseMutex(aux);

    if (condExists == 0)
    {
        wcscpy(messageClient, TEXT("Error: No such user\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }

    // adauga mesajul in folder (persistenta)
    
    // cream fisierul userului care trimite mesajul
    // cream in interior un nou .txt pt userul care primeste mesajul
    wchar_t pathInbox[MAX_BUFFER_SIZE];
    wchar_t pathCurrentUser[MAX_BUFFER_SIZE];
    wchar_t pathCurrentUserSendUser[MAX_BUFFER_SIZE];

    wcscpy(pathInbox, TEXT(PATH_TO_INBOX));
    CreateDirectoryW(pathInbox, NULL);
    
    wcscpy(pathCurrentUser, pathInbox);
    wcscat(pathCurrentUser, TEXT("\\"));
    wcscat(pathCurrentUser, ClCon->Username);
    CreateDirectoryW(pathCurrentUser, NULL);

    wcscpy(pathCurrentUserSendUser, pathCurrentUser);
    wcscat(pathCurrentUserSendUser, TEXT("\\"));
    wcscat(pathCurrentUserSendUser, username);
    wcscat(pathCurrentUserSendUser, TEXT(".txt"));

    // deschis fisierul .txt 
    char pathToFile[MAX_BUFFER_SIZE];
    wcstombs(pathToFile, pathCurrentUserSendUser, sizeof(pathToFile));

    HANDLE pathToFileMutex = CreateMutex(NULL, 1, pathCurrentUserSendUser);

    file = fopen(pathToFile, "a+");
    fputws(messageInFile, file);
    fclose(file);
    ReleaseMutex(pathToFileMutex);
    // end

    // creez si folderele userului care primeste mesajul
    wcscpy(pathCurrentUser, pathInbox);
    wcscat(pathCurrentUser, TEXT("\\"));
    wcscat(pathCurrentUser, username);
    CreateDirectoryW(pathCurrentUser, NULL);

    wcscpy(pathCurrentUserSendUser, pathCurrentUser);
    wcscat(pathCurrentUserSendUser, TEXT("\\"));
    wcscat(pathCurrentUserSendUser, ClCon->Username);
    wcscat(pathCurrentUserSendUser, TEXT(".txt"));

    wcstombs(pathToFile, pathCurrentUserSendUser, sizeof(pathToFile));

    pathToFileMutex = CreateMutex(NULL, 1, pathCurrentUserSendUser);
    file = fopen(pathToFile, "a+");
    fputws(messageInFile, file);
    fclose(file);
    ReleaseMutex(pathToFileMutex);

    wcscpy(messageClient, TEXT("Success\n"));

    error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }

    AcquireSRWLockShared(&gServer->TableUsersRWL);
    if (HtHasKey(gServer->TableUsers, username) < 1) // daca nu este logat
    {
        ReleaseSRWLockShared(&gServer->TableUsersRWL);

        wcstombs(pathToFile, pathCurrentUser, sizeof(pathToFile));
        strcat(pathToFile, "\\Unread.txt");
        
        pathToFileMutex = CreateMutex(NULL, 1, pathCurrentUser);
        file = fopen(pathToFile, "a+");
        fputws(message, file);
        fclose(file);
        ReleaseMutex(pathToFileMutex);

        //_tprintf_s(TEXT("Error: receiving user isnt logged in\n"));
        return CL_CON_SUCCESS;
    }
    ReleaseSRWLockShared(&gServer->TableUsersRWL);
    CLIENT_CONNECTION* clConToSend = NULL;

    AcquireSRWLockShared(&gServer->TableUsersRWL);
    error = HtGetKeyValue(gServer->TableUsers, username, &clConToSend);
    ReleaseSRWLockShared(&gServer->TableUsersRWL);

    if (error == -1)
    {
        _tprintf_s(TEXT("Unexpected error: not found in hash table\n"));
        return error;
    }

    error = SendMessageToClient(clConToSend, message);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }
   
    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientBroadcast(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageOthers[MAX_BUFFER_SIZE];
    wchar_t messageClient[MAX_BUFFER_SIZE];
    CL_CON_ERROR error;

    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 0)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

        wcscpy(messageClient, TEXT("Error: No user currently logged in\n"));
        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);


    wcscpy(messageOthers, TEXT("Broadcast from "));
    AcquireSRWLockShared(&ClCon->UsernameMutex);
    wcscat(messageOthers, ClCon->Username);
    ReleaseSRWLockShared(&ClCon->UsernameMutex);

    wcscat(messageOthers, TEXT(": "));
    wcscat(messageOthers, Text);
    wcscat(messageOthers, TEXT("\n"));

    wcscpy(messageClient, TEXT("Success\n"));

    error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }

    CC_HASH_TABLE_ITERATOR* it = NULL;
    wchar_t* idClientOther;
    CLIENT_CONNECTION* clConOther = NULL;

    AcquireSRWLockShared(&gServer->TableClientsRWL);
    error = HtGetFirstKey(gServer->TableClients, &it, &idClientOther);
    ReleaseSRWLockShared(&gServer->TableClientsRWL);

    if (error == -2)
    {
        return CL_CON_SUCCESS;
    }

    else if (error == -1)
    {
        return !CL_CON_SUCCESS;
    }

    AcquireSRWLockShared(&gServer->TableClientsRWL);
    do
    {
        ReleaseSRWLockShared(&gServer->TableClientsRWL);
        AcquireSRWLockShared(&gServer->TableClientsRWL);
        error = HtGetKeyValue(gServer->TableClients, idClientOther, &clConOther);
        ReleaseSRWLockShared(&gServer->TableClientsRWL);

        if (error == -1)
        {
            _tprintf_s(TEXT("Unexpected error: key doesnt exists in table\n"));
            return error;
        }
        AcquireSRWLockShared(&clConOther->IdClientMutex);
        AcquireSRWLockShared(&ClCon->IdClientMutex);
        if (wcscmp(clConOther->IdClient, ClCon->IdClient) == 0)
        {
            ReleaseSRWLockShared(&clConOther->IdClientMutex);
            ReleaseSRWLockShared(&ClCon->IdClientMutex);
            AcquireSRWLockShared(&gServer->TableClientsRWL);
            continue;
        }
        ReleaseSRWLockShared(&clConOther->IdClientMutex);
        ReleaseSRWLockShared(&ClCon->IdClientMutex);

        error = SendMessageToClient(clConOther, messageOthers);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        
        AcquireSRWLockShared(&gServer->TableClientsRWL);
    } while (HtGetNextKey(it, &idClientOther) >= 0);

    
    HtReleaseIterator(&it);
    ReleaseSRWLockShared(&gServer->TableClientsRWL);

    return CL_CON_SUCCESS;
}


CL_CON_ERROR SolveClientList(CLIENT_CONNECTION* ClCon)
{
    CC_HASH_TABLE_ITERATOR* it = NULL;
    wchar_t messageClient[MAX_BUFFER_SIZE];
    wchar_t* username;
    CL_CON_ERROR error;

    AcquireSRWLockShared(&gServer->TableUsersRWL);
    error = HtGetFirstKey(gServer->TableUsers, &it, &username);
    ReleaseSRWLockShared(&gServer->TableUsersRWL);

    if (error == -2)
    {
        return CL_CON_SUCCESS;
    }
    else if (error == -1)
    {
        return !CL_CON_SUCCESS;
    }
    
    AcquireSRWLockShared(&gServer->TableUsersRWL);
    do
    {
        ReleaseSRWLockShared(&gServer->TableUsersRWL);

        wcscpy(messageClient, username);
        wcscat(messageClient, TEXT("\n"));
        
        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        AcquireSRWLockShared(&gServer->TableUsersRWL);
    } while (HtGetNextKey(it, &username) >= 0);
    HtReleaseIterator(&it);
    ReleaseSRWLockShared(&gServer->TableUsersRWL);

    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientExit(CLIENT_CONNECTION* ClCon)
{
    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 1)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);

        AcquireSRWLockExclusive(&gServer->TableUsersRWL);
        HtRemoveKey(gServer->TableUsers, ClCon->Username);
        ReleaseSRWLockExclusive(&gServer->TableUsersRWL);
        
        AcquireSRWLockExclusive(&ClCon->IsUserLoggedInMutex);
        ClCon->IsUserLoggedIn = 0;
        ReleaseSRWLockExclusive(&ClCon->IsUserLoggedInMutex);

        AcquireSRWLockExclusive(&ClCon->UsernameMutex);
        wcscpy(ClCon->Username, TEXT(""));
        ReleaseSRWLockExclusive(&ClCon->UsernameMutex);
    }
    AcquireSRWLockExclusive(&gServer->TableClientsRWL);
    AcquireSRWLockShared(&ClCon->IdClientMutex);
    HtRemoveKey(gServer->TableClients, ClCon->IdClient);
    ReleaseSRWLockShared(&ClCon->IdClientMutex);
    ReleaseSRWLockExclusive(&gServer->TableClientsRWL);

    WaitForSingleObject(gServer->NoClientsMutex, INFINITE);
    gServer->NoClients--;
    ReleaseMutex(gServer->NoClientsMutex);
    DestroyClientConnection(ClCon);

    return !CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientHistory(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];
    CL_CON_ERROR error;

    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 0)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);
        wcscpy(messageClient, TEXT("Error: No user currently logged in\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }        
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);


    wchar_t* buffer = NULL;
    wchar_t* token = NULL;

    token = wcstok(Text, TEXT(" \n"), &buffer);
    wchar_t username[MAX_BUFFER_SIZE];
    wcscpy(username, token);

    token = wcstok(NULL, TEXT("\n"), &buffer);
    int count = (int)_tcstoi64(token, NULL, 10);
    if (count == _I64_MIN || count == _I64_MAX || count <= 0)
    {
        wcscpy(messageClient, TEXT("Unexpected error: count is not valid\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    FILE* file;

    // TREBUIE ADMIN RIGHTS TO DO IT IN C:
    // NOT THREAD SAFE
    HANDLE aux = CreateMutex(NULL, 1, TEXT(PATH_TO_REGISTRATION));
    file = fopen(PATH_TO_REGISTRATION, "a+");

    if (file == NULL)
    {
        ReleaseMutex(aux);
        wcscpy(messageClient, TEXT("Unexpected error: Cannot open registration.txt\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    wchar_t line[MAX_BUFFER_SIZE];
    int condExists = 0;
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        buffer = NULL;
        token = NULL;

        token = wcstok(line, TEXT(","), &buffer);

        if (wcscmp(token, username) == 0)
        {
            condExists = 1;
            break;
        }
    }
    fclose(file);
    ReleaseMutex(aux);


    if (condExists == 0)
    {
        wcscpy(messageClient, TEXT("Error: No such user\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }

    wchar_t pathCurrentUserSendUser[MAX_BUFFER_SIZE];

    wcscpy(pathCurrentUserSendUser, TEXT(PATH_TO_INBOX));
    wcscat(pathCurrentUserSendUser, TEXT("\\"));
    
    AcquireSRWLockShared(&ClCon->UsernameMutex);
    wcscat(pathCurrentUserSendUser, ClCon->Username);
    ReleaseSRWLockShared(&ClCon->UsernameMutex);

    wcscat(pathCurrentUserSendUser, TEXT("\\"));
    wcscat(pathCurrentUserSendUser, username);
    wcscat(pathCurrentUserSendUser, TEXT(".txt"));

    // deschis fisierul .txt 
    char pathToFile[MAX_BUFFER_SIZE];
    wcstombs(pathToFile, pathCurrentUserSendUser, sizeof(pathToFile));

    HANDLE pathToFileMutex = CreateMutex(NULL, 1, pathCurrentUserSendUser);
    file = fopen(pathToFile, "r");
    if (file == NULL)
    {
        ReleaseMutex(pathToFileMutex);
        // scrie linie goala
        wcscpy(messageClient, TEXT("\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    int kLines = 0;
    while(fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        kLines++;
    }
    //_tprintf_s(TEXT("%d\n"), kLines);

    fseek(file, 0, SEEK_SET);
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL && kLines > 0)
    {
        if (kLines <= count)
        {
            wcscpy(messageClient, line);

            error = SendMessageToClient(ClCon, messageClient);
            if (error == CL_CON_ERROR_SENT)
            {
                _tprintf_s(TEXT("Unexpected error: client closed\n"));
                return error;
            }
            else if (error == CL_CON_ERROR_MEMORY)
            {
                _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
                return error;
            }
            //Sleep(100);
        }
        kLines--;
    }
    fclose(file);
    ReleaseMutex(pathToFileMutex);

    return CL_CON_SUCCESS;
}
CL_CON_ERROR SolveClientSendfile(CLIENT_CONNECTION* ClCon, wchar_t* Text)
{
    wchar_t messageClient[MAX_BUFFER_SIZE];
    CL_CON_ERROR error;

    AcquireSRWLockShared(&ClCon->IsUserLoggedInMutex);
    if (ClCon->IsUserLoggedIn == 0)
    {
        ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);
        wcscpy(messageClient, TEXT("Error: No user currently logged in\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }
    ReleaseSRWLockShared(&ClCon->IsUserLoggedInMutex);


    wchar_t* buffer = NULL;
    wchar_t* token = NULL;

    token = wcstok(Text, TEXT(" \n"), &buffer);
    wchar_t username[MAX_BUFFER_SIZE];
    wcscpy(username, token);

    token = wcstok(NULL, TEXT("\n"), &buffer);

    wchar_t path[MAX_BUFFER_SIZE];
    wcscpy(path, token);

    FILE* file;

    // TREBUIE ADMIN RIGHTS TO DO IT IN C:
    // NOT THREAD SAFE    
    HANDLE aux = CreateMutex(NULL, 1, TEXT(PATH_TO_REGISTRATION));
    file = fopen(PATH_TO_REGISTRATION, "a+");

    if (file == NULL)
    {
        ReleaseMutex(aux);
        wcscpy(messageClient, TEXT("Unexpected error: Cannot open registration.txt\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }

    wchar_t line[MAX_BUFFER_SIZE];
    int condExists = 0;
    while (fgetws(line, MAX_BUFFER_SIZE, file) != NULL)
    {
        buffer = NULL;
        token = NULL;

        token = wcstok(line, TEXT(","), &buffer);

        if (wcscmp(token, username) == 0)
        {
            condExists = 1;
            break;
        }
    }
    fclose(file);
    ReleaseMutex(aux);

    if (condExists == 0)
    {
        wcscpy(messageClient, TEXT("Error: No such user\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }

    AcquireSRWLockShared(&gServer->TableUsersRWL);
    error = HtHasKey(gServer->TableUsers, username);
    ReleaseSRWLockShared(&gServer->TableUsersRWL);

    if (error < 1)
    {
        wcscpy(messageClient, TEXT("Error: User not active\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        return CL_CON_SUCCESS;
    }

    char pathToFile[MAX_BUFFER_SIZE];
    wcstombs(pathToFile, path, sizeof(pathToFile));

    HANDLE pathToFileMutex = CreateMutex(NULL, 1, path);
    file = fopen(pathToFile, "r");
    if (file == NULL)
    {
        ReleaseMutex(pathToFileMutex);
        // scrie linie goala
        wcscpy(messageClient, TEXT("Error: File not found\n"));

        error = SendMessageToClient(ClCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }

        return CL_CON_SUCCESS;
    }
    fclose(file);
    ReleaseMutex(pathToFileMutex);

    wcscpy(messageClient, TEXT("Unexpected error: unfortunately this functionality isn't fully implemented\n"));

    error = SendMessageToClient(ClCon, messageClient);
    if (error == CL_CON_ERROR_SENT)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return error;
    }
    else if (error == CL_CON_ERROR_MEMORY)
    {
        _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
        return error;
    }

    return CL_CON_SUCCESS;
}


CL_CON_ERROR SolveClientCommand(CLIENT_CONNECTION* ClCon)
{
    wchar_t* commandECHO = TEXT("echo");
    wchar_t* commandREG = TEXT("register");
    wchar_t* commandLOGIN = TEXT("login");
    wchar_t* commandLOGOUT = TEXT("logout");
    wchar_t* commandMSG = TEXT("msg");
    wchar_t* commandBROADCAST = TEXT("broadcast");    
    wchar_t* commandLIST = TEXT("list");
    wchar_t* commandEXIT = TEXT("exit");
    wchar_t* commandHISTORY = TEXT("history");
    wchar_t* commandSENDFILE = TEXT("sendfile"); // not fully implemented

    // citim cererea
    CL_CON_ERROR error;
    error = ReceiveMessageFromClient(ClCon);
    if (error == CL_CON_ERROR_RECEIVED)
    {
        _tprintf_s(TEXT("Unexpected error: client closed\n"));
        return !CL_CON_SUCCESS;
    }
    wchar_t* message = (wchar_t*)ReadMessageFromClient(ClCon);

    // luam comanda
    wchar_t* buffer = NULL;
    wchar_t* token = NULL;
    token = wcstok(message, TEXT(" \n"), &buffer);

    if (wcscmp(token, commandECHO) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientEcho(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandREG) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientRegister(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandLOGIN) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientLogin(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandLOGOUT) == 0)
    {
        error = SolveClientLogout(ClCon);
        return error;
    }
    else if (wcscmp(token, commandMSG) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientMsg(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandBROADCAST) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientBroadcast(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandLIST) == 0)
    {
        error = SolveClientList(ClCon);
        return error;
    }
    else if (wcscmp(token, commandEXIT) == 0)
    {
        error = SolveClientExit(ClCon);
        return error;
    }
    else if (wcscmp(token, commandHISTORY) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientHistory(ClCon, token);
        return error;
    }
    else if (wcscmp(token, commandSENDFILE) == 0)
    {
        token = wcstok(NULL, TEXT("\n"), &buffer);
        error = SolveClientSendfile(ClCon, token);
        return error;
    }
    else
    {
        return CL_CON_SUCCESS;
    }
}

DWORD WINAPI UserThreadTestSend(LPVOID lpParam)
{
    CLIENT_CONNECTION* clCon = (CLIENT_CONNECTION*)lpParam;
    wchar_t* messageClient = TEXT("SPAMMING...\n");
    while (TRUE)
    {
        CL_CON_ERROR error;
        error = SendMessageToClient(clCon, messageClient);
        if (error == CL_CON_ERROR_SENT)
        {
            _tprintf_s(TEXT("Unexpected error: client closed\n"));
            return error;
        }
        else if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(TEXT("Unexpected error: not enough memory\n"));
            return error;
        }
        //Sleep(1000);
    }
    return 0;
}

DWORD WINAPI UserThreadFunction(LPVOID lpParam)
{
    CM_LOG_INFO(TEXT("~SERVER~ Mesaj din thread nou inceput"));

    // preluam parametrii
    CLIENT_CONNECTION* clCon = (CLIENT_CONNECTION*)lpParam;

    while (TRUE)
    {
        CL_CON_ERROR error;
        error = SolveClientCommand(clCon);
        if (error != CL_CON_SUCCESS)
        {
            break;
        }
    }

    CM_LOG_INFO(TEXT("~SERVER~ Mesaj din thread nou final"));

    // TO DO DEALLOCATE PARAMS
    return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    //EnableCommunicationModuleLogger();

    wchar_t* messageMAX = TEXT("Error: maximum concurrent connection count reached\n");
    wchar_t* messageINVALID = TEXT("Error: invalid maximum number of connections\n");
    wchar_t* messageUNEXPECTED_SERVER = TEXT("Unexpected error: Failed to create server\n");
    wchar_t* messageUNEXPECTED_CLIENT = TEXT("Unexpected error: Failed to wait for client\n");
    wchar_t* messageUNEXPECTED_CC = TEXT("Unexpected error: Failed to create com channel\n");
    wchar_t* messageSUCCES_SERVER = TEXT("Success\n");

    SV_ERROR error = 0;
    error = CreateServer(&gServer, argc, argv);
    if (error == SV_ERROR_INVALID)
    {
        _tprintf_s(messageINVALID);
        return -1;
    }
    else
    if(error == SV_ERROR_MODULE || error == SV_ERROR_CM || error == SV_ERROR_MEMORY)
    {
        _tprintf_s(messageUNEXPECTED_SERVER);
        return -1;
    }
    _tprintf_s(messageSUCCES_SERVER);

    while (TRUE)
    {
        CLIENT_CONNECTION* clCon = NULL;
        CreateClientConnection(&clCon, gServer);
        //_tprintf_s(clCon->IdClient);
        if (error == CL_CON_ERROR_MEMORY)
        {
            _tprintf_s(messageUNEXPECTED_CC);
            //DestroyServer(Server);
            //return -1;
            continue;
        }
        else if (error == CL_CON_ERROR_AWAIT)
        {
            _tprintf_s(messageUNEXPECTED_CLIENT);
            //DestroyServer(Server);
            continue;
        }
        CM_LOG_INFO(TEXT("New client has connected"));
        
        if (gServer->NoClients >= gServer->NO_MAX_CLIENTS)
        {
            SendMessageToClient(clCon, messageMAX);
            DestroyClientConnection(clCon);
            
            continue;
        }

        error = SendMessageToClient(clCon, clCon->IdClient);
        if (error == CL_CON_ERROR_SENT || error == CL_CON_ERROR_MEMORY)
        {
            DestroyClientConnection(clCon);

            continue;
        }
        
        // cream un nou thread care serveste userul 
        DWORD threadIdentifier;
        HANDLE threadHandle = CreateThread(NULL, 0, UserThreadFunction, (void*)clCon, 0, &threadIdentifier);
        if (threadHandle == NULL)
        {
            _tprintf_s(TEXT("Unexpected error: Failed to create thread\n"));
            DestroyClientConnection(clCon);

            continue;
        }
        /*
        CreateThread(NULL, 0, UserThreadTestSend, (void*)clCon, 0, &threadIdentifier);
        if (threadHandle == NULL)
        {
            _tprintf_s(TEXT("Unexpected error: Failed to create thread\n"));
            DestroyClientConnection(clCon);

            continue;
        }
        */
        
        WaitForSingleObject(gServer->NoClientsMutex, INFINITE);
        gServer->NoClients ++; 
        ReleaseMutex(gServer->NoClientsMutex);
    }

    DestroyServer(gServer);
    return 0;
}

