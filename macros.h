/*
 * This file includes all the global macros of the program: constants, string messeges, macro functions
 * prototypes of input and output handling functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef MACROS_H_
#define MACROS_H_

typedef enum
{
    FALSE,
    TRUE
} boolean;

/* gneral macros */
#define COMPUTER_CAPACITY 4096 /* The imaginary computer memory size*/
#define LINE_LENGTH 81         /* the length is 80 plus the '\n' character */
#define LABLES_NAME_LENGTH 32
#define STARTING_POINT_OF_ADDRESS 100
#define NUM_OF_RESERVED_DATATYPE 4
#define ZERO 0
#define ONE_OPERAND 1
#define TWO_OPERANDS 2
#define FIRST 1
#define SECOND 2

#define INPUT_FILE_SUFFIX ".as"
#define OUTPUT_OBJECT_SUFFIX ".ob"
#define OUTPUT_EXTERN_SUFFIX ".ext"
#define OUTPUT_ENTRY_SUFFIX ".ent"

/* error and exceptions messeges */
#define MULTIPLE_COMMAS_ERROR "Multiple consequtive commas between arguments"
#define INVALID_LABLE_ERROR "Invalid label name: Illegal name for lables"
#define ALREADY_DECLARED_ERROR "Invalid label name: The lable has already been declared"
#define RESERVES_WORD_LABLE_ERROR "Invalid label name: This name is a special reserved word"
#define UNDECLARED_LABLE_ERROR "An undeclared lable use"
#define NO_SUCH_FILE_ERROR "The file does not exist"
#define MISSING_PARAMETER_ERROR "Missing parameter"
#define ALLOCATION_FALIURE "Memory allocation faliure"

#define printErrorMsg(msg) fprintf(stderr, "\tError at line %u: %s\n", currentLine, msg)

#define printExceptionMsg(msg)                                                  \
    {                                                                           \
        fprintf(stderr, "Exception: %s. The program has been terminated", msg); \
        freeProgramMemory();                                                    \
        exit(1);                                                                \
    }

#define printWarningMessege(msg) printf("\tWarning: at line %u: %s\n", currentLine, msg)


void freeProgramMemory(); /* frees all the structures */
/* calls the functions in order to assemble the file appropriately */
boolean assemble(char *fileName);
/* adds a suffix to the string passed as an argument and opens the file
if the file doesn't exist - prints an error message and returns false */
boolean openFlie(char *name);

#endif