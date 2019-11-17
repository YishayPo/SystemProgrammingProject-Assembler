#include "macros.h"
#include "memory_image.h"
#include "labels_table.h"
#include "language_macros.h"
#include "extern_table.h"
#include "entry_table.h"
#include "parsing.h"

/*********************** global variables ***********************/

struct s_label /* for each line define weather it has a label and if so - what its attributes are */
{
    boolean hasLabel;
    char labelName[LINE_LENGTH];
    unsigned int address;
};

char line[LINE_LENGTH];   /* store a line in the file */
unsigned int index;       /* the index in a line */
unsigned int currentLine; /* counting the lines in the program */
FILE *assmeblyFile;       /* a pointer to the file being assembled */

unsigned int ic; /* instructions counter */
unsigned int dc; /* data counter */

extern const char *reservedOpcodes[NUM_OF_OPCODES];
extern const char *reservedDatatypes[NUM_OF_ADDRESSING_METHODS];

usedLabel usedVariables[COMPUTER_CAPACITY]; /* for second scan */

/************************** static prototypes declaration **************************/

boolean openFlie(char *fileName);             /* opens the file */
static void closeFile();                      /* closing the opend file */
static void printOutputFiles(char *fileName); /* prints all orders in the orders array */
static void initProgramMemory();              /* initialize all the structures needed for the assembler */
static boolean fetchNextLine();               /* stores the next input line from the file and initialize the data  */
static boolean assembleNextLine();            /* assemble each line seperately */
/* check if all the variables used in this program also declared
also checks if all the entries declared in the program were also declared as variables */
static boolean secondScan();

/**************************  functions  **************************/

/*
*   calls the functions in order to assemble the file appropriately 
*/
boolean assemble(char *fileName)
{
    boolean succeeded = TRUE;

    freeProgramMemory(); /* free all the memory */
    initProgramMemory(); /* initialize new memory */

    openFlie(fileName); /* opens the assembly file */

    while (fetchNextLine()) /* check one line at a time */
        succeeded &= assembleNextLine();

    /* add the ic counter to all the data vars and memory words */
    editDataAddress(ic);     /* changes the addresses in the variable table */
    separateDataSegment(ic); /* changes the addresses in the memory image */

    if (succeeded) /* no errors were discovered throughout the first scan */
        succeeded &= secondScan();
    closeFile();

    if (succeeded != TRUE)
        return FALSE;

    /* create the output files */

    printOutputFiles(fileName);

    freeProgramMemory();

    return TRUE;
}

/* stores the next input line from the file and initialize the data  */
static boolean fetchNextLine()
{
    int len;
    boolean succeeded = (fgets(line, LINE_LENGTH, assmeblyFile)) ? TRUE : FALSE;
    if (line[(len = strlen(line)) - 1] != '\n') /* add a new line character at the end of a line if there isn't one */
    {
        line[len] = '\n';
        line[len + 1] = '\0';
    }
    index = 0;
    return succeeded;
}

/* assemble each line seperately */
static boolean assembleNextLine()
{
    boolean succeeded = TRUE;
    struct s_label label = {FALSE, "", 0}; /* default values */
    char c, type[LINE_LENGTH];

    c = nextSymbol();

    if (emptyOrCommentLine())
    {
        currentLine++;
        return TRUE;
    }

    if (c == ':')
    {
        label.hasLabel = TRUE;
        nextNonC_Word(label.labelName, ':');
        succeeded &= validLabelName(label.labelName, FALSE);
        skipC_Character(':'); /* skip the ':' character */
    }

    c = nextSymbol();

    if (c == '.')
    {

        skipC_Character('.'); /* skip the dot */
        copyNextWord(type);

        if (!strcmp(type, "define")) /* macro line */
        {
            if (label.hasLabel)
            {
                printErrorMsg("Macro cannot have a label");
                currentLine++;
                return FALSE;
            }
            else
            {
                succeeded &= parseMacroLine();
            }
        }
        if (!strcmp(type, "data"))
        {
            if (label.hasLabel)
                insertVariable(REGULAR_DATA, label.labelName, dc);
            succeeded &= parseDataLine();
        }
        if (!strcmp(type, "string"))
        {
            if (label.hasLabel)
                insertVariable(REGULAR_DATA, label.labelName, dc);
            succeeded &= parseStringLine();
        }
        if (!strcmp(type, "extern"))
        {
            if (label.hasLabel)
                printWarningMessege("Extern variable has a label");
            succeeded &= parseExternLine();
        }
        if (!strcmp(type, "entry"))
        {
            if (label.hasLabel)
                printWarningMessege("Entry variable has a label");
            succeeded &= parseEntryLine();
        }
    }
    else
    {
        copyNextWord(type);

        if (label.hasLabel)
            insertVariable(CODE, label.labelName, ic);
        succeeded &= parseOrderLine(type);
    }
    currentLine++;
    return succeeded;
}

