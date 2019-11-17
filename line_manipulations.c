/*
*   this file provides different functions to handle a line of assmbly language in a file
*/

#include "macros.h"
#include "reserved_words.h"
#include "line_manipulations.h"

/*********************** global variables ***********************/

extern char line[LINE_LENGTH];
extern unsigned int index;
extern unsigned int currentLine; /* counting the lines in the program */
extern FILE *assmeblyFile;

extern unsigned int ic; /* instruction counter */
extern unsigned int dc; /* data counter */

/************************** global functions **************************/

/* reads the next number and inserts its value into the pointer passed as an argument */
boolean getNum(int *dest)
{
    int num;
    char c, buffer[LINE_LENGTH];
    boolean negativeNumberFlag = FALSE;
    c = nextSymbol(); /* c contains the next non-alphabetic or digit character */

    if (c == '+' || c == '-')
    {
        negativeNumberFlag = (c == '-');
        skipC_Character((c == '-') ? '-' : '+'); /* skip the sign */
    }

    c = copyNextWord(buffer);

    if (c == '.') /* checks weather or not the number contains a decimal point */
    {
        skipC_Character('.');
        printErrorMsg("This particular machine doesn't support a floating point number");
        return FALSE;
    }

    if (isDigit(buffer))
        num = atoi(buffer);
    else if (macroExist(buffer))
        num = getMacroValue(buffer);
    else
    {
        printErrorMsg("Invalid number");
        return FALSE;
    }

    (*dest) = num * ((negativeNumberFlag) ? -1 : 1);

    return TRUE;
}

/* 
*   checks whether str can be a valid label name or not
*   duplacateAllowed indicates weather or not thee is a need to check if the label already exists
*   in case of entry statment for example there is no problem if the variable elready exists in the labels table 
*   returns TRUE or FALSE accordingly
*   the function issues an error messages if needed 
*/
boolean validLabelName(char *str, boolean duplacateAllowed)
{
    int i;

    /* checks if lable's name starts with an alphabetic character */
    if (!isalpha(str[0]))
    {
        printErrorMsg(INVALID_LABLE_ERROR);
        return FALSE;
    }
    /* checks if the lable's name only includes alphabetic or digits characters */
    for (i = 0; str[i]; i++)
    {
        if (!isalnum(str[i]))
        {
            printErrorMsg(INVALID_LABLE_ERROR);
            return FALSE;
        }
    }
    /* checks the lable isn't a reserve word */
    for (i = 0; i < NUM_OF_OPCODES; i++) /* comparing with opcodes names */
    {
        if (!strcmp(str, reservedOpcodes[i]))
        {
            printErrorMsg(RESERVES_WORD_LABLE_ERROR);
            return FALSE;
        }
    }
    for (i = 0; i < NUM_OF_RESERVED_DATATYPE; i++) /* comparing with reserved datatypes */
    {
        if (!strcmp(str, reservedDatatypes[i]))
        {
            printErrorMsg(RESERVES_WORD_LABLE_ERROR);
            return FALSE;
        }
    }
    if (strlen(str) == 2 && str[0] == 'r' && str[1] >= '0' && str[1] <= '7') /* comparing with registers names */
    {
        printErrorMsg(RESERVES_WORD_LABLE_ERROR);
        return FALSE;
    }
    if (!duplacateAllowed && searchVar(str)) /* the lable name already exists and it's not allowed */
    {
        printErrorMsg(ALREADY_DECLARED_ERROR);
        return FALSE;
    }

    return (strlen(str) <= LABLES_NAME_LENGTH) ? TRUE : FALSE;
}

/* skip space characters */
void skipSpaces()
{
    while (isspace(line[index]))
        index++;
}

/* returns the index of the next none-digit or alphabetic symbol 
this function does not move the index, just checking what the next char is */
char nextSymbol()
{
    int i = index;
    while (isalnum(line[i]) || isspace(line[i]))
        i++;
    return line[i];
}

/* copy into 'str' the next word untill 'c' char appears */
void nextNonC_Word(char *str, char c)
{
    int i = 0;
    skipSpaces();
    while ((str[i++] = line[index++]) != c)
        ;
    str[i - 1] = '\0';
}

/* returns the current character and moves to the next character */
void nextChar()
{
    if (line[index] == '\0')
        return;
    index++;
}

/* reports whether the proccessing has reached the current line ending  */
boolean endOfLine()
{
    skipSpaces();
    return line[index] == '\0';
}

/* reports whether the current line is a comment or an epmty line */
boolean emptyOrCommentLine()
{
    skipSpaces();
    return (line[index] == ';' || endOfLine());
}

/* checks if str is a strings of only digits */
boolean isDigit(char *str)
{
    int i = 0;
    while (str[i] >= '0' && str[i] <= '9')
        i++;
    return str[i] == '\0';
}

/* 
*   copy the next word from the line into str 
*   word is every character that's not a space or a comma or an equal sign
*/
char copyNextWord(char *str)
{
    int i;

    if (endOfLine())
        return line[index - 1];
    i = index;
    while (isalnum(line[i]))
        i++;
    copyNcharacters(str, (i - index) + 1);
    goToNth(i /*(line[i] == ',') ? i : i + 1*/); /* avoids losing a comma */
    return line[i];
}

/* 
*   checks weather or not the next character is a comma 
*   if it is a comma skips the comma and returns true
*/
boolean hasComma()
{
    skipSpaces();
    if (line[index] == ',')
    {
        skipC_Character(',');
        return TRUE;
    }
    return FALSE;
}

/* copying the next n characters of "line" into "str" */
void copyNcharacters(char *str, unsigned int n)
{
    strncpy(str, line + index, n);
    str[n - 1] = '\0';
    goToNth(index + n)
}

/* 
*   returns the next index of the char 'c' in 'line' string
*   return -1 if 'c' doesn't exist in the 'line' string 
*/
int searchCharacter(char c)
{
    int i = index;
    while (line[i])
    {
        if (line[i] == c)
            return i;
        i++;
    }
    return -1;
}

/* skipping the character c in string "line" */
void skipC_Character(char c)
{
    index = (searchCharacter(c) != -1) ? searchCharacter(c) : index;
    nextChar();
}