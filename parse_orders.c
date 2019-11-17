/*
*   this file is intended to parse an assembly program for the second time
*   the file has three large functions 
*   1.  parsing a command 
*   2.  parsing source argument
*   3.  parsing destination argument
*   I used intentionlly large functions 
*   because if i had divided them into many small ones it would make the code very unreadable and hard to understand
*   although it's usually better to have many small functions instead of large one - I guess in case of parsing strings it has to be that big
*/

#include "macros.h"
#include "memory_image.h"
#include "labels_table.h"
#include "language_macros.h"
#include "extern_table.h"
#include "entry_table.h"
#include "line_manipulations.h"
#include "parsing.h"

/*********************** global variables ***********************/

struct s_order
{
    unsigned int address;       /* the address of an order word */
    unsigned int numOfOperands; /* holds the number of operands needed for a specific order */
    unsigned int line;          /* the line number of the order */
    OPCODE opcode;              /* the machine code of that order */
    parameter parameterSrc;     /* holds the data of the source parameter */
    parameter parameterDest;    /* holds the data of the target parameter */
};

/* defines the different opcodes and their attributes */
struct s_opcode
{
    char *opcodeName;           /* the name of an operation */
    unsigned int numOfOperands; /* number of parameters of this operation */
    unsigned int opcodeAddress; /* the machine opcode of the operation */
    VALID srcImmediate;         /* is immediate value legal as source parameter for that order */
    VALID srcDirect;            /* is direct value legal as source parameter for that order */
    VALID srcFixedInd;          /* is fixed indexing legal as source parameter for that order */
    VALID srcRegister;          /* is a register legal as source parameter for that order */
    VALID destImmediate;        /* is immediate value legal as target parameter for that order */
    VALID destDirect;           /* is direct value legal as target parameter for that order */
    VALID destFixedInd;         /* is fixed indexing legal as target parameter for that order */
    VALID destRegister;         /* is a register legal as target parameter for that order */
} opcodes[] = {
    {"mov", 2, 0, LEGAL, LEGAL, LEGAL, LEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"cmp", 2, 1, LEGAL, LEGAL, LEGAL, LEGAL, LEGAL, LEGAL, LEGAL, LEGAL},
    {"add", 2, 2, LEGAL, LEGAL, LEGAL, LEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"sub", 2, 3, LEGAL, LEGAL, LEGAL, LEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"not", 1, 4, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"clr", 1, 5, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"lea", 2, 6, ILLEGAL, LEGAL, LEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"inc", 1, 7, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"dec", 1, 8, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"jmp", 1, 9, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, ILLEGAL, LEGAL},
    {"bne", 1, 10, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, ILLEGAL, LEGAL},
    {"red", 1, 11, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL},
    {"prn", 1, 12, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, LEGAL, LEGAL, LEGAL},
    {"jsr", 1, 13, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, LEGAL, ILLEGAL, LEGAL},
    {"rts", 0, 14, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL},
    {"stop", 0, 15, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL}};

extern char line[LINE_LENGTH];   /* holds the current line which now is being assembled */
extern unsigned int index;       /* the index which the line has already been assembled up untill that index */
extern unsigned int currentLine; /* counting the lines in the program */

extern usedLabel usedVariables[COMPUTER_CAPACITY];               /* saves the variables alongs with other data for second scan assembling */
unsigned int indexForLabels = 0;                                 /* saves how many variables have been saved for second scan so far */
extern const char *reservedOpcodes[NUM_OF_OPCODES];              /* names of saved language opcodes */
extern const char *reservedDatatypes[NUM_OF_ADDRESSING_METHODS]; /* aved words of the assembly language */
extern unsigned int ic;                                          /* instruction counter */

/************************** static prototypes declaration **************************/

/* parsing a source operand and inserts its data into order struct  */
static boolean getSrcOperand(struct s_order *order);
/* parsing a destination operand and inserts its data into order struct  */
static boolean getDestOperand(struct s_order *order);
/* inserts the operands into the memory */
static boolean insertOperands(struct s_order *order);

/**************************  functions  **************************/