/* frees all the structures */
void freeProgramMemory()
{
    destroyMemory();
    destroyTable();
    destroyMacros();
    destroyEntries();
    destroyExterns();
}

/* initialize all the structures needed for the assembler */
static void initProgramMemory()
{
    initMemoryImage();
    initTable();
    initMacroStructure();
    initEntryStructure();
    initExternStructure();

    ic = STARTING_POINT_OF_ADDRESS;
    dc = ZERO;
    currentLine = 1;

    (usedVariables[0]).addressOfUse = 0;
}

/*
*   adds a suffix to the string passed as an argument and opens the file
*   if the file doesn't exist - prints an error message and returns false
*/
boolean openFlie(char *name)
{
    char temp[LINE_LENGTH];

    strcpy(temp, name);              /* saves the file name in the temp char array */
    strcat(temp, INPUT_FILE_SUFFIX); /* appends the appropriate suffix */
    assmeblyFile = fopen(temp, "r"); /* open the file  for reading purposes */

    if (!assmeblyFile) /* if failed to open */
    {
        fprintf(stderr, "Error: File %s does not exist\n", name);
        return FALSE;
    }

    return TRUE;
}

/* closing the opend file */
static void closeFile()
{
    fclose(assmeblyFile);
}

/* check if all the variables used in this program have been also declared
also checks if all the entries declared in the program were also declared as variables */
static boolean secondScan()
{
    boolean succeeded = TRUE;
    int i = 0;
    while ((usedVariables[i]).addressOfUse && succeeded) /* address zero indicates the end of the array */
    {
        usedVariables[i].labelAddress = getVarAddress(usedVariables[i].labelName);
        if (usedVariables[i].labelAddress == -1)
        {
            fprintf(stderr, "\tError at line %u: Undefined variable name\n", usedVariables[i].lineOfUse);
            return FALSE;
        }
        if (isExternal(usedVariables[i].labelName))
        {
            succeeded &= insertOperandWord(EXTERNAL,
                                           usedVariables[i].labelAddress,
                                           usedVariables[i].addressOfUse);
            insertExtern(usedVariables[i].labelName,
                         usedVariables[i].addressOfUse);
        }
        else
        {
            succeeded &= insertOperandWord(RELOCATABLE,
                                           usedVariables[i].labelAddress,
                                           usedVariables[i].addressOfUse);
        }
        i++;
    }

    /* check if all the entries exist in the table */
    succeeded &= checkEntriesExistence();

    return succeeded;
}

/* prints all orders in the orders array */
static void printOutputFiles(char *fileName)
{
    char file[LINE_LENGTH];
    FILE *objectFile;
    FILE *externFile;
    FILE *entryFile;
    {
        strcpy(file, fileName);
        strcat(file, OUTPUT_OBJECT_SUFFIX);
        objectFile = fopen(file, "w");
        if (!objectFile)
            printExceptionMsg("Cannot create object file");
        printMemory(ic, dc, objectFile);
        fclose(objectFile);
    }

    if (hasExterns())
    {
        strcpy(file, fileName);
        strcat(file, OUTPUT_EXTERN_SUFFIX);
        externFile = fopen(file, "w");
        if (!externFile)
            printExceptionMsg("Cannot create extern file");
        printExterns(externFile);
        fclose(externFile);
    }

    if (hasEntries())
    {
        strcpy(file, fileName);
        strcat(file, OUTPUT_ENTRY_SUFFIX);
        entryFile = fopen(file, "w");
        if (!entryFile)
            printExceptionMsg("Cannot create entry file");
        printEntries(entryFile);
    }
}