#include <stdio.h>
#include "ccvector.h"
#include "ccstack.h"
#include "cchashtable.h"
#include "ccheap.h"
#include "cctree.h"
#include <assert.h>
#include <string.h>


int TestVector(); //done
int TestStack(); //done
int TestHashTable(); //done
int TestTree();//done
int TestHeap();//done


int TestItems();
int TestLinkedList();

void RunTests();

int main(void)
{
    RunTests();
    return 0;
}
void RunTests()
{
    /// NOTE: The tests provided here are by no means exhaustive and are only
    /// provided as a starting point (not all functions are tested, not all use cases
    /// and failure scenarios are covered). You are encouraged to expand these tests
    /// to include missing scenarios.
    if (0 == TestVector())
    {
        printf("Vector test passed\n");
    }
    else
    {
        printf("Vector test failed\n");
    }

    if (0 == TestItems())
    {
        printf("Items test passed\n");
    }
    else
    {
        printf("Items test failed\n");
    }
    if (0 == TestLinkedList())
    {
        printf("LinkedList test passed\n");
    }
    else
    {
        printf("LinkedList test failed\n");
    }


    if (0 == TestHashTable())
    {
        printf("HashTable test passed\n");
    }
    else
    {
        printf("HashTable test failed\n");
    }
   
    
    if (0 == TestStack())
    {
        printf("Stack test passed\n");
    }
    else
    {
        printf("Stack test failed\n");
    }

    if (0 == TestTree())
    {
        printf("Tree test passed\n");
    }
    else
    {
        printf("Tree test failed\n");
    }

    if (0 == TestHeap())
    {
        printf("Heap test passed\n");
    }
    else
    {
        printf("Heap test failed\n");
    }
}

int TestTree()
{
    int retGlobal = 0;
    int retVal = -1;
    int Value = -1;
    CC_TREE* Tree = NULL;

    // testing TreeCreate
    {
        retVal = TreeCreate(&Tree);
        retVal = retVal == -1 || Tree->Count != 0 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeCreate failed!\n");
            goto cleanup;
        }
    }

    // testing TreeInsert
    {
        retVal = TreeInsert(Tree, 10);
        retVal = retVal == -1 || Tree->Count != 1 || Tree->Root->Value != 10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeInsert failed!\n");
            goto cleanup;
        }
        retVal = TreeInsert(Tree, 20);
        retVal = retVal == -1 || Tree->Count != 2 || Tree->Root->Right->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeInsert failed!\n");
            goto cleanup;
        }
        retVal = TreeInsert(Tree, 20);
        retVal = retVal == -1 || Tree->Count != 3 || Tree->Root->Right->Value != 20 || Tree->Root->Right->HowMany != 2 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeInsert failed!\n");
            goto cleanup;
        }
        TreeInsert(Tree, 10);
        retVal = TreeInsert(Tree, 0);
        retVal = retVal == -1 || Tree->Count != 5 || Tree->Root->Left->Value != 0 || Tree->Root->HowMany != 2 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeInsert failed!\n");
            goto cleanup;
        }
    }

    // testing TreeRemove
    {
        // (10, 2)->(20, 2)
        //  |
        // (0, 1)
        retVal = TreeRemove(Tree, 10);
        retVal = retVal == -1 || Tree->Count != 3 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 1 failed!\n");
            goto cleanup;
        }

        retVal = TreeRemove(Tree, 30);
        retVal = retVal != -1 || Tree->Count != 3 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 2 failed!\n");
            goto cleanup;
        }

        retVal = TreeRemove(Tree, 0);
        retVal = retVal == -1 || Tree->Count != 2 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 3 failed!\n");
            goto cleanup;
        }

        retVal = TreeRemove(Tree, 0);
        retVal = retVal != -1 || Tree->Count != 2 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 4 failed!\n");
            goto cleanup;
        }

        retVal = TreeRemove(Tree, 20);
        retVal = retVal == -1 || Tree->Count != 0 || Tree->Root != NULL ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 5 failed!\n");
            goto cleanup;
        }




        TreeInsert(Tree, 10);
        TreeInsert(Tree, 10);
        TreeInsert(Tree, 20);
        TreeInsert(Tree, 20);
        TreeInsert(Tree, 15);



        retVal = TreeRemove(Tree, 10);
        //printf("%d", Tree->Root->Value);
        retVal = retVal == -1 || Tree->Count != 3 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 6 failed!\n");
            goto cleanup;
        }


        retVal = TreeRemove(Tree, 15);
        //printf("%d", Tree->Count);
        retVal = retVal == -1 || Tree->Count != 2 || Tree->Root->Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 7 failed!\n");
            goto cleanup;
        }

        retVal = TreeRemove(Tree, 20);
        retVal = retVal == -1 || Tree->Count != 0 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeRemove 8 failed!\n");
            goto cleanup;
        }
    }
   
    // testing TreeContains
    {
        retVal = TreeContains(Tree, -100);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("TreeContains failed!\n");
            goto cleanup;
        }
        TreeInsert(Tree, 1);
        TreeInsert(Tree, 7);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 8);
        TreeInsert(Tree, 1000);
        TreeInsert(Tree, 9);
        TreeInsert(Tree, -1);
        TreeInsert(Tree, 2);
        TreeInsert(Tree, 0);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -1000);
        TreeInsert(Tree, -7);

        retVal = TreeContains(Tree, -100);
        if (retVal != 1)
        {
            retGlobal = -1;
            printf("TreeContains failed!\n");
            goto cleanup;
        }
        retVal = TreeContains(Tree, -109);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("TreeContains failed!\n");
            goto cleanup;
        }
        retVal = TreeContains(Tree, 0);
        if (retVal != 1)
        {
            retGlobal = -1;
            printf("TreeContains failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, 0);
        retVal = TreeContains(Tree, 0);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("TreeContains failed!\n");
            goto cleanup;
        }
    }
    
    // testing TreeGetCount
    {
        retVal = TreeGetCount(Tree);
        if (retVal != 17)
        {
            retGlobal = -1;
            printf("TreeGetCount failed!\n");
            goto cleanup;
        }
        TreeInsert(Tree, 0);
        retVal = TreeGetCount(Tree);
        if (retVal != 18)
        {
            retGlobal = -1;
            printf("TreeGetCount failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, 0);
        retVal = TreeGetCount(Tree);
        if (retVal != 17)
        {
            retGlobal = -1;
            printf("TreeGetCount failed!\n");
            goto cleanup;
        }
    }

    // testing TreeGetHeight
    {
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 5)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, 9);
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 4)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, 8);
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 4)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, -1000);
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 4)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, -7);
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 4)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
        TreeRemove(Tree, 1000);
        retVal = TreeGetHeight(Tree);
        //printf("%d", retVal);
        if (retVal != 3)
        {
            retGlobal = -1;
            printf("TreeGetHeight failed!\n");
            goto cleanup;
        }
    }

    // testing TreeClear
    {
        retVal = TreeClear(Tree);
        //printf("%d", TreeContains(Tree, -1000));
        retVal = retVal == -1 || TreeGetHeight(Tree)!=0 || TreeContains(Tree, -1000) != 0? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeClear failed!\n");
            goto cleanup;
        }

    }

    // testing TreeGetNthPreorder
    {
        TreeInsert(Tree, 1);
        TreeInsert(Tree, 7);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 100);
        TreeInsert(Tree, 8);
        TreeInsert(Tree, 1000);
        TreeInsert(Tree, 9);
        TreeInsert(Tree, -1);
        TreeInsert(Tree, 2);
        TreeInsert(Tree, 0);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -100);
        TreeInsert(Tree, -1000);
        TreeInsert(Tree, -7);

        retVal = TreeGetNthPreorder(Tree, 0, &Value);
        retVal = retVal == -1 || Value != 1 ? -1 : 0;
        //printf("%d", retVal);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthPreorder failed!\n");
            goto cleanup;
        }

        retVal = TreeGetNthPreorder(Tree, 7, &Value);
        retVal = retVal == -1 || Value != -7 ? -1 : 0;
        //printf("%d", Value);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthPreorder failed!\n");
            goto cleanup;
        }
        /*
        printf("%d\n", Tree->Count);
        
        for (int i = 0; i < Tree->Count; i++)
        {
            TreeGetNthPreorder(Tree, i, &Value);
            printf("%d ", Value);
        }
        */
    }
   
    // testing TreeGetNthInorder
    {
        retVal = TreeGetNthInorder(Tree, 0, &Value);
        retVal = retVal == -1 || Value != -1000 ? -1 : 0;
        //printf("%d", Value);

        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthInorder 1 failed!\n");
            goto cleanup;
        }

        retVal = TreeGetNthInorder(Tree, 10, &Value);
        retVal = retVal == -1 || Value != 7 ? -1 : 0;
        //printf("%d", Value);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthInorder 2 failed!\n");
            goto cleanup;
        }
        /*
        printf("%d\n", Tree->Count);
        
        for (int i = 0; i < Tree->Count; i++)
        {
            TreeGetNthInorder(Tree, i, &Value);
            printf("%d ", Value);
        }
        */
        
    }

    // testing TreeGetNthPostorder
    {
        retVal = TreeGetNthPostorder(Tree, 0, &Value);
        retVal = retVal == -1 || Value != -1000 ? -1 : 0;
        //printf("%d", Value);

        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthPostorder 1 failed!\n");
            goto cleanup;
        }

        retVal = TreeGetNthPostorder(Tree, 10, &Value);
        retVal = retVal == -1 || Value != 8 ? -1 : 0;
        //printf("%d", Value);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("TreeGetNthPostorder 2 failed!\n");
            goto cleanup;
        }
        /*
        printf("%d\n", Tree->Count);
        
        for (int i = 0; i < Tree->Count; i++)
        {
            TreeGetNthPostorder(Tree, i, &Value);
            printf("%d ", Value);
        }
        */
        
    }

