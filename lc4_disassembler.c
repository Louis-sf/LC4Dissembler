#include <stdio.h>
#include "lc4_memory.h"
#include <stdlib.h>
#include <string.h>

int reverse_assemble (row_of_memory* memory)
{
    row_of_memory* current;
	int RD;    int RT;	int RS;	int subopt;
	short unsigned int imm;
	char* assembly;
	short unsigned int tmp;


	while ((current = search_opcode(memory, 0x0001)) != NULL) {

		assembly = malloc(17 * sizeof(char));
		current->assembly = malloc(17 * sizeof(char));

		if (assembly == NULL) return -1;
		
		tmp = current->contents << 4;
		RD = tmp >> 13;
		tmp = current->contents << 7;
		RS = tmp >> 13;
		tmp = current->contents << 13;
		RT = tmp >> 13;
		tmp = current->contents << 10;
		subopt = tmp >> 13;
		tmp = current->contents << 10;
		imm = tmp >> 15;
        
		if (imm == 1) 
        {
			tmp = current->contents << 11;
			imm = tmp >> 11;

			if (imm > 15) 
            {
				imm = 16 - (imm & 0b0000000000001111);
				sprintf(assembly, "ADD R%d, R%d, #-%d", RD, RS, imm);
				strcpy(current->assembly, assembly);
				free(assembly);
				continue;
			}

			// update cur->assembly
			sprintf(assembly, "ADD R%d, R%d, #%d", RD, RS, imm);
			strcpy(current->assembly, assembly);

			free(assembly);//free heap taken up by assembly
			continue;
		}
        //use switch case to determine print message 
		switch(subopt) 
        {
			case 0:
				sprintf(assembly, "ADD R%d, R%d, R%d", RD, RS, RT);
				strcpy(current->assembly, assembly);
				break;

			case 1:
				sprintf(assembly, "MUL R%d, R%d, R%d", RD, RS, RT);
 				strcpy(current->assembly, assembly);
				break;

			case 2:
				sprintf(assembly, "SUB R%d, R%d, R%d", RD, RS, RT);
				strcpy(current->assembly, assembly);
				break;

			case 3:
				sprintf(assembly, "DIV R%d, R%d, R%d", RD, RS, RT);
				strcpy(current->assembly, assembly);
				break;

		}
		free(assembly);//free heap space from strcpy
	}

	return 0;
}
