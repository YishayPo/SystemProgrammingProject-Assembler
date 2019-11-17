/*
*   this file is intended to parse an instruction or macro lines
*/

#include "parsing.h"

/*********************** global variables ***********************/

extern char line[LINE_LENGTH];   /* holds the current line which now is being assembled */
extern unsigned int index;       /* the index which the line has already been assembled up untill that index */
extern unsigned int currentLine; /* counting the lines in the program */

extern unsigned int dc; /* data counter */

/**************************  functions  **************************/

/* parse & checks if a line is a valid macro definition */
boolean parseMacroLine()
{
    boolean succeeded = TRUE;
    char macroName[LINE_LENGTH];
    char c;
    int macroValue;

    copyNextWord(macroName);
    c = nextSymbol();

    if (c != '=') /* Missing equality sign */
    {
        printErrorMsg("Missing equality sign");
        succeeded = FALSE;
    }
    else
    {
        succeeded &= validLabelName(macroName, FALSE);
        skipSpaces();
        skipC_Character('='); /* skip the equality sign */
        succeeded &= (succeeded) ? getNum(&macroValue) : succeeded;
    }
    if (!endOfLine()) /* if the line doesn't end */
    {
        printErrorMsg("Extraneous text");
        succeeded = FALSE;
    }
    if (succeeded)
        insertNewMacro(macroName, macroValue);
    return succeeded;
}

/*checks if an instruction of type data line is valid */
boolean parseDataLine()
{
    boolean succeeded = TRUE;
    boolean commaFlag;
    int dataValue;

    if (hasComma())
    {
        printErrorMsg("Invalid comma");
        return FALSE;
    }

    while ((!endOfLine()) && succeeded)
    {
        succeeded &= getNum(&dataValue);
        if ((commaFlag = hasComma()) && endOfLine())
        {
            printErrorMsg("Comma at the end of line");
            succeeded = FALSE;
        }
        else if (!commaFlag && (!endOfLine()))
        {
            printErrorMsg("Missing comma");
            succeeded = FALSE;
        }
        else if (commaFlag)
        {
            if (hasComma())
            {
                printErrorMsg("Multiple commas betweeen values");
                succeeded = FALSE;
            }
        }
        if (succeeded)
            succeeded &= insertDataWord(dataValue, dc++);
    }
    return succeeded;
}

/* checks if an insruction of type string is valid */
boolean parseStringLine()
{
    boolean succeeded = TRUE;
    char string[LINE_LENGTH];
    char c;
    int i = 0;

    if (nextSymbol() != '\"')
    {
        printErrorMsg("Invalid string - string must start with a quotation mark");
        return FALSE;
    }

    skipC_Character('\"');

    if (nextSymbol() != '\"')
    {
        printErrorMsg("Invalid string - string must end with a quotation mark");
        return FALSE;
    }
    copyNextWord(string);
    skipC_Character('\"');
    if (!endOfLine())
    {
        printErrorMsg("Extraneous text");
        return FALSE;
    }
    if (succeeded)
        do
        {
            succeeded &= insertDataWord((unsigned int)(c = string[i++]), dc++);
        } while ((c != '\0') && succeeded);

    /* while (((c = string[i++]) != '\0') && succeeded)
        succeeded &= insertDataWord((unsigned int)c, dc++);*/

    return succeeded;
}

/* checks if an extern line is valid */
boolean parseExternLine()
{
    boolean succeeded = TRUE;
    char externName[LINE_LENGTH];

    if (endOfLine())
    {
        printErrorMsg("Missing the extern name)");
        return FALSE;
    }
    copyNextWord(externName);

    if (!endOfLine())
    {
        printErrorMsg("Extraneous text");
        return FALSE;
    }
    succeeded &= validLabelName(externName, FALSE);

    if (succeeded)
        insertVariable(EXTERN_DATA, externName, ZERO);

    return succeeded;
}

/* checks if an entry line is valid */
boolean parseEntryLine()
{
    boolean succeeded = TRUE;
    char entryName[LINE_LENGTH];

    if (endOfLine())
    {
        printErrorMsg("Missing entry name)");
        return FALSE;
    }
    copyNextWord(entryName);

    if (!endOfLine())
    {
        printErrorMsg("Extraneous text");
        return FALSE;
    }
    succeeded &= validLabelName(entryName, TRUE);
    succeeded &= endOfLine();
    if (succeeded)
        insertEntry(entryName, ZERO); /* in the second scam the assembler will check if the entry exists and change its address */

    return succeeded;
}