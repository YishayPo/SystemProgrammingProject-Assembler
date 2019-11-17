/*
*   this file provides different functions to handle a line of assmbly language in a file
*/

#include "macros.h"
#include "memory_image.h"
#include "labels_table.h"
#include "language_macros.h"
#include "extern_table.h"
#include "entry_table.h"
#include "data_types.h"

#define goToNth(n)   \
    {                \
        index = (n); \
    }

boolean endOfLine();                             /* reports whether the has reached the current line ending  */
boolean emptyOrCommentLine();                    /* reports whether the current line is a comment or an epmty line */
/* 
*   checks whether str can be a valid label name or not
*   duplacateAllowed indicates weather or not thee is a need to check if the label already exists
*   in case of entry statment for example there is no problem if the variable elready exists in the labels table 
*   returns TRUE or FALSE accordingly
*   the function issues an error messages if needed 
*/
boolean validLabelName(char *str, boolean duplacateAllowed);
boolean isDigit(char *str);                      /* checks if str is astrings of only digits */
boolean hasComma();                              /* checks weather or not the next non-space character is a comma  and skips the comma */
boolean getNum(int *dest);                       /* reads the next number and inserts its value into the pointer passed as an argument */
int searchCharacter(char);                       /* returns the next index of the char 'c' return -1 if 'c' doesn't exist */
char nextSymbol();                               /* returns the index of the next none-digit or alphabetic symbol */
void nextChar();                                 /* returns the current character and moves to the next character */
char copyNextWord(char *str);                    /* copy the next word from the line into str  */
void skipSpaces();                               /* skip spaces characters */
void copyNcharacters(char *str, unsigned int n); /* copy into 'str' the next n characters at the current line */
void skipC_Character(char c);                    /* skipping the character c in string "line" */
void nextNonC_Word(char *str, char c);           /* copy into 'str' the next word 'till 'c' char */
