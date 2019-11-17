/*
*   this file represents the memory image of the assembly program
*   as well as providing different function to handle it
*/

#include "ADT_linked_list.h" /* using a linked list for maintaining the memory image */
#include "memory_image.h"

typedef struct s_memoryCell
{
    wordPtr cell;
    unsigned int cellAddress;
} singleCell;

void *memoryListHead = NULL;      /* pointer to the memory structure */
static unsigned int wordsCounter; /* counts the number of words */

/************************** static prototypes declaration **************************/

static cellPtr newWord();                                       /* creating a new word */
static void freeCell(void *);                                   /* free a cell in the memory */
static void printCell(void *, FILE *);                          /* print a cell in the memory */
static void sortMemory();                                       /* sorting the memory structure according to the addresses of cells */
static boolean withinMemoryLimitis();                           /* makes sure the program doesn't require more memory than there is */
static void changeInstructionCellAddress(void *cell, void *ic); /* changing a data word address to be its address plus the ic counter */

/************************** global functions **************************/

/*   initialize a new structure of memory image     */
void initMemoryImage()
{
    if (!memoryListHead)
    {
        memoryListHead = initNewList();
        wordsCounter = 0;
    }
    else
        fprintf(stderr, "Cannot re-allocate new block of memory, Please destroy the old block first\n");
}

/*   allocating new order word in the computer memory for an order as well as defining the first word of order    */
boolean insertOrderWord(unsigned int address, ADDRESSING_METHOD src, ADDRESSING_METHOD dest, OPCODE op)
{
    cellPtr newCell = newWord();
    newCell->cellAddress = address;
    defineWordType(newCell->cell, FIRST_WORD_OF_ORDER);
    insertEncodingType(newCell->cell, ABSOLUTE); /* first word is wlways absolute */
    insertSrcAddressMethod(newCell->cell, src);
    insertTargetAddressMethod(newCell->cell, dest);
    insertOpcode(newCell->cell, op);
    addToList(memoryListHead, (void *)newCell);
    return withinMemoryLimitis();
}

/*   allocating new instruction word in the computer memory for an instruction      */
boolean insertDataWord(int value, unsigned int address)
{
    cellPtr newCell = newWord();
    newCell->cellAddress = address;
    defineWordType(newCell->cell, INSTRUCTION_WORD);
    insertDataValue(newCell->cell, value);
    addToList(memoryListHead, (void *)newCell);
    return withinMemoryLimitis();
}

/*   add a word of direct value or an address      */
boolean insertOperandWord(ENCODING_TYPE a_r_e, int value, int address)
{
    cellPtr newCell = newWord();
    newCell->cellAddress = address;
    defineWordType(newCell->cell, OPERAND_DIRECT_VALUE);
    insertEncodingType(newCell->cell, a_r_e);
    insertOrderValue(newCell->cell, value);
    addToList(memoryListHead, (void *)newCell);
    return withinMemoryLimitis();
}

/*   add a word of registers addresses     */
boolean insertOperandRegisterWord(REGISTERS src, REGISTERS dest, int address)
{
    cellPtr newCell = newWord();
    newCell->cellAddress = address;
    defineWordType(newCell->cell, OPERAND_REGISTERS);
    insertEncodingType(newCell->cell, ABSOLUTE);
    insertSrcRegister(newCell->cell, src);
    insertTargetRegister(newCell->cell, dest);
    addToList(memoryListHead, (void *)newCell);
    return withinMemoryLimitis();
}

/*   add to all the data words the ic counter to separate them from order words   */
void separateDataSegment(unsigned int ic)
{
    changeData(memoryListHead, (void *)&ic, &changeInstructionCellAddress);
}

/*   free all the memory allocated by a program     */
void destroyMemory()
{
    if (memoryListHead)
        freeList(memoryListHead, &freeCell);
    memoryListHead = NULL;
    wordsCounter = 0;
}

/*   printing the memory in a special four basic way    */
void printMemory(unsigned int ic, unsigned int dc, FILE *file)
{
    fprintf(file, "\t%u %u\t", (ic - 100), dc);
    sortMemory();
    printList(memoryListHead, file, &printCell);
}

/**********************  static functions ***********************/

/* creating a new word */
static cellPtr newWord()
{
    cellPtr newCell = (cellPtr)malloc(sizeof(singleCell));
    if (!newCell)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    newCell->cell = allocateNewWord();
    return newCell;
}

/* changing a data word address to be its address plus the ic counter */
static void changeInstructionCellAddress(void *cell, void *ic)
{
    if (cell)
        if (getWordType(((cellPtr)cell)->cell) == INSTRUCTION_WORD)
            ((cellPtr)cell)->cellAddress += *((int *)ic);
}

/* free a cell in the memory */
static void freeCell(void *cell)
{
    if (cell)
    {
        deleteWord(((cellPtr)cell)->cell);
        free(cell);
    }
}

/* returns true if the address of src cell is smaller than the address of target */
static boolean smallerAddress(void *src, void *dest)
{
    return ((src != NULL) &&
            (dest != NULL) &&
            ((cellPtr)src)->cellAddress < ((cellPtr)dest)->cellAddress);
}

/* sorting the memory structure according to the cells' addresses */
static void sortMemory()
{
    sortList(memoryListHead, &smallerAddress);
}

/* print a cell in the memory */
static void printCell(void *cell, FILE *file)
{
    fprintf(file, "\n");
    fprintf(file, "%.4u\t", ((cellPtr)cell)->cellAddress);
    printWord(((cellPtr)cell)->cell, file);
}

/* makes sure the program doesn't require more memory than there is */
static boolean withinMemoryLimitis()
{
    wordsCounter++;
    return wordsCounter < COMPUTER_CAPACITY;
}