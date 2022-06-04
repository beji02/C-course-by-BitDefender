#pragma once
#pragma warning(disable:4996)
#include "ccitem.h"
#include "server_communication_api.h"

// complete

typedef struct _LinkedList LinkedList, * PLinkedList;
struct _LinkedList {
    HtItem* Item;
    LinkedList* Next;
};


int DestroyLinkedList(LinkedList** LinkedList);
int InsertLinkedList(LinkedList** List, HtItem* Item);
int SearchLinkedList(LinkedList* List, wchar_t* Key, CLIENT_CONNECTION** Value);
int ReplaceValueLinkedList(LinkedList* List, wchar_t* Key, CLIENT_CONNECTION* Value);
int DeleteItemLinkedList(LinkedList** List, wchar_t* Key);