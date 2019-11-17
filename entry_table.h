/*
*   this file represents a structure of entry declarations in an assembly program
*/

#include "macros.h"

#ifndef ENTRIES_TABLE_H
#define ENTRIES_TABLE_H

typedef struct s_entryVar *entryVarPtr;

/* initialize a new structure of entries */
void initEntryStructure();

/* adding a new entry variable into the entries structure */
void insertEntry(char *entryName, unsigned int address);

/* destroying the entries structure */
void destroyEntries();

/*   printing the entries into the file passed as an argument   */
void printEntries(FILE *);

/* check weather or not there are entry variables in the program */
boolean hasEntries();

/* check for every entry in the table if it exists in the regular labels table */
boolean checkEntriesExistence();

#endif