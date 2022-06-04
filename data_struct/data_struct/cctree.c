#include "cctree.h"
#include "common.h"

// complete

int TreeCreate(CC_TREE **Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (Tree == NULL)
    {
        return -1;
    }
    CC_TREE* myTree = NULL;
    myTree = (CC_TREE*)malloc(sizeof(CC_TREE));
    if (myTree == NULL)
    {
        return -1;
    }
    memset(myTree, 0, sizeof(*myTree));
    myTree->Count = 0;
    myTree->Root = NULL;
    *Tree = myTree;

    return 0;
}
int DeleteNode(CustomTree** Root)
{
    if (Root == NULL)
    {
        return -1;
    } 
    if (*Root == NULL)
    {
        return -1;
    }
    
    CustomTree* myRoot = *Root;
    if (myRoot == NULL)
    {
        return -1;
    }
    CustomTree* myLeft = myRoot->Left;
    CustomTree* myRight = myRoot->Right;

    DeleteNode(&myLeft);
    DeleteNode(&myRight);
    free(*Root);
    *Root = NULL;

    return 0;
}
int TreeDestroy(CC_TREE **Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    if (Tree == NULL)
    {
        return -1;
    }
    if (*Tree == NULL)
    {
        return -1;
    }
    CC_TREE* myTree = *Tree;
    CustomTree* myRoot = myTree->Root;
    DeleteNode(&myRoot);
    free(*Tree);
    *Tree = NULL;
    
    return 0;
}
int InsertRecursive(CustomTree** Root, CustomTree** NewNode)
{
    if (NewNode == NULL)
    {
        return -1;
    }

    if (*Root == NULL)
    {
        *Root = *NewNode;
        return 0;
    }


    if ((*Root)->Value == (*NewNode)->Value)
    {
        (*Root)->HowMany += (*NewNode)->HowMany;
        DeleteNode(NewNode);
        return 0;
    }
    else
    if ((*Root)->Value < (*NewNode)->Value)
    {
        if ((*Root)->Right == NULL)
        {
            (*Root)->Right = (*NewNode);
            return 0;
        }
        InsertRecursive(&(*Root)->Right, NewNode);
    }
    else
    {
        if ((*Root)->Left == NULL)
        {
            (*Root)->Left = (*NewNode);
            return 0;
        }
        InsertRecursive(&(*Root)->Left, NewNode);
        return 0;
    }
    return 0;
}
int TreeInsert(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Tree == NULL)
    {
        return -1;
    }
    CustomTree* root = Tree->Root;
    CustomTree* node = NULL;
    node = (CustomTree*)malloc(sizeof(CustomTree));
    if (node == NULL)
    {
        return -1;
    }
    Tree->Count++;
    node->Value = Value;
    node->HowMany = 1;
    node->Left = NULL;
    node->Right = NULL;

    InsertRecursive(&root, &node);
    Tree->Root = root;

    return 0;
}

CustomTree* GetParentOfLowestInTree(CustomTree* Root)
{
    if (Root->Left == NULL)
        return NULL;

    while (Root->Left->Left != NULL)
        Root = Root->Left;
    return Root;
}
CustomTree* GetPointerToParentOfThisValue(CustomTree* Root, int Value)
{
    if (Root == NULL)
        return NULL;
    if (Root->Value < Value)
    {
        if (Root->Right == NULL)
            return NULL;
        else
            if (Root->Right->Value == Value)
                return Root;
            else
                return GetPointerToParentOfThisValue(Root->Right, Value);
    }
    else
    {
        if (Root->Left == NULL)
            return NULL;
        else
            if (Root->Left->Value == Value)
                return Root;
            else
                return GetPointerToParentOfThisValue(Root->Left, Value);
    }
}
int TreeRemove(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Tree == NULL || Tree->Root == NULL)
        return -1;
    
    CustomTree* root = Tree->Root;
    
    CustomTree* parentValue = NULL;
    CustomTree* nodeValue = NULL;

    CustomTree* nodDrept = NULL;
    CustomTree* nodStang = NULL;

    CustomTree* replacingNode = NULL;

    if (root->Value == Value)
        nodeValue = root;
    else
    {
        parentValue = GetPointerToParentOfThisValue(root, Value);
        if (parentValue == NULL)
            return -1;
        if (parentValue->Left != NULL && parentValue->Left->Value == Value)
            nodeValue = parentValue->Left;
        else
            nodeValue = parentValue->Right;
    }
    Tree->Count = Tree->Count - nodeValue->HowMany;
    nodDrept = nodeValue->Right;
    nodStang = nodeValue->Left;

    if (nodStang == NULL && nodDrept == NULL)
        replacingNode = NULL; 
    else
    if (nodStang == NULL)
    {
        replacingNode = nodDrept;
    }
    else
    if (nodDrept == NULL)
    {
        replacingNode = nodStang;
    }
    else
    if (nodDrept->Left == NULL)
    {
        replacingNode = nodDrept;
        replacingNode->Left = nodStang;
    }
    else
    {
        CustomTree* parentLowest = GetParentOfLowestInTree(nodDrept);
        CustomTree* lowest = parentLowest->Left;

        parentLowest->Left = lowest->Right;
        lowest->Right = NULL;
        replacingNode = lowest;

        replacingNode->Right = nodDrept;
        replacingNode->Left = nodStang;
    }

    if (root->Value == Value)
        Tree->Root = replacingNode;
    else
    if (parentValue->Left != NULL && parentValue->Left->Value == Value)
        parentValue->Left = replacingNode;
    else
        parentValue->Right = replacingNode;

    free(nodeValue);

    return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if (Tree == NULL)
    {
        return -1;
    }
    CustomTree* root = Tree->Root;
    while (root != NULL && root->Value != Value)
    {
        if (root->Value < Value)
            root = root->Right;
        else
            root = root->Left;
    }
    if (root != NULL)
        return 1;
    return 0;
}
int TreeGetCount(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    if (Tree == NULL)
    {
        return -1;
    }
    return Tree->Count;
}