cleanup:
    if (Tree != NULL)
    {
        // testing TreeDestroy
        {
            retVal = TreeDestroy(&Tree);
            if (retVal != 0)
            {
                printf("TreeDestroy failed!\n");
                retGlobal = -1;
            }
        }
    }
    return retGlobal;

    /*
    TreeCreate(&Tree);
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 7);
    TreeInsert(Tree, 9);
    TreeInsert(Tree, 3);
    TreeInsert(Tree, 9);
    TreeInsert(Tree, 4);
    TreeInsert(Tree, 5);
    TreeInsert(Tree, 10);
    TreeInsert(Tree, -1);
    TreeInsert(Tree, -10);
    TreeInsert(Tree, -110);
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthPreorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthPostorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");

    TreeRemove(Tree, 1);    
    TreeRemove(Tree, 7);
    TreeRemove(Tree, 9);
    TreeRemove(Tree, 10);
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    for (int i = -100; i <= 100; i++)
    {
        if (TreeContains(Tree, i) == 1)
        {
            TreeRemove(Tree, i);
            TreeInsert(Tree, i * 1000);
        }
    }
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    TreeClear(Tree);
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 7);
    TreeInsert(Tree, 9);
    TreeInsert(Tree, 10);
    printf("%d, %d: ", TreeGetCount(Tree), TreeGetHeight(Tree));
    for (int i = 1; i <= TreeGetCount(Tree); i++)
    {
        TreeGetNthInorder(Tree, i, &Value);
        printf("%d ", Value);
    }
    printf("\n");

    
    TreeDestroy(&Tree);
    */
   
    /*int retVal = -1;
    CC_TREE* usedTree = NULL;

    retVal = TreeCreate(&usedTree);
    if (0 != retVal)
    {
        printf("TreeCreate failed!\n");
        goto cleanup;
    }

    retVal = TreeInsert(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeInsert failed!\n");
        goto cleanup;
    }

    if (1 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = TreeRemove(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeRemove failed!\n");
        goto cleanup;
    }

    if (0 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value after remove!\n");
        retVal = -1;
        goto cleanup;
    }

    if (0 != TreeGetCount(usedTree))
    {
        printf("TreeGetCount invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedTree)
    {
        if (0 != TreeDestroy(&usedTree))
        {
            printf("TreeDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;*/
}            
void printHeap(CC_HEAP* Heap)
{
    printf("Nr noduri:%d\n", Heap->Count);
    int p2 = 1;
    int k = 1;
    for (int i = 0; i < Heap->Count; i++)
    {
        printf("%d ", Heap->Array[i]);
        k--;
        if (k == 0)
        {
            printf("\n");
            p2 = p2 * 2;
            k = p2;
        }
    }
    printf("\n");
}
int TestHeap()
{
    int retGlobal = 0;
    int retVal = -1;
    int Value = -1;
    int Value2 = -1;

    CC_VECTOR* Vector = NULL;
    CC_HEAP* MinHeap = NULL;
    CC_HEAP* MaxHeap = NULL;

    VecCreate(&Vector);
    VecInsertTail(Vector, 10);
    VecInsertTail(Vector, -10);
    VecInsertTail(Vector, 20);
    VecInsertTail(Vector, 20);
    VecInsertTail(Vector, -40);
    //printf("%d", Vector->Count);


    // testing HpCreateMinHeap and HpCreateMaxHeap
    {
        retVal = HpCreateMinHeap(&MinHeap, NULL);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpCreateMinHeap failed!\n");
            goto cleanup;
        }
        retVal = HpCreateMaxHeap(&MaxHeap, Vector);
        retVal = retVal == -1 || MaxHeap->Count != 5 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpCreateMaxHeap failed\n");
            goto cleanup;
        }
    }

    // testing HpInsert
    {
        retVal = HpInsert(MinHeap, 10);
        retVal = retVal == -1 || MinHeap->Count != 1 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpInsert 1 failed!\n");
            goto cleanup;
        }
        retVal = HpInsert(MinHeap, -10);
        retVal = retVal == -1 || MinHeap->Count != 2 || MinHeap->Array[0] != -10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpInsert 2 failed!\n");
            goto cleanup;
        }
        retVal = HpInsert(MinHeap, 20);
        retVal = retVal == -1 || MinHeap->Count != 3 || MinHeap->Array[0] != -10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpInsert 3 failed!\n");
            goto cleanup;
        }
    }
    
    // testing HpRemove
    {
        retVal = HpRemove(MaxHeap, -10);
        retVal = retVal == -1 || MaxHeap->Count != 4 || MaxHeap->Array[0] != 20 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 1 failed!\n");
            goto cleanup;
        }
        retVal = HpRemove(MaxHeap, 20);
        retVal = retVal == -1 || MaxHeap->Count != 2 || MaxHeap->Array[0] != 10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 2 failed!\n");
            goto cleanup;
        }
        retVal = HpRemove(MaxHeap, 10);
        retVal = retVal == -1 || MaxHeap->Count != 1 || MaxHeap->Array[0] != -40 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 3 failed!\n");
            goto cleanup;
        }
        retVal = HpRemove(MaxHeap, -40);
        retVal = retVal == -1 || MaxHeap->Count != 0? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 4 failed!\n");
            goto cleanup;
        }




        retVal = HpRemove(MinHeap, -10);
        retVal = retVal == -1 || MinHeap->Count != 2 || MinHeap->Array[0] != 10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 1 failed!\n");
            goto cleanup;
        }
        retVal = HpRemove(MinHeap, 20);
        retVal = retVal == -1 || MinHeap->Count != 1 || MinHeap->Array[0] != 10 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 2 failed!\n");
            goto cleanup;
        }
        retVal = HpRemove(MinHeap, 10);
        retVal = retVal == -1 || MinHeap->Count != 0 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpRemove 3 failed!\n");
            goto cleanup;
        }
    }

    // testing HpGetExtreme
    {
        HpInsert(MinHeap, 1);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != 1 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }
        HpInsert(MinHeap, 2);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != 1 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }
        HpInsert(MinHeap, -1);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != -1 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }
        HpInsert(MinHeap, 0);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != -1 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }
        HpInsert(MinHeap, -2);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != -2 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }
        HpInsert(MinHeap, -3);
        retVal = HpGetExtreme(MinHeap, &Value);
        retVal = retVal == -1 || Value != -3 ? -1 : 0;
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("HpGetExtreme failed!\n");
            goto cleanup;
        }

    }

    // testing HpPopExtreme
    {
        VecClear(Vector);
        VecInsertTail(Vector, -1);
        VecInsertTail(Vector, -2);
        VecInsertTail(Vector, -3);
        VecInsertTail(Vector, 4);
        VecInsertTail(Vector, 110);
        VecInsertTail(Vector, -111);

        HpDestroy(&MaxHeap);
        HpCreateMaxHeap(&MaxHeap, Vector);
        VecSort(Vector);
        while (MaxHeap->Count != 0)
        {
            retVal = HpPopExtreme(MaxHeap, &Value);
            VecGetValueByIndex(Vector, 0, &Value2);
            VecRemoveByIndex(Vector, 0);
            retVal = retVal == -1 || Value != Value2 ? -1:0;
            if (retVal == -1)
            {
                //printf("%d;%d\n", Value, Value2);
                retGlobal = -1;
                printf("HpPopExtreme failed!\n");
                goto cleanup;
            }
        }
    }

    // testing HpGetElementCount
    {
        HpInsert(MaxHeap, 1);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 1)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 1 failed!\n");
            goto cleanup;
        }
        HpInsert(MaxHeap, 2);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 2)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 2 failed!\n");
            goto cleanup;
        }
        HpInsert(MaxHeap, 3);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 3)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 3 failed!\n");
            goto cleanup;
        }
        HpInsert(MaxHeap, 3);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 4)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 4 failed!\n");
            goto cleanup;
        }
        HpInsert(MaxHeap, -1);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 5)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 5 failed!\n");
            goto cleanup;
        }
        HpInsert(MaxHeap, -1);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 6)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 6 failed!\n");
            goto cleanup;
        }
        HpPopExtreme(MaxHeap, &Value);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 4)
        {
            printf("%d;\n", retVal);
            retGlobal = -1;
            printf("HpGetElementCount 7 failed!\n");
            goto cleanup;
        }
        HpPopExtreme(MaxHeap, &Value);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 3)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 8 failed!\n");
            goto cleanup;
        }
        HpPopExtreme(MaxHeap, &Value);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 2)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 9 failed!\n");
            goto cleanup;
        }
        HpPopExtreme(MaxHeap, &Value);
        retVal = HpGetElementCount(MaxHeap);
        if (retVal != 0)
        {
            //printf("%d;%d\n", Value, Value2);
            retGlobal = -1;
            printf("HpGetElementCount 10 failed!\n");
            goto cleanup;
        }
    }


    // testing HpSortToVector
    {
        /*
        VecInsertTail(Vector, 1);
        VecInsertTail(Vector, -1);
        VecInsertTail(Vector, -2);
        VecInsertTail(Vector, -2);
        VecInsertTail(Vector, -3);
        VecInsertTail(Vector, 3);
        VecInsertTail(Vector, 4);
        VecInsertTail(Vector, -444000);

        HpDestroy(&MaxHeap);
        
        /*
        HpCreateMinHeap(&MaxHeap, Vector);
        HpSortToVector(MaxHeap, Vector);
        for (int i = 0; i < Vector->Count; i++)
            printf("%d ", Vector->Array[i]);
        printf("\n");
        */
    }

