#include "stdafx.h"
#include "ccitem.h"
#include "common.h"
// complete

int CreateItem(wchar_t* Key, CLIENT_CONNECTION* Value, HtItem** NewItem) {
    if (NewItem == NULL)
    {
        return -1;
    }

    HtItem* item = NULL;
    item = (HtItem*)malloc(sizeof(HtItem));

    if (item == NULL)
    {
        return -1;
    }

    memset(item, 0, sizeof(*item));
    item->Key = (wchar_t*)malloc(wcslen(Key) + 1);
    if (item->Key == NULL)
    {
        free(item);
        return -1;
    }
    wcscpy(item->Key, Key);
    item->Value = Value;

    *NewItem = item;
    return 0;
}
int DestroyItem(HtItem** Item)
{
    if (Item == NULL)
    {
        return -1;
    }

    HtItem* myItem = *Item;
    if (myItem == NULL)
    {
        free(myItem);
        return -1;
    }

    //free(myItem->Key);
    //free(myItem->Value);
    free(myItem);

    *Item = NULL;

    return 0;
}
