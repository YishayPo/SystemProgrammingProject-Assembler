# Assembler - A project for System Programming course

## Compile and run

The project has only been tested on Ubuntu OS 
Compiling the System can easily be done by openning the folder containing the program files and using "make" command
afterwards in order to assemble files you can use the command assembler and pass the files you wish to assemble as arguments 
the files extention must be .asm and the extention shouldn't be typed when passing the files to the program
if the assembly file contain no errors the system would output a file with the machine code 
also if the assembly file has extern variables or intern variables - those variables would also be output as files 

## Project Structure in summary

### Data Structures
The file ADT_linked_list implements an abstract linked list which can create and maintain linked lists for all kinds of data - struct, union, int etc.
There also is a header file for that abstract linked list which declares the functions to maintain the linked list and manipulate the data stored in the list

Using that file the program creates few linked lists to store data like Macros, Variables, Memory Image etc.
each data structure needed for assembling a file uses its own functionality which is built with the abstract functionality provided with the abstract linked list

The memory image is manipulated with two files - each responsible for its own functionality:
    The file single_memory_word defines each memory word individually and provides various funcions to maintain each word for fully representing the data stored in that memory cell 
    The file memory_image implements the memory image as a whole - using a linked list of data type - single_memory_word along with addresses and a few more data to represent the whole memory of the imaginary computer, this file provides the functionality required to maintain the memory image, initialize it, print it etc,
 
### Parsing the Input
The assmebly files which are passed to the program and being assembled are made from simple text representing the assmebly code and for identifing each line of code, parsing and assembling it the program contains few files which are responsible for parsing input

The file line_manipulations declares a few simple and small functions each does a simple task like converting a string to a number or finding out if a line is a comment line or an empty one copying a single word from a line into a buffer etc.

in addition to these files there are also three more important files:
The file assemble which opens a file and then fetches each line at a time and passes it on to the appropriate functions which in turn check what kind of line it is, what are its arguments and so on
The file parse_order which gets a line that is an order line and parses its type of order, its arguments, etc.
The file parse_instractions which gets a line that's an instraction line and identifying its type of instraction, checks for legality etc.

##### above all these files there is the main file - assembler which opens the assembly files and passes them on to the assemble file's functions to parse and output its machine code