cleanup:
    
    if (MinHeap != NULL)
    {
        // testing HpDestroy
        {
            retVal = HpDestroy(&MinHeap);
            if (retVal == -1)
            {
                retGlobal = -1;
                printf("HpDestroy failed!\n");
            }
        }
        
    }
    if (MaxHeap != NULL)
    {
        HpDestroy(&MaxHeap);
    }
    
    VecDestroy(&Vector);

    /*
    VecCreate(&Vector);
    VecInsertTail(Vector, 1);
    VecInsertTail(Vector, 2);
    VecInsertTail(Vector, 3);
    VecInsertTail(Vector, -4);
    VecInsertTail(Vector, -2);
    VecInsertTail(Vector, -2);

    HpCreateMinHeap(&MinHeap, Vector);
    HpCreateMaxHeap(&MaxHeap, Vector);

    printHeap(MinHeap);
    printHeap(MaxHeap);

    HpInsert(MinHeap, 100);
    HpInsert(MaxHeap, 100);

    printHeap(MinHeap);
    printHeap(MaxHeap);

    HpInsert(MinHeap, -100);
    HpInsert(MaxHeap, -100);
    HpInsert(MinHeap, -100);
    HpInsert(MaxHeap, -100);

    printHeap(MinHeap);
    printHeap(MaxHeap);

    HpRemove(MinHeap, 100);
    HpRemove(MaxHeap, 100);

    printHeap(MinHeap);
    printHeap(MaxHeap);

    HpRemove(MinHeap, -100);
    HpRemove(MaxHeap, -100);

    printHeap(MinHeap);
    printHeap(MaxHeap);

    
    HpSortToVector(MinHeap, Vector);
    for (int i = 0; i < HpGetElementCount(MinHeap); i++)
        printf("%d, ", Vector->Array[i]);
    printf("\n");
    printHeap(MinHeap);

    HpSortToVector(MaxHeap, Vector);
    for (int i = 0; i < HpGetElementCount(MaxHeap); i++)
        printf("%d, ", Vector->Array[i]);
    printf("\n");
    printHeap(MaxHeap);

    
    while (HpGetElementCount(MinHeap) != 0)
    {
        HpGetExtreme(MinHeap, &Value);
        HpPopExtreme(MinHeap, &Value2);
        assert(Value == Value2);
        printf("Nod Scos: %d\n", Value);
        printHeap(MinHeap);
    }

    while (HpGetElementCount(MaxHeap) != 0)
    {
        HpGetExtreme(MaxHeap, &Value);
        HpPopExtreme(MaxHeap, &Value2);
        assert(Value == Value2);
        printf("Nod Scos: %d\n", Value);
        printHeap(MaxHeap);
    }

    HpDestroy(&MinHeap);
    HpDestroy(&MaxHeap);
    */

    return retGlobal;
    /*int retVal = -1;
    int foundVal = -1;
    CC_HEAP* usedHeap = NULL;

    retVal = HpCreateMinHeap(&usedHeap, NULL);
    if (0 != retVal)
    {
        printf("HpCreateMinHeap failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 20);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 10);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    if (2 != HpGetElementCount(usedHeap))
    {
        printf("Invalid element count!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = HpGetExtreme(usedHeap, &foundVal);
    if (0 != retVal)
    {
        printf("HpGetExtreme failed!\n");
        goto cleanup;
    }

    if (10 != foundVal)
    {
        printf("Invalid minimum value returned!");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedHeap)
    {
        if (0 != HpDestroy(&usedHeap))
        {
            printf("HpDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;*/
}

