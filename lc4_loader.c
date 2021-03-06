/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"

/* declarations of functions that must defined in lc4_loader.c */
FILE* open_file(char* file_name)
{
    FILE *file = fopen(file_name, "rb");
	if(file == NULL) {
		fprintf(stderr, "error1: usag: ./lc4 <object_file.obj>\n");
	}
	return file;
}

int parse_file (FILE* my_obj_file, row_of_memory** memory)
{
    
    int count;
	unsigned short int address;
	unsigned short int word;
	row_of_memory* tmp;
    
    int arr[2];
    while(1)
    {
        arr[0] = fgetc(my_obj_file);
        if (arr[0] == -1) 
            {
                break;
            }
        arr[1] = fgetc(my_obj_file);

		word = (arr[0] << 8) + arr[1];// manage eadiness and form a complete commandline for test
        
		// whether it is the head
		if (word == 0xCADE) //Entering CODE section
        {		
            
			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			address = (arr[0] << 8) + arr[1];
			printf("%x\n", address);

			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			count = (arr[0] << 8) + arr[1];

			for (int i = count; i > 0; i--) {
				arr[0] = fgetc(my_obj_file);
				arr[1] = fgetc(my_obj_file);
				word = (arr[0] << 8) + arr[1];

				if(add_to_list(memory, address, word) != 0) 
                {
					fprintf(stderr, "adding node failed.\n");
					return 2;
				}
				address++;
			}
		} else if (word == 0xDADA) {	
			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			address = (arr[0] << 8) + arr[1];

			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			count = (arr[0] << 8) + arr[1];

			for (int i = count; i > 0; i--) {

				arr[0] = fgetc(my_obj_file);
				arr[1] = fgetc(my_obj_file);
				word = (arr[0] << 8) + arr[1];

				if(add_to_list(memory, address, word) != 0) 
                {
					fprintf(stderr, "adding node failed.\n");
					return 2;
				}
				address++;
			}			
		} else if (word == 0xC3B7) {	
			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			address = (arr[0] << 8) + arr[1];

			tmp = search_address(*memory, address);
			if (tmp == NULL) 
            {
				if (add_to_list(memory, address, 0x0000) != 0) 
                {
					fprintf(stderr, "adding node failed.\n");
					return 2;
				}
			}
 			tmp = search_address(*memory, address);
			arr[0] = fgetc(my_obj_file);
			arr[1] = fgetc(my_obj_file);
			count = (arr[0] << 8) + arr[1];

			char* label = malloc((count + 1) * sizeof(char));

			for (int i = 0; i < count + 1; i++) 
            {
				if (i == count) 
				{
					label[i] = '\0';
					break;
				}
				label[i] = fgetc(my_obj_file);
			}

			tmp->label = malloc((count + 1) * sizeof(char));

			if (tmp->label == NULL) 
            {
				fprintf(stderr, "allocating memory failed.\n");
				return 3;
			}

			strcpy(tmp->label, label);
			free(label);//free heap space taken up by trcpy
		}
	}
	if (fclose(my_obj_file)) 
    {
		fprintf(stderr, "can not close file\n");
		int deleteState = delete_list(memory);
        return (deleteState == 0)? 0: deleteState;
	}
	return 0;     
    }


