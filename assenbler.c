/*
 ====================================================================================
 Name        : assembler.c
 Author      : Pnina Goldshmit & Yishay Polatov
 Description : Assembler - Translates a specific assembly code into defined machine code
 ====================================================================================
 */

/* main file - intended to get the arguments from user and pass them along to the corresponding functions in order to translate them */

#include "macros.h"

/* 
*	the assembly files are being passed to the program via command line arguments 
*/
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Files must be provided by the command line arguments\n");
		return 1;
	}

	while (argc-- > 1)
	{
		printf("At file: \'%s.as\':\n", *++argv);
		if (openFlie(*argv))
		{
			if (assemble(*argv))
				printf("\tThe file has been successfully assembled!\n\n");
		}
	}

	return EXIT_SUCCESS;
}