int GetHeightRecursive(CustomTree* Root)
{
    if (Root == NULL)
    {
        return 0;
    }
    int leftHeight = GetHeightRecursive(Root->Left);
    int rightHeight = GetHeightRecursive(Root->Right);

    return max(leftHeight, rightHeight)+1;
}
int TreeGetHeight(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    if (Tree == NULL)
    {
        return -1;
    }
    int depth = GetHeightRecursive(Tree->Root);
    return depth;
}
int TreeClear(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    if (Tree == NULL)
    {
        return -1;
    }
    DeleteNode(&Tree->Root);
    Tree->Count = 0;
    return 0;
}

int GetNthPreorderRecursive(CustomTree* Root, int *Index, int* Value)
{
    if (Root == NULL)
    {
        return -1;
    }
    *Index = (*Index) - Root->HowMany;
    if (*Index <= 0)
    {
        *Value = Root->Value;
        return 0;
    }
    int ret = GetNthPreorderRecursive(Root->Left, Index, Value);
    if (ret == -1)
        return GetNthPreorderRecursive(Root->Right, Index, Value);
    return 0;
}
int TreeGetNthPreorder(CC_TREE *Tree, int Index, int* Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    Index++;
    if (Tree == NULL || Value == NULL || Index < 1 || Index > Tree->Count)
        return -1;
    
    int ret = GetNthPreorderRecursive(Tree->Root, &Index, Value);
    if (ret == -1)
        return -1;
    return 0;
}

int GetNthInorderRecursive(CustomTree* Root, int* Index, int* Value)
{
    if (Root == NULL)
    {
        return -1;
    }
    int ret = GetNthInorderRecursive(Root->Left, Index, Value);
    if (ret != -1)
        return 0;

    *Index = (*Index) - Root->HowMany;
    if (*Index <= 0)
    {
        *Value = Root->Value;
        return 0;
    }
    return GetNthInorderRecursive(Root->Right, Index, Value);
}
int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    Index++;
    if (Tree == NULL || Value == NULL || Index < 1 || Index > Tree->Count)
        return -1;

    int ret = GetNthInorderRecursive(Tree->Root, &Index, Value);
    if (ret == -1)
        return -1;
    return 0;
}

int GetNthPostorderRecursive(CustomTree* Root, int* Index, int* Value)
{
    if (Root == NULL)
    {
        return -1;
    }
    int ret = GetNthPostorderRecursive(Root->Left, Index, Value);
    if (ret != -1)
        return 0;

    ret = GetNthPostorderRecursive(Root->Right, Index, Value);
    if (ret != -1)
        return 0;

    *Index = (*Index) - Root->HowMany;
    if (*Index <= 0)
    {
        *Value = Root->Value;
        return 0;
    }
    return -1;
}
int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    Index++;
    if (Tree == NULL || Value == NULL || Index < 1 || Index > Tree->Count)
        return -1;

    int ret = GetNthPostorderRecursive(Tree->Root, &Index, Value);
    if (ret == -1)
        return -1;
    return 0;
}

