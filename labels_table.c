/* 
*	this file represents the labels table of regular lables along with their addresses
*   the structure is maintained using a linked list
*/

#include "ADT_linked_list.h" /* using a linked list for maintaining the labels table */
#include "labels_table.h"

typedef struct s_variable
{
    LABEL_TYPE status;    /* indicating weather the label points at an operation, regular variable or an extern one */
    unsigned int address; /* holds the address of the text segment for a code indicating label or the address of a variable in the data segment */
    char varName[LABLES_NAME_LENGTH];
} variable;

static void *labelsTable = NULL; /* pointer to the head of the label list */

/************************** static prototype declarations **************************/

static varPtr newVariable();                   /* allocating space for a new variable */
static boolean compare(void *var, void *name); /* comparing a variablr name to a string passed as an argument */
static void addToAddress(void *, void *);      /* add the ic counter to data addresses */

/************************** global functions **************************/

/* initializing a new table of variables */
void initTable()
{
    if (!labelsTable)
        labelsTable = initNewList();
    else /* if there already is a list assigned to this pointer */
        fprintf(stderr, "Cannot re-allocate the table, please destroy the old table first\n");
}

/* adding a variable */
void insertVariable(LABEL_TYPE stat, char *varName, unsigned int varAddress)
{
    varPtr newVar = newVariable(); /* init new node of label type */
    newVar->status = stat;
    strcpy(newVar->varName, varName);
    newVar->address = varAddress;
    addToList(labelsTable, (void *)newVar); /* insert the new label into the list */
}

/* destroying the table of variables */
void destroyTable()
{
    if (labelsTable)
        freeList(labelsTable, &free);
    labelsTable = NULL;
}

/* searching for a variable in the structures */
varPtr searchVar(char *varName)
{
    return ((varPtr)searchList(labelsTable, (void *)varName, &compare));
}

/* gets an address of a variable by its name - returns -1 if there is no such variable */
int getVarAddress(char *varName)
{
    varPtr var;
    if ((var = searchVar(varName)))
        return var->address;
    return -1; /* if it's an extern var it returns 0 as an address therefore the code for "not found" is -1 */
}

/* adding the ic counter to all the variables that represent a data */
void editDataAddress(unsigned int ic)
{
    changeData(labelsTable, (void *)&ic, &addToAddress);
}

/* checks if a variable is external and return 1 if true, and -1 if there is no such variable */
int isExternal(char *labelName)
{
    varPtr var = searchVar(labelName);
    if (!var)
        return -1;
    else
        return (var->status == EXTERN_DATA) ? TRUE : FALSE;
}

/**********************  static functions ***********************/

/* comparing a variable name to a string passed as an argument */
static boolean compare(void *var, void *name)
{
    return (strcmp(((char *)name), ((varPtr)var)->varName) == 0) ? TRUE : FALSE;
}

/* add the ic counter to data address */
static void addToAddress(void *var, void *ic)
{
    if (var)
        if (((varPtr)var)->status == REGULAR_DATA)
            ((varPtr)var)->address += *((unsigned int *)ic);
}

/* allocating space for a new variable */
static varPtr newVariable()
{
    varPtr newVar = (varPtr)malloc(sizeof(variable));
    if (!newVar)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    return newVar;
}