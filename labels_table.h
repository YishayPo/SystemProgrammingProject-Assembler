/* 
*	this file represents the labels table of regular lables along with their addresses 
*/

#ifndef LABELS_TABLE_H_
#define LABELS_TABLE_H_

#include "macros.h"

typedef enum
{
	REGULAR_DATA,
	EXTERN_DATA,
	CODE
} LABEL_TYPE;

typedef struct s_variable *varPtr;

/*   initializing a new table of variables	*/
void initTable();

/*   adding a variable	*/
void insertVariable(LABEL_TYPE, char *varName, unsigned int varAddress);

/*   destroying the table of variables	*/
void destroyTable();

/*   searching for a variable in the table	*/
varPtr searchVar(char *varName);

/* gets an address of a variable by its name - returns -1 if there is no such variable */
int getVarAddress(char *varName);

/*   adding the ic counter to all the variables that represent a data	*/
void editDataAddress(unsigned int ic);

/* checks if a variable is external and return 1 if true, and -1 if there is no such variable */
int isExternal(char *labelName);

#endif