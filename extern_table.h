/*
*   this file represents a structure of externs declarations in an assembly program
*/

#include "macros.h"

#ifndef EXTERNS_TABLE_H
#define EXTERNS_TABLE_H

typedef struct s_externVar *externVarPtr;

/*   initialize a new structure of externs     */
void initExternStructure();

/* adding a new extern variable into the externs structure */
void insertExtern(char *externName, unsigned int address);

/* destroying the externs structure */
void destroyExterns();

/*   printing the extern variables into the file passed as an argument */
void printExterns(FILE *);

/* check weather or not there are externs variables in the program */
boolean hasExterns();

#endif