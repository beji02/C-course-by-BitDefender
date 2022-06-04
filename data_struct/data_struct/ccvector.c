#include "ccvector.h"
#include "common.h"

#define INITIAL_SIZE    1

// complete

int VecCreate(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = NULL;

    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (NULL == vec)
    {
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc( sizeof(int) * INITIAL_SIZE );
    if (NULL == vec->Array) 
    {
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}
int VecDestroy(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = *Vector;

    if (NULL == Vector)
    {
        return -1;
    }

    if (NULL == vec)
    {
        return -1;
    }

    free(vec->Array);
    free(vec);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        int newSize = Vector->Size * 2;
        int* newArray = (int*)realloc(Vector->Array, sizeof(int) * newSize);
        
        if (newArray == NULL)
        {
            return -1;
        }
            
        Vector->Array = newArray;
        Vector->Size = newSize;
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

    return 0;
}
int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        int newSize = Vector->Size * 2;
        int* newArray = (int*)realloc(Vector->Array, sizeof(int) * newSize);

        if (newArray == NULL)
        {
            return -1;
        }

        Vector->Array = newArray;
        Vector->Size = newSize;
    }

    for (int i = Vector->Count-1; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[0] = Value;
    Vector->Count++;

    return 0;
}
int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);


    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        int newSize = Vector->Size * 2;
        int* newArray = (int*)realloc(Vector->Array, sizeof(int) * newSize);

        if (newArray == NULL)
        {
            return -1;
        }

        Vector->Array = newArray;
        Vector->Size = newSize;
    }
    if (Index < -1 || Index >= Vector->Count)
    {
        return -1;
    }

    for (int i = Vector->Count - 1; i > Index; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[Index+1] = Value;
    Vector->Count++;

    return 0;
}
int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);

    if (Vector == NULL || Index >= Vector->Count || Index < 0)
    {
        return -1;
    }

    for (int i = Index; i+1 < Vector->Count; i++)
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }
    Vector->Count--;

    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Vector == NULL || Index >= Vector->Count || Index < 0)
    {
        return -1;
    }
    *Value = Vector->Array[Index];

    return 0;
}
int VecGetCount(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    if (NULL == Vector)
    {
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    if (NULL == Vector)
    {
        return -1;
    }

    Vector->Count = 0;
    return 0;
}


void Merge(CC_VECTOR* Vector, int Left, int Middle, int Right)
{
    int i, j, k;
    int n1 = Middle - Left + 1;
    int n2 = Right - Middle;

    CC_VECTOR* leftVector = NULL;
    CC_VECTOR* rightVector = NULL;

    VecCreate(&leftVector);
    VecCreate(&rightVector);

    for (i = 0; i < n1; i++)
        VecInsertTail(leftVector, Vector->Array[Left + i]);
    for (i = 0; i < n2; i++)
        VecInsertTail(rightVector, Vector->Array[Middle + 1 + i]);

    i = 0; 
    j = 0; 
    k = Left; 
    while (i < n1 && j < n2) {
        if (leftVector->Array[i] > rightVector->Array[j]) {
            Vector->Array[k] = leftVector->Array[i];
            i++;
        }
        else {
            Vector->Array[k] = rightVector->Array[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        Vector->Array[k] = leftVector->Array[i];
        i++;
        k++;
    }

    while (j < n2) {
        Vector->Array[k] = rightVector->Array[j];
        j++;
        k++;
    }

    VecDestroy(&leftVector);
    VecDestroy(&rightVector);
}
void MergeSort(CC_VECTOR* Vector, int Left, int Right)
{
    if (Left < Right) {
        int m = Left + (Right - Left) / 2;

        MergeSort(Vector, Left, m);
        MergeSort(Vector, m + 1, Right);

        Merge(Vector, Left, m, Right);
    }
}
int VecSort(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    if (NULL == Vector)
    {
        return -1;
    }

    int l = 0;
    int r = Vector->Count - 1;
    MergeSort(Vector, l, r);

    /*
    for(int i=0; i<Vector->Count; i++)
        for(int j=i+1; j<Vector->Count; j++)
            if (Vector->Array[i] < Vector->Array[j])
            {
                int aux = Vector->Array[i];
                Vector->Array[i] = Vector->Array[j];
                Vector->Array[j] = aux;
            }
    */
    return 0;
}
int VecAppend(CC_VECTOR *DestVector, CC_VECTOR *SrcVector)
{
    CC_UNREFERENCED_PARAMETER(DestVector);
    CC_UNREFERENCED_PARAMETER(SrcVector);
    
    for (int i = 0; i < SrcVector->Count; i++)
    {
        int retVal = VecInsertTail(DestVector, SrcVector->Array[i]);
        if (retVal == -1)
            return -1;
    }
    return 0;
}
