#pragma once
#pragma warning(disable:4996)
#include "ccitem.h"

// complete

typedef struct _LinkedList LinkedList, *PLinkedList;
struct _LinkedList {
    HtItem* Item;
    LinkedList* Next;
};


int DestroyLinkedList(LinkedList** LinkedList);
int InsertLinkedList(LinkedList** List, HtItem* Item);
int SearchLinkedList(LinkedList* List, char* Key, int* Value);
int ReplaceValueLinkedList(LinkedList* List, char* Key, int Value);
int DeleteItemLinkedList(LinkedList** List, char* Key);