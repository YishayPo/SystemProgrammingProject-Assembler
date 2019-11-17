/*
 *  this file represents a single word in the computer memory
 *  as well as functions to handle a single memory word
*/

#include "single_memory_word.h"

#define ARE_FIELD_SIZE 2
#define ADDRESS_METHOD_FIELD_SIZE 2
#define OPCODE_FIELD_SIZE 4
#define VALUE_FIELD_SIZE 12
#define REGISTER_FIELD_SIZE 3
#define WORD_SIZE 14

#define ARE_POSITION 13, 14
#define SRC_ADDRESS_METHOD_POSITION 9, 10
#define SRC_REGISTER_POSITION 7, 9
#define TARGET_ADDRESS_METHOD_POSITION 11, 12
#define TARGET_REGISTER_POSITION 10, 12
#define OPCODE_POSITION 5, 8
#define VALUE_POSITION 1, 12
#define INSTRUCTION_WORD_POSITION 1, 14

typedef struct s_singleMemoryWord
{
    KIND_OF_WORD wordType;
    char BinaryRep[WORD_SIZE];
} singleMemoryWord;

/************************** static prototypes declaration **************************/

/* insert the binary representation of num into str between startIndex and endIndex position */
static void charBinaryRep(char *str,
                          unsigned int startIndex,
                          unsigned int endIndex,
                          int num);
/* initialize the word to be zero */
static void initWord(wordPtr);

/************************** global functions **************************/

/* 
*   allocaing new word in the memory
*/
wordPtr allocateNewWord()
{
    wordPtr newWord = (wordPtr)malloc(sizeof(singleMemoryWord));
    if (!newWord)
    {
        printExceptionMsg(ALLOCATION_FALIURE)
    }
    initWord(newWord);
    return newWord;
}

/* erasing a word */
void deleteWord(wordPtr word)
{
    if (word)
        free(word);
}

/*
*   printing a word in a special 4 base 
*/
void printWord(wordPtr word, FILE *file)
{
    int i;
    for (i = 0; i < WORD_SIZE; i++)
    {
        if (word->BinaryRep[i] == '0')
            (word->BinaryRep[++i] == '0') ? fprintf(file, "%c", '*') : fprintf(file, "%c", '#');
        else if (word->BinaryRep[i] == '1')
            (word->BinaryRep[++i] == '0') ? fprintf(file, "%c", '%') : fprintf(file, "%c", '!');
    }
}

/* defining the type of a word */
void defineWordType(wordPtr word, KIND_OF_WORD wordType)
{
    if (word)
        word->wordType = wordType;
}

/* inserting the ARE field */
void insertEncodingType(wordPtr word, ENCODING_TYPE ARE)
{
    charBinaryRep(word->BinaryRep, ARE_POSITION, ARE);
}

/* inserting the addressing method of the source operand */
void insertSrcAddressMethod(wordPtr word, ADDRESSING_METHOD method)
{
    charBinaryRep(word->BinaryRep, SRC_ADDRESS_METHOD_POSITION, method);
}

/* inserting the addressing method of the target operand */
void insertTargetAddressMethod(wordPtr word, ADDRESSING_METHOD method)
{
    charBinaryRep(word->BinaryRep, TARGET_ADDRESS_METHOD_POSITION, method);
}

/* inserting the opcode address */
void insertOpcode(wordPtr word, OPCODE opcode)
{
    charBinaryRep(word->BinaryRep, OPCODE_POSITION, opcode);
}

/* inserting the source register number */
void insertSrcRegister(wordPtr word, REGISTERS reg)
{
    charBinaryRep(word->BinaryRep, SRC_REGISTER_POSITION, reg);
}

/* inserting the target register number */
void insertTargetRegister(wordPtr word, REGISTERS reg)
{
    charBinaryRep(word->BinaryRep, TARGET_REGISTER_POSITION, reg);
}

/* inserting a direct value of an order or an address of variable */
void insertOrderValue(wordPtr word, int value)
{
    charBinaryRep(word->BinaryRep, VALUE_POSITION, value);
}

/* inserting a value */
void insertDataValue(wordPtr word, int value)
{
    charBinaryRep(word->BinaryRep, INSTRUCTION_WORD_POSITION, value);
}

/* returns the type of a word */
KIND_OF_WORD getWordType(wordPtr word)
{
    return word->wordType;
}

/* insert the binary representation of num into str between startIndex and endIndex */
static void charBinaryRep(char *str, unsigned int startIndex, unsigned int endIndex, int num)
{
    unsigned int length = (endIndex - startIndex);
    unsigned int mask = 1;
    mask <<= (length);
    while (mask)
    {
        str[startIndex - 1] = (num & mask) ? '1' : '0';
        mask >>= 1;
        startIndex++;
    }
}

/* initialize the word to be zero */
static void initWord(wordPtr word)
{
    int i;
    for (i = 0; i < WORD_SIZE; i++)
        word->BinaryRep[i] = '0';
}