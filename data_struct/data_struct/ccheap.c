#include "ccheap.h"
#include "common.h"

#define INITIAL_SIZE 1

// complete

int HpCreateHeap(CC_HEAP** MaxHeap)
{
    if (MaxHeap == NULL)
    {
        return -1;
    }
    CC_HEAP* myHeap = NULL;
    myHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (myHeap == NULL)
    {
        return -1;
    }
    memset(myHeap, 0, sizeof(*myHeap));
    myHeap->Size = INITIAL_SIZE;
    myHeap->Count = 0;
    myHeap->Array = (int*)malloc(sizeof(int) * INITIAL_SIZE);

    if (myHeap->Array == NULL)
    {
        free(myHeap);
        return -1;
    }

    *MaxHeap = myHeap;
    return 0;
}

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    CC_UNREFERENCED_PARAMETER(MaxHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    int retVal = HpCreateHeap(MaxHeap);
    if (retVal == -1)
        return -1;

    (*MaxHeap)->Type = 1;
    if (InitialElements == NULL)
        return -1;
    int len = VecGetCount(InitialElements);
    int el;
    for (int i = 0; i < len; i++)
    {
        VecGetValueByIndex(InitialElements, i, &el);
        HpInsert(*MaxHeap, el);
    }

    return 0;
}
int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    CC_UNREFERENCED_PARAMETER(MinHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    int retVal = HpCreateHeap(MinHeap);
    if (retVal == -1)
        return -1;

    (*MinHeap)->Type = -1;
    if (InitialElements == NULL)
        return 0;
    int len = VecGetCount(InitialElements);
    int el;
    for (int i = 0; i < len; i++)
    {
        VecGetValueByIndex(InitialElements, i, &el);
        HpInsert(*MinHeap, el);
    }

    return 0;
}
int HpDestroy(CC_HEAP **Heap)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    if (Heap == NULL || *Heap == NULL)
        return -1;
    CC_HEAP* myHeap = *Heap;
    free(myHeap->Array);
    free(myHeap);
    *Heap = NULL;

    return 0;
}

int HpResizeHeap(CC_HEAP* Heap)
{
    if (Heap == NULL)
    {
        return -1;
    }
    /// REALLOC
    int newSize = Heap->Size * 2;
    int* newArray = (int*)realloc(Heap->Array, sizeof(int) * newSize);

    if (newArray == NULL)
    {
        return -1;
    }

    Heap->Array = newArray;
    Heap->Size = newSize;
    return 0;
}
void Swap(int* A, int* B)
{
    int c = *A;
    *A = *B;
    *B = c;
}
void HeapifyUp(CC_HEAP* Heap)
{
    if (Heap == NULL)
        return;
    int i = Heap->Count;
    int sign = Heap->Type;
    while (i != 0 && Heap->Array[i] * sign > Heap->Array[(i-1) / 2] * sign)
    {
        Swap(&Heap->Array[i], &Heap->Array[(i-1) / 2]);
        i = (i-1) / 2;
    }
}
void HeapifyDown(CC_HEAP* Heap, int I)
{
    if (Heap == NULL)
        return;

    int sign = Heap->Type;
    int len = Heap->Count;
    int leftValue;
    int rightValue;
    int goChildIndex;
    while (I * 2 + 1 < len)
    {
        if (I * 2 + 2 < len)
        {
            leftValue = Heap->Array[2 * I + 1];
            rightValue = Heap->Array[2 * I + 2];

            if (leftValue * sign > rightValue * sign)
                goChildIndex = 2 * I + 1;
            else
                goChildIndex = 2 * I + 2;
        }
        else
        {
            leftValue = Heap->Array[2 * I + 1];
            goChildIndex = 2 * I + 1;
        }

        if (Heap->Array[I] * sign < Heap->Array[goChildIndex] * sign)
        {
            Swap(&Heap->Array[I], &Heap->Array[goChildIndex]);
            I = goChildIndex;
        }
        else
            break;
    }
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Heap == NULL)
        return -1;
    if (Heap->Count == Heap->Size)
        HpResizeHeap(Heap);
    Heap->Array[Heap->Count] = Value;
    HeapifyUp(Heap);
    Heap->Count++;

    return 0;
}
int HpRemove(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);
    
    if (Heap == NULL)
        return -1;

    int ok = 0;
    while (!ok)
    {
        ok = 1;
        int len = Heap->Count;
        for (int i = 0; i < len; i++)
        {
            if (Heap->Array[i] == Value)
            {
                ok = 0;
                int lastItem = Heap->Array[len-1];
                Heap->Array[i] = lastItem;
                Heap->Count--;
                HeapifyDown(Heap, i);
                break;
            }
        }
    }


    return 0;
}
int HpRemoveSingle(CC_HEAP* Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Heap == NULL)
        return -1;
    int len = Heap->Count;
    int lastItem = Heap->Array[len - 1];

    for (int i = 0; i < len; i++)
    {
        if (Heap->Array[i] == Value)
        {
            lastItem = Heap->Array[len - 1];
            Heap->Array[i] = lastItem;
            Heap->Count--;
            HeapifyDown(Heap, i);
            break;
        }
    }


    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);

    if (Heap == NULL)
        return -1;
    if (ExtremeValue == NULL)
        return -1;
    if (Heap->Count == 0)
        return -1;
    *ExtremeValue = Heap->Array[0];

    return 0;
}
int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);
    if (Heap == NULL)
        return -1;
    if (ExtremeValue == NULL)
        return -1;
    if (Heap->Count == 0)
        return -1;

    *ExtremeValue = Heap->Array[0];
    HpRemove(Heap, *ExtremeValue);

    return 0;
}
int HpPopExtremeSingle(CC_HEAP* Heap, int* ExtremeValue)
{   
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);
    if (Heap == NULL)
        return -1;
    if (ExtremeValue == NULL)
        return -1;
    if (Heap->Count == 0)
        return -1;
    
    *ExtremeValue = Heap->Array[0];
    HpRemoveSingle(Heap, *ExtremeValue);

    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    if (Heap == NULL)
        return -1;
    return Heap->Count;
}
int HpSortToVector(CC_HEAP *Heap, CC_VECTOR** SortedVector)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(SortedVector);
    if (Heap == NULL)
        return -1;
    if (SortedVector == NULL)
        return -1;
    
    int el, len = Heap->Count;
    CC_HEAP* auxHeap = NULL;
    CC_VECTOR* vec = NULL;

    VecCreate(&vec);
    HpCreateMinHeap(&auxHeap, NULL);
    for (int i = 0; i < len; i++)
    {
        el = Heap->Array[i];
        HpInsert(auxHeap, el);
    }
    for (int i = 0; i < len; i++)
    {
        HpPopExtremeSingle(auxHeap, &el);
        VecInsertTail(vec, el);
    }
    HpDestroy(&auxHeap);
    *SortedVector = vec;
    
    return 0;
}