int TestItems()
{
    int retGlobal = 0;
    int retVal = -1;

    HtItem* Item = NULL;

    // testing CreateItem
    {
        retVal = CreateItem("Andrei", 19, &Item);
        if (retVal == -1)
        {
            retGlobal = -1;
            printf("CreateItem failed!\n");
            goto cleanup;
        }
    }

cleanup:
    // testing DestroyItem
    {
        retVal = DestroyItem(&Item);
        retVal = retVal == -1 || Item != NULL ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("DestroyItem failed!\n");
        }
    }
    
    return retGlobal;
}
int TestLinkedList()
{
    int retGlobal = 0;
    int retVal = -1;
    int Value = -1;

    HtItem* Item = NULL;
    LinkedList* list = NULL;

   
    // testing InsertLinkedList
    {
        CreateItem("Paul", 19, &Item);
        retVal = InsertLinkedList(&list, Item);
        retVal = retVal == -1 || list->Item->Value != 19 ? -1 : 0;
        if (retVal == -1)
        {
            printf("InsertLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        CreateItem("Andrei", 20, &Item);
        retVal = InsertLinkedList(&list, Item);
        retVal = retVal == -1 || list->Item->Value != 20 || list->Next->Item->Value != 19 ? -1 : 0;
        if (retVal == -1)
        {
            printf("InsertLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        CreateItem("Beji", 31, &Item);
        retVal = InsertLinkedList(&list, Item);
        retVal = retVal == -1 || list->Item->Value != 31 || list->Next->Item->Value != 20 || list->Next->Next->Item->Value != 19 ? -1 : 0;
        if (retVal == -1)
        {
            printf("InsertLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing SearchLinkedList
    {
        retVal = SearchLinkedList(list, "Andrei", &Value);
        retVal = retVal == -1 || Value != 20 ? -1 : 0;
        if (retVal == -1)
        {
            printf("SearchLinkedList 1 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = SearchLinkedList(list, "Beji", &Value);
        retVal = retVal == -1 || Value != 31 ? -1 : 0;
        if (retVal == -1)
        {
            printf("SearchLinkedList 2 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = SearchLinkedList(list, "Paul", &Value);
        retVal = retVal == -1 || Value != 19 ? -1 : 0;
        if (retVal == -1)
        {
            printf("SearchLinkedList 3 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = SearchLinkedList(list, "NUP", &Value);
        if (retVal != -1)
        {
            printf("SearchLinkedList 4 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }
    
    // testing DeleteItemLinkedList
    {
        retVal = DeleteItemLinkedList(&list, "DAP");
        if (retVal != -1)
        {
            printf("DeleteItemLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = DeleteItemLinkedList(&list, "Paul");
        if (retVal == -1)
        {
            printf("DeleteItemLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = DeleteItemLinkedList(&list, "Andrei");
        if (retVal == -1)
        {
            printf("DeleteItemLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        
    }

    // testing ReplaceValueLinkedList
    {
        retVal = ReplaceValueLinkedList(list, "Dap", 14);
        if (retVal != -1)
        {
            printf("ReplaceValueLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = ReplaceValueLinkedList(list, "Beji", 14);
        retVal = retVal == -1 || list->Item->Value != 14 ? -1 : 0;
        if (retVal == -1)
        {
            printf("ReplaceValueLinkedList failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }




cleanup:

    // testing DestroyLinkedList
    {
        retVal = DestroyLinkedList(&list);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("DestroyLinkedList failed!\n");
        }
    }
   
    return retGlobal;
}
int TestHashTable()
{
    
    int retGlobal = 0;
    int retVal = -1;
    int Value = -1;
    char* Key = NULL;


    CC_HASH_TABLE* HashTable = NULL;
    CC_HASH_TABLE* HashTable1 = NULL;

    CC_HASH_TABLE_ITERATOR* Iterator = NULL;
    CC_HASH_TABLE_ITERATOR* Iterator1 = NULL;


    // testing HtCreate
    {
        retVal = HtCreate(&HashTable);
        retVal = retVal == -1 || HashTable->size != 1031 || HashTable->count != 0 ? -1 : 0;
        if (retVal != 0)
        {
            printf("HtCreate failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        //printf("%d\n", HashTable->size);
    }

    // testing HtSetKeyValue
    {
        retVal = HtSetKeyValue(HashTable, "Andrei", 100);
        retVal = retVal == -1 || HashTable->count != 1 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtSetKeyValue 1 failed!\n");
            retGlobal = -1;
            goto cleanup;
     
        }
        retVal = HtSetKeyValue(HashTable, "Beji", 200);
        retVal = retVal == -1 || HashTable->count != 2 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtSetKeyValue 2 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = HtSetKeyValue(HashTable, "Deiu", 300);
        retVal = retVal == -1 || HashTable->count != 3 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtSetKeyValue 3 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = HtSetKeyValue(HashTable, "Deiu", 101);
        retVal = retVal != -1 || HashTable->count != 3 ? -1 : 0;
        //retVal = retVal == -1 || HashTable->count != 3 || HashTable->buckets[HashFunction("Deiu")]->item->value != 101 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtSetKeyValue 4 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        retVal = HtSetKeyValue(HashTable, "Beiu", 102);
        retVal = retVal == -1 || HashTable->count != 4 ? -1 : 0;
        //retVal = retVal == -1 || HashTable->count != 4 || HashTable->buckets[HashFunction("Beiu")]->item->value != 102 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtSetKeyValue 4 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }
    
    // testing HtGetKeyCount
    {
        retVal = HtGetKeyCount(HashTable);
        if (retVal != 4)
        {
            printf("HtGetKeyCount failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = HtGetKeyCount(HashTable1);
        if (retVal != -1)
        {
            printf("HtGetKeyCount failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing HtClear
    {
        retVal = HtClear(HashTable);
        retVal = retVal == -1 || HashTable->count != 0 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtClear failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        HtSetKeyValue(HashTable, "Andrei", 100);
        HtSetKeyValue(HashTable, "Beji", 200);
        HtSetKeyValue(HashTable, "Deiu", 300);
        HtSetKeyValue(HashTable, "Deiu", 101);
        HtSetKeyValue(HashTable, "Beiu", 102);


    }

    // testing HtGetKeyValue
    {
        Value = -1;
        retVal = HtGetKeyValue(HashTable, "Andrei", &Value);
        retVal = retVal == -1 || Value != 100 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtGetKeyValue 1 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        Value = -1;
        retVal = HtGetKeyValue(HashTable, "Deiu", &Value);
        retVal = retVal == -1 || Value != 300 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtGetKeyValue 2 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        Value = -1;
        retVal = HtGetKeyValue(HashTable, "Beji", &Value);
        retVal = retVal == -1 || Value != 200 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtGetKeyValue 3 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
        Value = -1;
        retVal = HtGetKeyValue(HashTable, "Bejan", &Value);
        if (retVal != -1)
        {
            printf("HtGetKeyValue 4 failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing HtRemoveKey
    {
        retVal = HtRemoveKey(HashTable, "Beji");
        retVal = retVal == -1 || HtGetKeyValue(HashTable, "Beji", &Value) != -1 || HashTable->count != 3 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtRemoveKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = HtRemoveKey(HashTable, "Beji");
        retVal = retVal != -1 || HtGetKeyValue(HashTable, "Beji", &Value) != -1 || HashTable->count != 3 ? -1 : 0;
        if (retVal == -1)
        {
            printf("HtRemoveKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing HtHasKey
    {
        retVal = HtHasKey(HashTable, NULL);
        if (retVal != -1)
        {
            printf("HtHasKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = HtHasKey(HashTable, "Beji");
        if (retVal != 0)
        {
            printf("HtHasKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        retVal = HtHasKey(HashTable, "Deiu");
        if (retVal != 1)
        {
            printf("HtHasKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing HtGetFirstKey
    {
        retVal = HtGetFirstKey(HashTable, &Iterator, &Key);
        //printf("%d", HashFunction(Key));
        //printf("%s", Key);
        if (retVal != 0)
        {
            printf("HtGetFirstKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }

        HtCreate(&HashTable1);

        retVal = HtGetFirstKey(HashTable1, &Iterator1, &Key);
        //printf("%d", retVal);
        //printf("%s", Key);
        if (retVal != -2)
        {
            printf("HtGetFirstKey failed!\n");
            retGlobal = -1;
            goto cleanup;
        }
    }

    // testing HtGetNextKey
    {
        /*
        //retVal = HtGetFirstKey(HashTable, &Iterator, &Key);
        retVal = HtGetNextKey(Iterator, &Key);
        //printf("%d", HashFunction(Key));
        //printf("%s", Key);
        retVal = retVal != 0 || strcmp(Key, "Deiu") != 0 ? -1 : 0;
        if (retVal != 0)
        {
            printf("HtGetNextKey 1 failed!\n");
            retGlobal = -1;
            goto cleanup2;
        }

        retVal = HtGetNextKey(Iterator, &Key);
        //printf(Key);
        retVal = retVal != 0 || strcmp(Key, "Beiu") != 0 ? -1 : 0;
        if (retVal != 0)
        {
            printf("HtGetNextKey 2 failed!\n");
            retGlobal = -1;
            goto cleanup2;
        }

        retVal = HtGetNextKey(Iterator, &Key);
        if (retVal != -2)
        {
            printf("HtGetNextKey 3 failed!\n");
            retGlobal = -1;
            goto cleanup2;
        }

        //HtCreate(&HashTable1);

        retVal = HtGetFirstKey(HashTable1, &Iterator1, &Key);
        retVal = HtGetNextKey(Iterator1, &Key);
        //printf("%d\n", retVal);
        if (retVal != -1)
        {
            printf("HtGetNextKey 4 failed!\n");
            retGlobal = -1;
            goto cleanup2;
        }
*/
        /*retVal = HtGetFirstKey(HashTable, &Iterator, &Key);
        while (Iterator!=NULL)
        {
            HtGetKeyValue(HashTable, Key, &Value);
            printf("%s:%d\n", Key, Value);
            if (HtGetNextKey(Iterator, &Key) == -2)
                Iterator = NULL;
        }*/
        
    }


    

    if (NULL != Iterator)
    {
        // testing HtReleaseIterator
        {
            retVal = HtReleaseIterator(&Iterator);
            retVal = retVal != 0 || Iterator != NULL ? -1 : 0;
            if (retVal != 0)
            {
                retGlobal = -1;
                printf("HtReleaseIterator failed!\n");
            }
        }
        
    }
    if (NULL != Iterator1)
    {
        retVal = HtReleaseIterator(&Iterator1);
    }



cleanup:
    if (NULL != HashTable)
    {
        // testing HtDestroy
        {
            retVal = HtDestroy(&HashTable);
            if (retVal != 0)
            {
                retGlobal = -1;
                printf("HtDestroy failed!\n");
            }
        }
        
    }
    if (NULL != HashTable1)
    {
        retVal = HtDestroy(&HashTable1);
    }

    /*Value = -1;
    Key = NULL;
    CC_HASH_TABLE* table = NULL;
    CC_HASH_TABLE_ITERATOR* it = NULL;

    HtCreate(&table);
    char key[100] = "Andy";
    for (int i = 0; i < 30; i++)
    {
        strcat(key, "!");
        HtSetKeyValue(table, key, i);
    }

    HtGetFirstKey(table, &it, &Key);
    while (it != NULL)
    {
        HtGetKeyValue(table, Key, &Value);
        printf("%s:%d\n", Key, Value);
        if (HtGetNextKey(it, &Key) == -2)
            it = NULL;
    }
    HtReleaseIterator(&it);
    strcpy(key, "Andy");
    for (int i = 0; i < 30; i++)
    {
        strcat(key, "!");
        HtGetKeyValue(table, key, &Value);
        printf("%s:%d\n", key, Value);
    }
    HtSetKeyValue(table, "Andy!", 100);
    HtGetKeyValue(table, "Andy!", &Value);
    printf("%s:%d\n", "Andy!", Value);

    HtRemoveKey(table, "Andy!");
    printf("%d\n", HtHasKey(table, "Andy!"));
    printf("%d\n", HtHasKey(table, "Andy!!"));

    HtRemoveKey(table, "Andy!!!!");
    HtRemoveKey(table, "Andy!!!!!");
    HtRemoveKey(table, "Andy!!!!!!");
    HtRemoveKey(table, "Andy!!!!!!!");


    HtGetFirstKey(table, &it, &Key);
    while (it != NULL)
    {
        HtGetKeyValue(table, Key, &Value);
        printf("%s:%d\n", Key, Value);
        if (HtGetNextKey(it, &Key) == -2)
            it = NULL;
    }
    HtReleaseIterator(&it);
    strcpy(key, "Andy");
    for (int i = 0; i < 30; i++)
    {
        strcat(key, "!");
        if (HtGetKeyValue(table, key, &Value) != -1)
        {
            printf("%s:%d\n", key, Value);

        }
    }
    printf("%d\n", HtGetKeyCount(table));

    HtClear(table);
    printf("%d\n", HtGetKeyCount(table));

    strcpy(key, "Andy");
    HtSetKeyValue(table, key, 109);
    HtGetFirstKey(table, &it, &Key);
    while (it != NULL)
    {
        HtGetKeyValue(table, Key, &Value);
        printf("%s:%d\n", Key, Value);
        if (HtGetNextKey(it, &Key) == -2)
            it = NULL;
    }
    HtReleaseIterator(&it);
    HtDestroy(&table);
    printf("%d\n", table != NULL);*/

    return retGlobal;
}
int TestStack()
{
    int retGlobal = 0;
    int retVal = -1;
    int Value = 0;
    CC_STACK* Stiva = NULL;
    CC_STACK* Stiva2 = NULL;


    // testing StCreate
    {
        retVal = StCreate(&Stiva);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StCreate failed!\n");
            goto cleanup;
        }
        StCreate(&Stiva2);
    }
    

    // testing StPush
    {
        retVal = StPush(Stiva, 10);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPush failed!\n");
            goto cleanup;
        }
        StPush(Stiva, 20);
    }
    

    // testing StPop
    {
        retVal = StPop(Stiva, &Value);
        retVal = retVal == -1 || Value != 20 ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPop failed!\n");
            goto cleanup;
        }
        retVal = StPop(Stiva, &Value);
        retVal = retVal == -1 || Value != 10 ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPop failed!\n");
            goto cleanup;
        }
        retVal = StPop(Stiva, &Value);
        if (retVal != -1)
        {
            retGlobal = -1;
            printf("StPop failed!\n");
            goto cleanup;
        }
    }

    // testing StPeek
    {
        retVal = StPeek(Stiva, &Value);
        if (retVal != -1)
        {
            retGlobal = -1;
            printf("StPeek failed!\n");
            goto cleanup;
        }
        StPush(Stiva, 10);
        retVal = StPeek(Stiva, &Value);
        retVal = retVal == -1 || Value != 10 ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPeek failed!\n");
            goto cleanup;
        }
        StPush(Stiva, 20);
        retVal = retVal == -1 || Value != 20 ? -1 : 0;
        retVal = StPeek(Stiva, &Value);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPeek failed!\n");
            goto cleanup;
        }
    }
    

    // testing StIsEmpty
    {
        retVal = StIsEmpty(Stiva);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StIsEmpty failed!\n");
            goto cleanup;
        }
        StPop(Stiva, &Value);
        StPop(Stiva, &Value);

        retVal = StIsEmpty(Stiva);
        if (retVal != 1)
        {
            retGlobal = -1;
            printf("StIsEmpty failed!\n");
            goto cleanup;
        }

    }

    // testing StGetCount
    {
        StPush(Stiva, 10);
        StPush(Stiva, 20);
        retVal = StGetCount(Stiva);
        if (retVal != 2)
        {
            retGlobal = -1;
            printf("StGetCount failed!\n");
            goto cleanup;
        }
        StPop(Stiva, &Value);
        retVal = StGetCount(Stiva);
        if (retVal != 1)
        {
            retGlobal = -1;
            printf("StGetCount failed!\n");
            goto cleanup;
        }
        StPop(Stiva, &Value);
        retVal = StGetCount(Stiva);
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StGetCount failed!\n");
            goto cleanup;
        }

    }


    // testing StClear
    {
        retVal = StClear(Stiva) != 0 || StIsEmpty(Stiva) != 1?-1:0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StClear failed!\n");
            goto cleanup;
        }
        StPush(Stiva, 10);
        StPeek(Stiva, &Value);
        retVal = StClear(Stiva) != 0 || StIsEmpty(Stiva) != 1 || Value != 10 ? -1:0 ;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StClear failed!\n");
            goto cleanup;
        }

     
    }


    // testing StPushStack
    {
        StPush(Stiva2, 3);
        StPush(Stiva2, 2);
        StPush(Stiva2, 1);
        
        retVal = StPushStack(Stiva, Stiva2);
        retVal = retVal != 0 || StGetCount(Stiva) != 3 || StGetCount(Stiva2) != 0 ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPushStack failed!\n");
            goto cleanup;
        }

        StPush(Stiva2, 5);
        StPush(Stiva2, 4);
        StPush(Stiva2, 1);

        retVal = StPushStack(Stiva, Stiva2);
        retVal = retVal != 0 || StGetCount(Stiva) != 6 || StGetCount(Stiva2) != 0 ? -1 : 0;
        if (retVal != 0)
        {
            retGlobal = -1;
            printf("StPushStack failed!\n");
            goto cleanup;
        }
        /*
        while (StIsEmpty(Stiva) == 0)
        {
            StPop(Stiva, &Value);
            printf("%d ", Value);
        }
        printf("\n");
        while (StIsEmpty(Stiva2) == 0)
        {
            StPop(Stiva2, &Value);
            printf("%d ", Value);
        }
        printf("\n");
        */
    }

cleanup:
    if (Stiva != NULL)
    {
        // testing StDestroy
        {
            retVal = StDestroy(&Stiva);
            if (retVal != 0)
            {
                printf("StDestroy failed!\n");
                retGlobal = -1;
            }
        }
    }
    if(Stiva2 != NULL)
        StDestroy(&Stiva2);


    return retGlobal;

    /*
    retVal = -1;
    int foundVal = -1;
    CC_STACK* usedStack = NULL;

    retVal = StCreate(&usedStack);
    if (0 != retVal)
    {
        printf("StCreate failed!\n");
        goto cleanup2;
    }

    retVal = StPush(usedStack, 10);
    if (0 != retVal)
    {
        printf("StPush failed!\n");
        goto cleanup2;
    }

    if (1 == StIsEmpty(usedStack))
    {
        printf("Invalid answer to StIsEmpty!\n");
        retVal = -1;
        goto cleanup2;
    }

    retVal = StPop(usedStack, &foundVal);
    if (0 != retVal)
    {
        printf("StPop failed!\n");
        goto cleanup2;
    }

    if (foundVal != 10)
    {
        printf("Invalid value after pop!\n");
        retVal = -1;
        goto cleanup2;
    }

cleanup2:
    if (NULL != usedStack)
    {
        if (0 != StDestroy(&usedStack))
        {
            printf("StDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
    */
}
int TestVector()
{
    // testing VecCreate
    int retVal = -1;
    CC_VECTOR* Vector = NULL;
    CC_VECTOR* Vector2 = NULL;
    
    retVal = VecCreate(&Vector);

    if (retVal != 0)
    {
        printf("VecCreate failed!\n");
        goto cleanup;
    }
    VecCreate(&Vector2);


    

    // testing VecInsertTail
    for (int i = 0; i <= 100 && retVal != -1; i++)
    {
        retVal = VecInsertTail(Vector, i);
        if (Vector->Array[i] != i)
            retVal = -1;
    }

    retVal = retVal != -1 && Vector->Count == 101 && Vector->Size == 128 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertTail failed!\n");
        goto cleanup;
    }





    // testing VecInsertHead
    for (int i = 0; i <= 100 && retVal != -1; i++)
    {
        retVal = VecInsertHead(Vector, i);
        if (Vector->Array[0] != i)
            retVal = -1;
    }

    retVal = retVal != -1 && Vector->Count == 202 && Vector->Size == 256 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertHead failed!\n");
        goto cleanup;
    }





    // testing VecInsertAfterIndex
    retVal = VecInsertAfterIndex(Vector, -1, 10);
    retVal = retVal != -1 && Vector->Count == 203 && Vector->Array[0] == 10 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertAfterIndex 1 failed!\n");
        goto cleanup;
    }

    retVal = VecInsertAfterIndex(Vector, 0, 20);
    retVal = retVal != -1 && Vector->Count == 204 && Vector->Array[1] == 20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertAfterIndex 2 failed!\n");
        goto cleanup;
    }

    retVal = VecInsertAfterIndex(Vector, 4, -20);
    retVal = retVal != -1 && Vector->Count == 205 && Vector->Array[5] == -20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertAfterIndex 3 failed!\n");
        goto cleanup;
    }

    retVal = VecInsertAfterIndex(Vector, Vector->Count-1, -20);
    retVal = retVal != -1 && Vector->Count == 206 && Vector->Array[Vector->Count-1] == -20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecInsertAfterIndex 4 failed!\n");
        goto cleanup;
    }





    // testing VecRemoveByIndex
    retVal = VecRemoveByIndex(Vector, 0);
    retVal = retVal != -1 && Vector->Count == 205 && Vector->Array[0] == 20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecRemoveByIndex 1 failed!\n");
        goto cleanup;
    }

    retVal = VecRemoveByIndex(Vector, 3);
    retVal = retVal != -1 && Vector->Count == 204 && Vector->Array[3] == -20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecRemoveByIndex 2 failed!\n");
        goto cleanup;
    }

    retVal = VecRemoveByIndex(Vector, Vector->Count-1);
    retVal = retVal != -1 && Vector->Count == 203 && Vector->Array[Vector->Count - 1] != -20 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecRemoveByIndex 3 failed!\n");
        goto cleanup;
    }




    // testing VecGetValueByIndex
    int Value = -1;
    retVal = VecGetValueByIndex(Vector, 0, &Value);
    retVal = retVal != -1 && Vector->Array[0] == Value ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecGetValueByIndex 1 failed!\n");
        goto cleanup;
    }

    retVal = VecGetValueByIndex(Vector, 3, &Value);
    retVal = retVal != -1 && Vector->Array[3] == Value ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecGetValueByIndex 2 failed!\n");
        goto cleanup;
    }

    retVal = VecGetValueByIndex(Vector, Vector->Count - 1, &Value);
    retVal = retVal != -1 && Vector->Array[Vector->Count - 1] == Value ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecGetValueByIndex 3 failed!\n");
        goto cleanup;
    }



    // testing VecGetCount
    retVal = VecGetCount(Vector);
    retVal = retVal == Vector->Count ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecGetCount failed!\n");
        goto cleanup;
    }



    // testing VecSort
    retVal = VecSort(Vector);
    for (int i = 0; i < Vector->Count - 1 && retVal != -1; i++)
        if (Vector->Array[i] < Vector->Array[i + 1])
            retVal = -1;
    if (retVal != 0)
    {
        printf("VecSort failed!\n");
        goto cleanup;
    }



    // testing VecAppend
    for (int i = 0; i < 4; i++)
        VecInsertTail(Vector2, i);

    int len = Vector->Count + 4;
    retVal = VecAppend(Vector, Vector2);
    retVal = retVal != -1 && Vector->Count == len ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecAppend failed!\n");
        goto cleanup;
    }




    // testing VecClear
    retVal = VecClear(Vector);
    retVal = retVal != -1 && Vector->Count == 0 ? 0 : -1;
    if (retVal != 0)
    {
        printf("VecClear failed!\n");
        goto cleanup;
    }

cleanup:
    if (NULL != Vector)
    {    
        // testing VecDestroy
        retVal = VecDestroy(&Vector);
        if (retVal != 0)
        {
            printf("VecDestroy failed!\n");
        }
    }
    if (NULL != Vector2)
    {
        VecDestroy(&Vector2);
    }
    /*
    VecCreate(&Vector);
    VecInsertTail(Vector, 0);
    VecInsertTail(Vector, 1);
    VecInsertTail(Vector, 2);
    VecInsertTail(Vector, 3);
    for (int i = 0; i < Vector->Count; i++)
        printf("%d ", Vector->Array[i]);
    printf("\n");
    VecInsertAfterIndex(Vector, 1, 10);
    for (int i = 0; i < Vector->Count; i++)
        printf("%d ", Vector->Array[i]);
    printf("\n");
    VecDestroy(&Vector);
    */

    return retVal;
}
