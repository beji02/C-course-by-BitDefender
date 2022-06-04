#pragma once
#pragma warning(disable:4996)
//#include "custom_server_com_api.h"
#include <Windows.h>
// complete

typedef struct _CLIENT_CONNECTION CLIENT_CONNECTION;

typedef struct _HtItem {
    wchar_t* Key;
    CLIENT_CONNECTION* Value;
}HtItem, * PHtItem;


int CreateItem(wchar_t* Key, CLIENT_CONNECTION* Value, HtItem** NewItem);
int DestroyItem(HtItem** Item);