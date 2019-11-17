/*
*   this file represents a structure of macros defined in the assembly code
*/

#ifndef LANGUAGE_MACROS_HEADER
#define LANGUAGE_MACROS_HEADER

#include "macros.h"

typedef struct s_macro *macroPtr;

/*   initialize a new structure of macros    */
boolean initMacroStructure();

/*   creating a new macro    */
boolean insertNewMacro(char *macroName, int macroValue);

/*   searching for a macro    */
boolean macroExist(char *macroName);

/*   checks the value of a macro    */
int getMacroValue(char *macroName);

/*   freeing the macros structure    */
void destroyMacros();

#endif