/*
*   this file is intended to parse an instruction or macro lines
*/

#include "macros.h"
#include "memory_image.h"
#include "labels_table.h"
#include "language_macros.h"
#include "extern_table.h"
#include "entry_table.h"
#include "line_manipulations.h"
#include "data_types.h"

#ifndef PARSE_INSTRUCTIONS_H
#define PARSE_INSTRUCTIONS_H

/**************  instructions parsing functions  **************/

/* checks if a line is a valid macro definition */
boolean parseMacroLine();
/*checks if an instruction of type data line is valid */
boolean parseDataLine();
/* checks if an insruction of type string is valid */
boolean parseStringLine();
/* checks if an extern line is valid */
boolean parseExternLine();
/* checks if an entry line is valid */
boolean parseEntryLine();

/**************  orders parsing functions  **************/

/* checks if an order line is valid */
boolean parseOrderLine(char *op);

#endif