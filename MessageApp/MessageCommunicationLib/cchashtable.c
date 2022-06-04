#include "stdafx.h"
#include "cchashtable.h"
#include "common.h"
#include <stdio.h>
#define INITIAL_SIZE    1031

int HashFunction(wchar_t* Key);

// complete

int HtCreate(CC_HASH_TABLE** HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);

    if (HashTable == NULL)
    {
        return -1;
    }

    CC_HASH_TABLE* myHashTable = NULL;
    myHashTable = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));

    if (myHashTable == NULL)
    {
        return -1;
    }

    memset(myHashTable, 0, sizeof(*myHashTable));
    myHashTable->size = INITIAL_SIZE;
    myHashTable->count = 0;
    myHashTable->buckets = (LinkedList**)malloc(sizeof(LinkedList*) * INITIAL_SIZE);

    //myHashTable->items = (HtItem**)malloc(sizeof(HtItem*) * INITIAL_SIZE);
    if (myHashTable->buckets == NULL)
    {
        free(myHashTable);
        return -1;
    }

    memset(myHashTable->buckets, 0, sizeof(LinkedList*) * INITIAL_SIZE);

    *HashTable = myHashTable;
    return 0;
}
int HtDestroy(CC_HASH_TABLE** HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    if (HashTable == NULL)
    {
        return -1;
    }
    if (*HashTable == NULL)
    {
        return -1;
    }

    CC_HASH_TABLE* myHashTable = *HashTable;
    for (int i = 0; i < myHashTable->size; i++)
    {
        LinkedList* list = myHashTable->buckets[i];
        if (list != NULL)
        {
            DestroyLinkedList(&list);
        }
    }
    //free(myHashTable->buckets);
    free(myHashTable);

    *HashTable = NULL;

    return 0;
}

int HashFunction(wchar_t* Key)
{
    CC_UNREFERENCED_PARAMETER(Key);

    unsigned long hash = 5381;
    int c;
    c = *Key++;
    while (c)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = *Key++;
    }

    return hash % INITIAL_SIZE;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, wchar_t* Key, CLIENT_CONNECTION* Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Key == NULL)
    {
        return -1;
    }

    HtItem* item = NULL;
    if (CreateItem(Key, Value, &item) == -1)
    {
        return -1;
    }

    int index = HashFunction(Key);

    CLIENT_CONNECTION* value = NULL;
    int retVal = SearchLinkedList(HashTable->buckets[index], Key, &value);
    if (retVal == 0)
        return -1;
    HashTable->count++;
    if (InsertLinkedList(&HashTable->buckets[index], item) == -1)
        return -1;
    return 0;
}
int HtGetKeyValue(CC_HASH_TABLE* HashTable, wchar_t* Key, CLIENT_CONNECTION** Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    int index = HashFunction(Key);

    int retVal = SearchLinkedList(HashTable->buckets[index], Key, Value);
    if (retVal == -1)
        return -1;
    return 0;
}
int HtRemoveKey(CC_HASH_TABLE* HashTable, wchar_t* Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    //TODO
    int index = HashFunction(Key);

    int retVal = DeleteItemLinkedList(&HashTable->buckets[index], Key);
    if (retVal == -1)
        return -1;

    HashTable->count--;
    return 0;
}

int HtHasKey(CC_HASH_TABLE* HashTable, wchar_t* Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    if (Key == NULL)
    {
        return -1;
    }
    
    int index = HashFunction(Key);
    CLIENT_CONNECTION* value = NULL;
    int retVal = SearchLinkedList(HashTable->buckets[index], Key, &value);
    if (retVal == -1)
        return 0;
    return 1;
}
int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR** Iterator, wchar_t** Key)
{
    CC_HASH_TABLE_ITERATOR* iterator = NULL;

    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }
    if (HashTable->count == 0)
    {
        *Iterator = NULL;
        return -2;
    }

    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
    if (NULL == iterator)
    {
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    // INITIALIZE THE REST OF THE FIELDS OF iterator
    iterator->HashTable = HashTable;
    iterator->CurrentList = NULL;
    iterator->_index = 0;

    *Iterator = iterator;
    // FIND THE FIRST KEY AND SET Key

    int i = 0;
    while (iterator->HashTable->size > i && iterator->HashTable->buckets[i] == NULL)
    {
        i++;
    }
    iterator->CurrentList = iterator->HashTable->buckets[i];
    *Key = iterator->CurrentList->Item->Key;
    //strcpy(*Key, iterator->CurrentItem->key);

    return 0;
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR* Iterator, wchar_t** Key)
{
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Iterator);

    if (Iterator == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    if (Iterator->HashTable == NULL)
    {
        return -1;
    }
    if (Iterator->CurrentList == NULL)
    {
        return -1;
    }
    if (Iterator->_index == Iterator->HashTable->count - 1)
    {
        return -2;
    }

    Iterator->_index++;
    if (Iterator->CurrentList->Next != NULL)
    {
        Iterator->CurrentList = Iterator->CurrentList->Next;
        *Key = Iterator->CurrentList->Item->Key;

        return 0;
    }
    int hashValue = HashFunction(Iterator->CurrentList->Item->Key) + 1;
    while (Iterator->HashTable->size > hashValue && Iterator->HashTable->buckets[hashValue] == NULL)
    {
        hashValue++;
    }
    if (Iterator->HashTable->buckets[hashValue] != NULL)
    {
        Iterator->CurrentList = Iterator->HashTable->buckets[hashValue];
        *Key = Iterator->CurrentList->Item->Key;

        return 0;
    }

    return -1;
}
int HtReleaseIterator(CC_HASH_TABLE_ITERATOR** Iterator)
{
    CC_UNREFERENCED_PARAMETER(Iterator);

    if (Iterator == NULL)
    {
        return -1;
    }
    CC_HASH_TABLE_ITERATOR* it = *Iterator;
    if (it == NULL)
    {
        return -1;
    }

    free(it);
    *Iterator = NULL;
    return 0;
}
int HtClear(CC_HASH_TABLE* HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    if (HashTable == NULL)
    {
        return -1;
    }
    int i = 0;
    while (i < HashTable->size)
    {
        DestroyLinkedList(&HashTable->buckets[i]);
        i++;
    }
    HashTable->count = 0;
    return 0;
}
int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    if (HashTable == NULL)
    {
        return -1;
    }

    return HashTable->count;
}
