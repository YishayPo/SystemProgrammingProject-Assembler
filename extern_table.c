/*
*   this file represents a structure of externs declarations in an assembly program
*   the structure is maintained using a linked list 
*/

#include "ADT_linked_list.h"
#include "extern_table.h"

typedef struct s_externVar
{
    char varName[LABLES_NAME_LENGTH];
    unsigned int varAddress;
} externVar;

static void *externsListHead = NULL; /* a pointer to the head of the externs structure */

/************************** static prototypes declaration **************************/

static externVarPtr newExternVar();      /* creating a new variable */
static void freeVar(void *var);          /* free an extern variable */
static void sortExterns();               /* sorting the externs structure according to the addresses of vars */
static void printVar(void *var, FILE *); /* print a var in the externs */

/************************** global functions **************************/

/* initialize a new structure of externs */
void initExternStructure()
{
    if (!externsListHead)
        externsListHead = initNewList();
    else
        fprintf(stderr, "cannot re-initialize this structure, Please destroy the structure first\n");
}

/* adding a new extern variable into the extern structure */
void insertExtern(char *externName, unsigned int address)
{
    externVarPtr newExtern = newExternVar();
    strcpy(newExtern->varName, externName);
    newExtern->varAddress = address;
    addToList(externsListHead, (void *)newExtern);
}

/* destroying the externs structure */
void destroyExterns()
{
    if (externsListHead)
        freeList(externsListHead, &freeVar);
    externsListHead = NULL;
}

/*   printing the externs into the file */
void printExterns(FILE *file)
{
    sortExterns();
    printList(externsListHead, file, &printVar);
}

/* check weather or not there are externs variables in the program */
boolean hasExterns()
{
    return notEmpty(externsListHead);
}

/**********************  static functions ***********************/

/* creating a new variable */
static externVarPtr newExternVar()
{
    externVarPtr newExtern = (externVarPtr)malloc(sizeof(externVar));
    if (!newExtern)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    return newExtern;
}

/* free an extern variable */
static void freeVar(void *var)
{
    if (var)
        free(var);
}

/* returns true if the address of src var is smaller than the address of target */
static boolean smallerAddress(void *src, void *target)
{
    return (((externVarPtr)src)->varAddress < ((externVarPtr)target)->varAddress) ? TRUE : FALSE;
}

/* sorting the externs structure according to the addresses of vars */
static void sortExterns()
{
    sortList(externsListHead, &smallerAddress);
}

/* print a var in the externs */
static void printVar(void *var, FILE *file)
{
    fprintf(file, "%-10s\t%u\n", /* LABLES_NAME_LENGTH, */ ((externVarPtr)var)->varName, ((externVarPtr)var)->varAddress);
}