#include "cclinkedlist.h"
#include "common.h"

// complete

int AllocateSpaceForNewNode(LinkedList** NewNode)
{
    LinkedList* node = NULL;
    node = (LinkedList*)malloc(sizeof(LinkedList));
    if (node == NULL)
    {
        return -1;
    }
    node->Item = NULL;
    node->Next = NULL;

    *NewNode = node;
    return 0;
}
int DeAllocateSpaceForNode(LinkedList** Node)
{
    if (Node == NULL || *Node == NULL)
    {
        return -1;
    }

    LinkedList* aux = *Node;
    DestroyItem(&(aux->Item));
    free(aux);
    *Node = NULL;
    return 0;
}

int DestroyLinkedList(LinkedList** List)
{
    if (List == NULL || *List == NULL)
    {
        return -1;
    }

    LinkedList* Head = *List;
    LinkedList* Aux = Head;

    while (Head) {
        Aux = Head;
        Head = Head->Next;
        DeAllocateSpaceForNode(&Aux);
    }
    *List = NULL;

    return 0;
}
int InsertLinkedList(LinkedList** List, HtItem* item)
{
    if (List == NULL)
    {
        return -1;
    }

    LinkedList* NewHead = NULL;
    AllocateSpaceForNewNode(&NewHead);
    if (NewHead == NULL)
    {
        return -1;
    }

    NewHead->Item = item;
    NewHead->Next = *List;

    *List = NewHead;
    return 0;
}
int SearchLinkedList(LinkedList* List, char* Key, int* Value)
{
    if (List == NULL)
    {
        return -1;
    }

    while (List)
    {
        if (Strcmp(List->Item->Key, Key) == 0)
        {
            *Value = List->Item->Value;
            return 0;
        }
        List = List->Next;
    }
    return -1;
}
int DeleteItemLinkedList(LinkedList** List, char* Key)
{
    if (List == NULL || *List == NULL)
    {
        return -1;
    }
    
    LinkedList* Head = *List;
    if (Strcmp(Head->Item->Key, Key) == 0)
    {
        *List = Head->Next;
        DeAllocateSpaceForNode(&Head);
        return 0;
    }

    LinkedList* NextNode = Head->Next;
    while (NextNode != NULL)
    {
        if (Strcmp(NextNode->Item->Key, Key) == 0)
        {
            Head->Next = NextNode->Next;
            DeAllocateSpaceForNode(&NextNode);
            return 0;
        }
        Head = Head->Next;
        NextNode = Head->Next;
    }
    return -1;
}
int ReplaceValueLinkedList(LinkedList* List, char* Key, int Value)
{
    if (List == NULL)
    {
        return -1;
    }
    while (List != NULL)
    {
        if (Strcmp(List->Item->Key, Key) == 0)
        {
            List->Item->Value = Value;
            return 0;
        }
        List = List->Next;
    }
    return -1;
}
