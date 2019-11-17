/*
*   this file represents the memory image of the assembly program
*   as well as providing different function to handle it
*   such as:
*   initialize a new memory structure, 
*   add a new order word (first word) or a word specifying its operands
*   add variables
*   print the memory image and so on
*/

#include "macros.h"
#include "single_memory_word.h"

#ifndef MEMORY_IMAGE_H
#define MEMORY_IMAGE_H

typedef struct s_memoryCell *cellPtr; /* ptr to a memory cell */

/*   initialize a new structure of memory image    */
void initMemoryImage();

/*   allocating new order word in the computer memory for an order as well as defining the first word of order    */
boolean insertOrderWord(unsigned int address, ADDRESSING_METHOD src, ADDRESSING_METHOD dest, OPCODE op);

/*   allocating new instruction word in the computer memory for an instruction      */
boolean insertDataWord(int value, unsigned int address);

/*   add a word of direct value or an address      */
boolean insertOperandWord(ENCODING_TYPE, int value, int address);

/*   add a word of registers addresses     */
boolean insertOperandRegisterWord(REGISTERS src, REGISTERS dest, int address);

/*   add to all the data words the ic counter      */
void separateDataSegment(unsigned int ic);

/*   free all the memory allocated by a program     */
void destroyMemory();

/*   printing the computer memory in a special four basic way    */
void printMemory(unsigned int ic, unsigned int dc, FILE *);

#endif