/* checks if an order line is valid */
boolean parseOrderLine(char *op)
{
    boolean succeeded = FALSE;
    int i;
    struct s_order order;

    for (i = 0; i < NUM_OF_OPCODES; i++)
    {
        if (strcmp(op, reservedOpcodes[i]) == 0)
        {
            order.address = ic++;                           /* store the address so that the first word will be before the operands words */
            order.line = currentLine;                       /* the line that the order appears at */
            order.opcode = (OPCODE)i;                       /* the code of the opcode */
            order.numOfOperands = opcodes[i].numOfOperands; /* the number of operands needed for that opcode */
            succeeded = TRUE;                               /* found a match */
            break;
        }
    }
    if (!succeeded) /* no operation matched */
    {
        printErrorMsg("Undefined order name");
        return FALSE;
    }

    if (order.numOfOperands == 2)
    {
        succeeded &= getSrcOperand(&order);
        succeeded &= getDestOperand(&order);
    }
    else if (order.numOfOperands == 1)
    {
        order.parameterSrc.addressing = (ADDRESSING_METHOD)ZERO; /* default value */
        succeeded &= getDestOperand(&order);
    }
    else                                                         /* no operands */
        order.parameterSrc.addressing = (ADDRESSING_METHOD)ZERO; /* default value */

    if (succeeded) /* all operands have been succesfully parsed */
    {
        succeeded &= insertOperands(&order);

        succeeded &= insertOrderWord(order.address,
                                     order.parameterSrc.addressing,
                                     order.parameterDest.addressing,
                                     order.opcode);
    }

    return succeeded;
}

/* parsing a destination operand and inserts its data into order struct  */
static boolean getSrcOperand(struct s_order *order)
{
    boolean succeeded = TRUE;
    char srcOperand[LINE_LENGTH];
    char c;

    if (endOfLine())
    {
        printErrorMsg(MISSING_PARAMETER_ERROR);
        return FALSE;
    }

    c = copyNextWord(srcOperand);
    if (c == '#') /* immediate value type */
    {
        if (!((opcodes[order->opcode]).srcDirect)) /* cannot have an immediate value as a source operand */
        {
            printErrorMsg("Illegal kind of source parameter for this operation");
            return FALSE;
        }
        skipC_Character('#');
        order->parameterSrc.addressing = IMMEDIATE_ADDRESS;
        succeeded &= getNum(&(order->parameterSrc.data.directValue));
    }
    else if (c == '[') /* fixed indexing type */
    {
        if (!((opcodes[order->opcode]).srcFixedInd)) /* cannot have fixed indexing as a source operand */
        {
            printErrorMsg("Illegal kind of source parameter for this operation");
            return FALSE;
        }
        skipC_Character('[');

        if (nextSymbol() != ']')
        {
            printErrorMsg("A closing parenthesis should appear after the index");
            return FALSE;
        }

        order->parameterSrc.addressing = FIXED_INDEXING_ADDRESS;     /* define the addressing method */
        strcpy(order->parameterSrc.data.fixedInd.label, srcOperand); /* copy the name of the variable into the second-scan array of vars */

        succeeded &= getNum(&(order->parameterSrc.data.fixedInd.fixedIndex)); /* save the value of the index */

        skipC_Character(']');
    }
    else if (strlen(srcOperand) == 2 &&
             srcOperand[0] == 'r' && srcOperand[1] >= '0' && srcOperand[1] <= '7') /* register type */
    {
        if (!((opcodes[order->opcode]).srcRegister)) /* cannot have a register as a source operand */
        {
            printErrorMsg("Illegal kind of source parameter for this operation");
            return FALSE;
        }
        order->parameterSrc.addressing = REGISTER_ADDRESS;
        order->parameterSrc.data.registerVal = ((REGISTERS)(srcOperand[1] - '0'));
        order->parameterSrc.encode = ABSOLUTE;
    }
    else /* label name */
    {
        if (!((opcodes[order->opcode]).srcDirect)) /* cannot have a direct value as a source operand */
        {
            printErrorMsg("Illegal kind of source parameter for this operation");
            return FALSE;
        }
        order->parameterSrc.addressing = DIRECT_ADDRESS;
        strcpy(order->parameterSrc.data.varName, srcOperand);
    }

    if (!hasComma()) /* after a source operand there always is a target operand and they have to be separated with a comma */
    {
        printErrorMsg("Missing comma between operands");
        succeeded = FALSE;
    }
    if (succeeded && hasComma())
    {
        printErrorMsg("Multiple commas between operands");
        succeeded = FALSE;
    }

    return succeeded;
}

