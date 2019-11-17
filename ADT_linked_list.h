/*
*   this is an ADT linked list containing functions to handle the data structure 
*   the nodes can represent any kind of data and the choise is in the user hands what type of data to store
*/

#ifndef ADT_LIST_HEADER
#define ADT_LIST_HEADER

#include "macros.h"

typedef struct s_listNode *nodePtr;

/*
*   initialize a new list and returning a pointer to its head
*/
nodePtr initNewList();

/*
*   adding a node to the list
*   the function gets a head for the list and a void pointer to any kind of data and stores the data in a new node
*   the function adds to the beginning of the list (which means there's no defaulted order among the list nodes)
*/
nodePtr addToList(nodePtr head, void *data);

/*
*   seraching the list for a specific data
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get two void pointers and compare them and return either 1 if there's a match and 0 otherwise
*/
void *searchList(nodePtr, void *, boolean (*compare)(void *, void *));

/*
*   prints the list into a file
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer as well as a file pointer which to print the data into and print the void type data as the user wishes
*/
void printList(nodePtr, FILE *file, void (*printNode)(void *, FILE *));

/*
*   free all the memory allocated for a specific list 
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer and free its data 
*/
void freeList(nodePtr, void (*freeData)(void *));

/*
*   changes the data in the list (intended to cahnge all the data like increasing all the integers for example etc.)
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a void pointer and change its data as the user's wishes
*/
void changeData(nodePtr, void *, void (*change)(void *, void *));

/*
*   sorting the list 
*   since the nodes can hold any kind of data the user is supposed to also pass as an argument a ptr to a function
*   the function is supposed to get a two void pointers (the list data type) and return true if the first one is smaller than the second one 
*   the sorting proccess has complexity of O(nlgn) n being the list length 
*/
void sortList(nodePtr, boolean (*smallerThan)(void *, void *));

/*
*   returns true if the list is not empty
*/
boolean notEmpty(nodePtr head);

#endif