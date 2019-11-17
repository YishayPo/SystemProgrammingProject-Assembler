/*
*   this is an ADT linked list contains functions to handle a data structure of a linked list type
*   in this list the head of the list holds nothing but a pointer to the list first node
*   the nodes can represent any kind of data and the choise is in the user hands what type of data to store
*/

#include "ADT_linked_list.h"

/* basic data type for nodes in the list */
typedef struct s_listNode
{
    void *data;
    struct s_listNode *next;
} listNode;

/************************** static prototypes declaration **************************/

/*   returns the node's data    */
static void *getData(nodePtr node);
/* returns the next node after the argument */
static nodePtr getNext(nodePtr node);
/* sets the node's data to be the second argument */
static void setData(nodePtr node, void *data);
/* sets the next node to be the second argument */
static void setNext(nodePtr node, nodePtr nextNode);
/* spliting a list and returning a pointer to the second list */
static nodePtr split(nodePtr);
/* merging two lists in descending order */
static nodePtr merge(nodePtr list1, nodePtr list2, boolean (*smallerThan)(void *, void *));
/* sorting with a mergeSort algorithm O(nlgn) operations - n being the number of nodes in the list */
static nodePtr mergeSort(nodePtr, boolean (*smallerThan)(void *, void *));

/************************** global functions **************************/

/*
*   initialize a new list and returning a pointer to its head
*/
nodePtr initNewList()
{
    nodePtr head = (nodePtr)malloc(sizeof(listNode)); /* allocate memory for a new list head */
    if (!head)
    {
        printExceptionMsg(ALLOCATION_FALIURE) /* terminate in case of allocation failure */
    }
    setData(head, NULL); /* the head of a list always holds nothing but a pointer to the rest of the list */
    setNext(head, NULL); /* assures that the last node will always point to NULL */
    return head;
}

/*
*   adding a node to the list
*   the function gets a head for the list and a void pointer to any kind of data and stores the data in a new node
*   the function adds to the beginning of the list (the next node after the head)
*/
nodePtr addToList(nodePtr head, void *data)
{
    nodePtr temp;
    nodePtr newNode = (nodePtr)malloc(sizeof(listNode));
    if (!newNode)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    newNode->data = data;
    temp = getNext(head);
    setNext(head, newNode);
    setNext(newNode, temp);
    return newNode;
}

/*
*   searching the list for a specific data
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get two void pointers and compare them and return either 1 if there's a match and 0 otherwise
*/
void *searchList(nodePtr head, void *data, boolean (*compare)(void *a, void *b))
{
    while (head)
    {
        if (getData(head))
            if (compare(getData(head), data))
                return getData(head);
        head = getNext(head);
    }
    return NULL;
}

/*
*   returns boolean value indicating if the list is empty
*/
boolean notEmpty(nodePtr head)
{
    if (head)
        return head->next != NULL;
    return FALSE;
}

/*
*   prints the list into a file
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer as well as a file pointer which to print the data into and print the void type data as the user wishes
*/
void printList(nodePtr head, FILE *file, void (*printNode)(void *, FILE *))
{
    while (head)
    {
        if (getData(head))
            printNode(getData(head), file);
        head = getNext(head);
    }
}

/*
*   free all the memory allocated for a specific list 
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer and free its data 
*/
void freeList(nodePtr head, void (*freeData)(void *))
{
    if (head)
    {
        if (getData(head))
            freeData(getData(head));
        if (getNext(head))
            freeList(getNext(head), freeData); /* recursively free the rest of the list */
        free(head);
    }
}

/*
*   changes the data in the list (intended to change all the data like increasing all the integers for example etc.)
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer and change its data as the user wishes
*/
void changeData(nodePtr head, void *valToAdd, void (*change)(void *, void *))
{
    while (head)
    {
        if (getData(head))
            change(getData(head), valToAdd);
        head = getNext(head);
    }
}

/*
*   sorting the list 
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a two void pointers (the list data type) and return true if the first one is smaller than the second one 
*/
void sortList(nodePtr head, boolean (*smallerThan)(void *, void *))
{
    /* strating from the second element since the first one is just the head */
    setNext(head, mergeSort(getNext(head), smallerThan)); /* align the sorted list to its head */
}

/**********************  static functions ***********************/

/* sets the next node to be the second argument */
static void setNext(nodePtr node, nodePtr nextNode)
{
    node->next = (struct s_listNode *)nextNode;
}

/* sets the node's data to be the second argument */
static void setData(nodePtr node, void *data)
{
    node->data = data;
}

/* returns the next node after the argument */
static nodePtr getNext(nodePtr node)
{
    return (struct s_listNode *)node->next;
}

/*   returns the node's data    */
static void *getData(nodePtr node)
{
    return node->data;
}

/* spliting a list and returning a pointer to the second list 
* used for merge sort */
static nodePtr split(nodePtr head)
{
    nodePtr list2;
    if (head == NULL || getNext(head) == NULL)
        return NULL;
    list2 = getNext(head);
    setNext(head, getNext(list2));
    setNext(list2, split(getNext(list2)));
    return list2;
}

/* merging two lists in descending order */
static nodePtr merge(nodePtr list1, nodePtr list2, boolean (*smallerThan)(void *, void *))
{
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;
    if (smallerThan(getData(list1), getData(list2)))
    {
        setNext(list1, merge(getNext(list1), list2, smallerThan));
        return list1;
    }
    else
    {
        setNext(list2, merge(getNext(list2), list1, smallerThan));
        return list2;
    }
}

/* sorting with a mergeSort algorithm */
static nodePtr mergeSort(nodePtr head, boolean (*smallerThan)(void *, void *))
{
    nodePtr list1;
    nodePtr list2;
    if (head == NULL || getNext(head) == NULL)
        return head; /* checks for empty or single node in the list */
    list1 = head;
    list2 = split(list1);
    list1 = mergeSort(list1, smallerThan);
    list2 = mergeSort(list2, smallerThan);
    return merge(list1, list2, smallerThan);
}
