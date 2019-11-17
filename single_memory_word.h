/*
 *  this file represents a single word in the computer memory
 *  as well as functions to handle a word
*/

#include "macros.h"
#include "data_types.h"

#ifndef SINGLE_WORD_MEMORY_H_
#define SINGLE_WORD_MEMORY_H_

typedef struct s_singleMemoryWord *wordPtr;

/*   allocaing new word in the memory    */
wordPtr allocateNewWord();

/* returns the type of a word */
KIND_OF_WORD getWordType(wordPtr);

/*   erasing a word    */
void deleteWord(wordPtr word);

/*   printing a word in a special 4 base    */
void printWord(wordPtr word, FILE *);

/* defining the type of a word */
void defineWordType(wordPtr word, KIND_OF_WORD wordType);

/* inserting the ARE field */
void insertEncodingType(wordPtr word, ENCODING_TYPE ARE);

/* inserting the addressing method of the source operand */
void insertSrcAddressMethod(wordPtr word, ADDRESSING_METHOD method);

/* inserting the addressing method of the target operand */
void insertTargetAddressMethod(wordPtr word, ADDRESSING_METHOD method);

/* inserting the opcode address */
void insertOpcode(wordPtr word, OPCODE opcode);

/* inserting the source register number */
void insertSrcRegister(wordPtr word, REGISTERS reg);

/* inserting the target register number */
void insertTargetRegister(wordPtr word, REGISTERS reg);

/* inserting a direct value of an order or an address of variable */
void insertOrderValue(wordPtr word, int value);

/* inserting a value */
void insertDataValue(wordPtr word, int value);

#endif
