/*
*   this file represents a structure of entry declarations in an assembly program
*   the structure is maintained using a linked list 
*/

#include "ADT_linked_list.h"
#include "labels_table.h"
#include "entry_table.h"

typedef struct s_entryVar
{
    char varName[LABLES_NAME_LENGTH];
    unsigned int varAddress;
} entryVar;

static void *entriesListHead = NULL; /* a pointer to the head of the entries structure */

/************************** static prototypes declaration **************************/

static entryVarPtr newEntryVar();                          /* creating a new word */
static void freeVar(void *var);                            /* free an entry variable */
static void sortEntries();                                 /* sorting the entries structure according to the addresses of vars */
static void printVar(void *var, FILE *);                   /* print a var in the entries */
static void checkEntryExistence(void *entry, void *exist); /* check if an entry exists in the regular labels table */

/************************** global functions **************************/

/*   initialize a new structure of entries     */
void initEntryStructure()
{
    if (!entriesListHead)
        entriesListHead = initNewList();
    else
        fprintf(stderr, "Cannot re-initialize this structure, Please destroy the structure first\n");
}

/* adding a new entry variable into the entries structure */
void insertEntry(char *entryName, unsigned int address)
{
    entryVarPtr newEntry = newEntryVar();
    strcpy(newEntry->varName, entryName);
    newEntry->varAddress = address;
    addToList(entriesListHead, (void *)newEntry);
}

/* destroying the entries structure */
void destroyEntries()
{
    if (entriesListHead)
        freeList(entriesListHead, &freeVar);
    entriesListHead = NULL;
}

/*   printing the entries into the file passed as an argument   */
void printEntries(FILE *file)
{
    sortEntries();
    printList(entriesListHead, file, &printVar);
}

/* check weather or not there are entry variables in the program */
boolean hasEntries()
{
    return notEmpty(entriesListHead);
}

/* check for every entry in the table it exists in the regular labels table */
boolean checkEntriesExistence()
{
    boolean exist = TRUE;
    if (!hasEntries())
        return TRUE;
    /* uses the change data func to confirm that all the entries in the struct were actually declared as vars in the program */
    changeData(entriesListHead, (void *)&exist, &checkEntryExistence); /* asserts into the exist var if all the entries exist in the reg table */
    return exist;
}
/**********************  static functions ***********************/

/* creating a new variable */
static entryVarPtr newEntryVar()
{
    entryVarPtr newEntry = (entryVarPtr)malloc(sizeof(entryVar));
    if (!newEntry)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    return newEntry;
}

/* free an entry variable */
static void freeVar(void *var)
{
    if (var)
        free(var);
}

/* returns true if the address of src var is smaller than the address of target */
static boolean smallerAddress(void *src, void *target)
{
    return (((entryVarPtr)src)->varAddress < ((entryVarPtr)target)->varAddress) ? TRUE : FALSE;
}

/* sorting the entries structure according to their addresses */
static void sortEntries()
{
    sortList(entriesListHead, &smallerAddress);
}

/* print a var in the entries into the file */
static void printVar(void *var, FILE *file)
{
    fprintf(file, "%-10s\t%.4u\n", ((entryVarPtr)var)->varName, ((entryVarPtr)var)->varAddress);
}

/* check if a single entry exists in the regular labels table */
static void checkEntryExistence(void *entry, void *exist)
{
    *((boolean *)exist) &= (searchVar(((entryVarPtr)entry)->varName)) ? TRUE : FALSE;
    if (*((boolean *)exist))
        ((entryVarPtr)entry)->varAddress = getVarAddress(((entryVarPtr)entry)->varName);
}