/* parsing a destination operand and inserts its data into order struct  */
static boolean getDestOperand(struct s_order *order)
{
    boolean succeeded = TRUE;
    char destOperand[LINE_LENGTH];
    char c;

    if (endOfLine())
    {
        printErrorMsg(MISSING_PARAMETER_ERROR);
        return FALSE;
    }

    c = copyNextWord(destOperand);

    if (c == '#') /* immediate value type */
    {
        if (!((opcodes[order->opcode]).destDirect)) /* cannot have an immediate value as a source operand */
        {
            printErrorMsg("Illegal kind of destination parameter for this operation");
            return FALSE;
        }
        skipC_Character('#');
        order->parameterDest.addressing = IMMEDIATE_ADDRESS;
        succeeded &= getNum(&(order->parameterDest.data.directValue));
    }
    else if (c == '[') /* fixed indexing type */
    {
        if (!((opcodes[order->opcode]).destFixedInd)) /* cannot have fixed indexing as a source operand */
        {
            printErrorMsg("Illegal kind of destination parameter for this operation");
            return FALSE;
        }
        skipC_Character('[');

        order->parameterDest.addressing = FIXED_INDEXING_ADDRESS;
        strcpy(order->parameterDest.data.fixedInd.label, destOperand);
        succeeded &= getNum(&(order->parameterDest.data.fixedInd.fixedIndex));

        if (nextSymbol() != ']')
        {
            printErrorMsg("A closing parenthesis should appear after the index");
            return FALSE;
        }
        skipC_Character(']');
    }
    else if (strlen(destOperand) == 2 &&
             destOperand[0] == 'r' && destOperand[1] >= '0' && destOperand[1] <= '7') /* register type */
    {
        if (!((opcodes[order->opcode]).destRegister)) /* cannot have a register as a source operand */
        {
            printErrorMsg("Illegal kind of destination parameter for this operation");
            return FALSE;
        }
        order->parameterDest.addressing = REGISTER_ADDRESS;
        order->parameterDest.data.registerVal = ((REGISTERS)((int)(destOperand[1]) - '0'));
        order->parameterDest.encode = ABSOLUTE;
    }
    else /* label name */
    {
        if (!((opcodes[order->opcode]).destDirect)) /* cannot have a direct value as a source operand */
        {
            printErrorMsg("Illegal kind of destination parameter for this operation");
            return FALSE;
        }
        order->parameterDest.addressing = DIRECT_ADDRESS;
        strcpy(order->parameterDest.data.varName, destOperand);
    }

    if (!endOfLine())
    {
        printErrorMsg("Extraneous text after end of command");
        succeeded = FALSE;
    }

    return succeeded;
}

