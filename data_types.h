/*
 * this files declares different sata types used to handle the commands and their parameters 
 */
#include "macros.h"

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#define NUM_OF_OPCODES 16
#define NUM_OF_ADDRESSING_METHODS 4

typedef enum
{
	FIRST_WORD_OF_ORDER,
	OPERAND_DIRECT_VALUE,
	OPERAND_REGISTERS,
	INSTRUCTION_WORD
} KIND_OF_WORD;

typedef enum
{
	ABSOLUTE,
	EXTERNAL,
	RELOCATABLE
} ENCODING_TYPE;

typedef enum
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7
} REGISTERS;

typedef enum
{
	MOV,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
} OPCODE;

typedef enum
{
	IMMEDIATE_ADDRESS,
	DIRECT_ADDRESS,
	FIXED_INDEXING_ADDRESS,
	REGISTER_ADDRESS
} ADDRESSING_METHOD;

/* union for different types of parameters */
typedef union u_parameterData {
	int directValue;
	char varName[LINE_LENGTH];
	struct s_fixed
	{
		char label[LINE_LENGTH];
		int fixedIndex;
	} fixedInd;
	REGISTERS registerVal;
} parameterData;

typedef struct s_parametr
{
	parameterData data;
	ADDRESSING_METHOD addressing;
	ENCODING_TYPE encode;
} parameter;

typedef enum
{
	ILLEGAL,
	LEGAL
} VALID;

/* saves a label for second scan manipulations */
typedef struct s_usedLabel
{
	char labelName[LABLES_NAME_LENGTH];
	unsigned int labelAddress;
	unsigned int lineOfUse;	/* in case of an error - the line number is needed to indicate where the error occured */
	unsigned int addressOfUse; /* in case of use in an external label - the address of use is needed for the .ext file */
} usedLabel;

#endif