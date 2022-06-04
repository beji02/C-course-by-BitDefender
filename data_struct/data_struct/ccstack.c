#include "ccstack.h"
#include "common.h"

// complete

int StCreate(CC_STACK **Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (Stack == NULL)
    {
        return -1;
    }
    CC_STACK* myStack = NULL;
    myStack = (CC_STACK*)malloc(sizeof(CC_STACK));

    if (myStack == NULL)
    {
        return -1;
    }
    memset(myStack, 0, sizeof(*myStack));
    myStack->Count = 0;
    *Stack = myStack;
    return 0;
}
int StDestroy(CC_STACK **Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    if (Stack == NULL)
    {
        return -1;
    }
    if (*Stack == NULL)
    {
        return -1;
    }

    CC_STACK* myStack = *Stack;
    LinkedListBasic* myList = myStack->List;
    LinkedListBasic* auxList = NULL;

    for (int i = 0; i < myStack->Count; i++)
    {
        auxList = myList;
        myList = myList->Next;
        free(auxList);
    }
    free(*Stack);
    *Stack = NULL;

    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Stack == NULL)
    {
        return -1;
    }

    LinkedListBasic* node = NULL;
    node = (LinkedListBasic*)malloc(sizeof(LinkedListBasic));
    if (node == NULL)
    {
        return -1;
    }
    Stack->Count++;
    node->Value = Value;
    node->Next = Stack->List;
    Stack->List = node;

    return 0;
}
int StPop(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Stack == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    if (Stack->Count == 0)
    {
        return -1;
    }
    Stack->Count--;
    LinkedListBasic* auxList = Stack->List;
    Stack->List = Stack->List->Next;
    *Value = auxList->Value;
    free(auxList);
    
    return 0;
}
int StPeek(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);
    if (Stack == NULL)
    {
        return -1;
    }
    if (Value == NULL)
    {
        return -1;
    }
    if (Stack->Count == 0)
    {
        return -1;
    }

    *Value = Stack->List->Value;
    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->Count ? 0 : 1;
}
int StGetCount(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->Count;
}
int StClear(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (Stack == NULL)
    {
        return -1;
    }

    CC_STACK* myStack = Stack;
    LinkedListBasic* myList = myStack->List;
    LinkedListBasic* auxList = NULL;

    for (int i = 0; i < myStack->Count; i++)
    {
        auxList = myList;
        myList = myList->Next;
        free(auxList);
    }
    Stack->Count = 0;
    Stack->List = NULL;

    return 0;
}
int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(StackToPush);
    if (Stack == NULL)
    {
        return -1;
    }
    if (StackToPush == NULL)
    {
        return -1;
    }
    if (Stack->List == NULL)
    {
        Stack->List = StackToPush->List;
        StackToPush->List = NULL;
        Stack->Count = StackToPush->Count;
        StackToPush->Count = 0;
        
        return 0;
    }

    LinkedListBasic* auxList = Stack->List;

    for (int i = 0; i < Stack->Count-1; i++)
        auxList = auxList->Next;
    auxList->Next = StackToPush->List;
    StackToPush->List = NULL;
    Stack->Count = Stack->Count + StackToPush->Count;
    StackToPush->Count = 0;

    return 0;
}