/* inserts the operands into the memory */
static boolean insertOperands(struct s_order *order)
{
    boolean succeeded = TRUE;

    if (order->numOfOperands == 2)
    {
        if (order->parameterSrc.addressing == REGISTER_ADDRESS &&
            order->parameterDest.addressing == REGISTER_ADDRESS) /* if both operands are registers */
            insertOperandRegisterWord(order->parameterSrc.data.registerVal,
                                      order->parameterDest.data.registerVal,
                                      ic++);
        else
        {
            if (order->parameterSrc.addressing == REGISTER_ADDRESS) /* just the source operand is a register */
                succeeded &= insertOperandRegisterWord(order->parameterSrc.data.registerVal, (REGISTERS)ZERO, ic++);
            else if (order->parameterSrc.addressing == IMMEDIATE_ADDRESS) /* target operand is immediate */
                succeeded &= insertOperandWord(ABSOLUTE, order->parameterSrc.data.directValue, ic++);
            else if (order->parameterSrc.addressing == FIXED_INDEXING_ADDRESS) /* operand is fixed indexing type */
            {
                strcpy(usedVariables[indexForLabels].labelName,
                       order->parameterSrc.data.fixedInd.label);
                usedVariables[indexForLabels].addressOfUse = ic++;
                usedVariables[indexForLabels].lineOfUse = currentLine;
                usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */

                succeeded &= insertOperandWord(ABSOLUTE, order->parameterSrc.data.fixedInd.fixedIndex, ic++);
            }
            else /* operand is a variable */
            {
                strcpy(usedVariables[indexForLabels].labelName,
                       order->parameterSrc.data.varName);
                usedVariables[indexForLabels].addressOfUse = ic++;
                usedVariables[indexForLabels].lineOfUse = currentLine;
                usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */
            }

            if (order->parameterDest.addressing == REGISTER_ADDRESS) /* just the target operand is a register */
                succeeded &= insertOperandRegisterWord((REGISTERS)ZERO, order->parameterDest.data.registerVal, ic++);
            else if (order->parameterDest.addressing == IMMEDIATE_ADDRESS) /* target operand is immediate */
                succeeded &= insertOperandWord(ABSOLUTE, order->parameterDest.data.directValue, ic++);
            else if (order->parameterDest.addressing == FIXED_INDEXING_ADDRESS) /* operand is fixed indexing type */
            {
                strcpy(usedVariables[indexForLabels].labelName,
                       order->parameterDest.data.fixedInd.label);
                usedVariables[indexForLabels].addressOfUse = ic++;
                usedVariables[indexForLabels].lineOfUse = currentLine;
                usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */

                succeeded &= insertOperandWord(ABSOLUTE, order->parameterDest.data.fixedInd.fixedIndex, ic++);
            }
            else /* operand is a variable */
            {
                strcpy(usedVariables[indexForLabels].labelName,
                       order->parameterDest.data.varName);
                usedVariables[indexForLabels].addressOfUse = ic++;
                usedVariables[indexForLabels].lineOfUse = currentLine;
                usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */
            }
        }
    }
    else if (order->numOfOperands == 1)
    {
        order->parameterSrc.addressing = (ADDRESSING_METHOD)ZERO; /* default value */
        order->parameterSrc.data.directValue = ZERO;              /* default value */
        order->parameterSrc.encode = ABSOLUTE;                    /* default value */

        if (order->parameterDest.addressing == REGISTER_ADDRESS) /* operand is a register type */
            succeeded &= insertOperandRegisterWord((REGISTERS)ZERO, order->parameterDest.data.registerVal, ic++);
        else if (order->parameterDest.addressing == IMMEDIATE_ADDRESS) /* operand is immediate value type */
            succeeded &= insertOperandWord(ABSOLUTE, order->parameterDest.data.directValue, ic++);
        else if (order->parameterDest.addressing == FIXED_INDEXING_ADDRESS) /* operand is fixed indexing type */
        {
            strcpy(usedVariables[indexForLabels].labelName,
                   order->parameterDest.data.fixedInd.label);
            usedVariables[indexForLabels].addressOfUse = ic++;
            usedVariables[indexForLabels].lineOfUse = currentLine;
            usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */

            succeeded &= insertOperandWord(ABSOLUTE, order->parameterDest.data.fixedInd.fixedIndex, ic++);
        }
        else /* operand is a variable */
        {
            strcpy(usedVariables[indexForLabels].labelName,
                   order->parameterDest.data.varName);
            usedVariables[indexForLabels].addressOfUse = ic++;
            usedVariables[indexForLabels].lineOfUse = currentLine;
            usedVariables[++indexForLabels].addressOfUse = 0; /* a way to determine the last element */
        }
    }
    else /* no operands */
    {
        order->parameterSrc.addressing = (ADDRESSING_METHOD)ZERO;  /* default value */
        order->parameterDest.addressing = (ADDRESSING_METHOD)ZERO; /* default value */
    }

    return succeeded;
}