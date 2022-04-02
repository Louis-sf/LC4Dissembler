/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"


int add_to_list (row_of_memory** head,		 short unsigned int address,		 short unsigned int contents)
{
	row_of_memory* newNode = malloc(sizeof(row_of_memory));
    //allocate memory to newNode
    
	if (newNode == NULL) {
		return -1;
	}
    //check if malloc was succesfull
    
	newNode->address = address;
	newNode->label = NULL;
	newNode->contents = contents;	
	newNode->assembly = NULL;
	newNode->next = NULL;

	if(*head == NULL) 
    {
    //generate new node if passed in arguement is null
        *head = newNode;
		return 0;
	}

    //Traverse the memory 
    
    row_of_memory* cur = *head;
	while (cur->next != NULL) 
    {
		if (cur->next->next != NULL && cur->next->address < address) 
        {
			cur = cur->next;
		} else 
        {
			break;
		}
	}
	if (cur->address <= address) 
    {
		if (cur->next == NULL)
        {
			cur->next = newNode;
			return 0;//add new node if we reached to the end of linked_list
		}
		if (cur->next->address > address) {
			row_of_memory* tmp = cur->next;
			cur->next = newNode;
			newNode->next = tmp;
			return 0;
		}
		row_of_memory* tmp = cur->next;
		tmp->next = newNode;
		return 0;	
    }

	// append at the end
	newNode->next = cur;
	*head = newNode;
	return 0;
}


row_of_memory* search_opcode  (row_of_memory* head,
				      short unsigned int opcode  )
{

	row_of_memory* cur = head;

	while (cur != NULL) 
    {
		if ( cur->assembly == NULL&&(cur->contents/0x1000) == opcode ) 
        {
			break;
		}

		cur = cur->next;
	}

	return (cur == NULL)? NULL: cur;
}
int delete_list   (row_of_memory** head )
{
	row_of_memory* cur = *head;

    //traverse and free the heap occupied by LinkedList
	while (cur != NULL) 
    {
		row_of_memory* tmp = cur->next;
		free(cur->assembly);
        free(cur->label);
		free(cur);
		cur = tmp;
	}
	if (cur == NULL) 
	{
		*head = NULL;
		return 0;
	}
	return -1;
}
void print_list (row_of_memory* head )
{
	if (head == NULL) 
	{
		printf("The head is NULL");
		return;
	}

	row_of_memory* cur = head;

	printf("<label>\t\t<address>\t<contents>\t<assembly>\n");
	while (cur != NULL) 
	{
		printf("%s\t\t%04x\t\t%04x\t\t%s\n", cur->label, cur->address, cur->contents, cur->assembly);
		cur = cur->next;
	}

	return;
}

row_of_memory* search_address (row_of_memory* head, short unsigned int address )
{

	row_of_memory* cur = head;
    while (cur != NULL) 
    {
		if (cur->address == address) break;
		cur = cur->next;
	}
	return cur; 
}
