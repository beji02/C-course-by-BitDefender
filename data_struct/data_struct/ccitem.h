#pragma once
#pragma warning(disable:4996)

// complete

typedef struct _HtItem {
    char* Key;
    int Value;
}HtItem, *PHtItem;


int CreateItem(char* Key, int Value, HtItem** NewItem);
int DestroyItem(HtItem** Item);