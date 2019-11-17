/*
*   this file represents a structure of macros defined in the assembly code
*   the structure is maintained by a linked list 
*/

#include "ADT_linked_list.h"
#include "language_macros.h"

typedef struct s_macro
{
    char macroName[LABLES_NAME_LENGTH];
    int macroValue;
} macro;

void *macroListHead; /* a pointer to the macros structure */

/******************** static prototypes declaration **********************/

static boolean compare(void *, void *); /* comparing between a string and a macro name */
static boolean addMacro(macroPtr);      /* adding a macro into the list */

/************************** global functions **************************/

/*   initialize a new structure of macros    */
boolean initMacroStructure()
{
    if (!macroListHead)
        macroListHead = initNewList();
    else /* if there already is a list assigned to this pointer */
        fprintf(stderr, "Cannot re-allocate the structure, please destroy the old structure first\n");
    return macroListHead != NULL;
}

/*   creating a new macro    */
boolean insertNewMacro(char *macroName, int macroValue)
{
    macroPtr newMacro = (macroPtr)malloc(sizeof(macro));
    if (!newMacro)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    strcpy(newMacro->macroName, macroName);
    newMacro->macroValue = macroValue;
    return (addMacro(newMacro)) ? TRUE : FALSE;
}

/*   searching for a macro    */
boolean macroExist(char *macroName)
{
    return (searchList(macroListHead, macroName, &compare)) ? TRUE : FALSE;
}

/*   checks the value of a macro    */
int getMacroValue(char *macroName)
{
    macroPtr temp = searchList(macroListHead, (void *)macroName, &compare);
    if (temp)
        return temp->macroValue;
    return 0; /* assumes the macro isn't zero */
}

/*   freeing the macros structure    */
void destroyMacros()
{
    freeList(macroListHead, &free);
    macroListHead = NULL;
}

/**********************  static functions ***********************/

/* comparing between a string and a macro name */
static boolean compare(void *macroData, void *macroName)
{
    return (strcmp(((macroPtr)macroData)->macroName, (char *)macroName) == 0) ? TRUE : FALSE;
}

/* adding a macro into the list */
static boolean addMacro(macroPtr newMacro)
{
    return (addToList((void *)macroListHead, newMacro) != NULL) ? TRUE : FALSE;
}