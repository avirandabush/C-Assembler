/*assembler project*/
/*maman 14*/
/*data structures function*/

#include "info.h"
#include "struct.h"

/*symbol*/

/*add to the symbol list*/

void push_symbol(symbol_ptr * symbol_head, char *name, int address, int direct, int external)
{
	symbol_ptr p1, p2, temp;
	
	temp = (symbol_ptr) malloc(sizeof(symbol_table));

	if(!temp)
	{
		printf("cannot allocate memory\n");
		return;
	}
	strcpy(temp->name,name);
	temp->address = address;
	temp->directive = direct;
	temp->external = external;

	if(!(*symbol_head))
	{
		temp->next = NULL;
		*symbol_head = temp;
		return;
	}
	
	p1 = *symbol_head;
	while(p1)
	{
		p2 = p1;
		p1 = p1->next;
	}
	temp->next = p1;
	p2->next = temp;
}

/*print symbol list*/

void print_symbol(symbol_ptr p)
{
	while(p)
	{
		printf("%s \n", p->name);
		printf("%d \n", p->address);
		printf("%d \n", p->directive);
		printf("%d \n", p->external);
		p = p->next;
	}
}

/*add ic value for variables*/

void var_address(symbol_ptr * h)
{
	/*assignments*/

	symbol_ptr p1, p2;
	p1 = *h;

	while(p1)
	{
		if(p1->directive == 1)
			p1->address += ic;
		p2 = p1;
		p1 = p2->next;
	}
}

/*check label in symbol table*/

int check_symbol(symbol_ptr *ptr, char *label)
{
	/*assignments*/

	symbol_ptr p1, p2;
	int val;
	int i;
	int flag = 0;


	p1 = *ptr;

	while(p1)
	{
	
		for(i=0;p1->name[i] != '\0';i++)
		{
			if(p1->name[i] != label[i])
				flag++;
		}

		if(flag != 0)
		{
			if(p1->external == 1)
				return -2;	/*its external variable*/

			val = p1->address;
			return val;		/*all good. return the address*/
		}

		p2 = p1;
		p1 = p2->next;
	}
	return -1;
}

/*free symbol list*/

void free_symbol(symbol_ptr * h)
{
	symbol_ptr p;
	while(*h)
	{
		p = *h;
		*h = p->next;
		free(p);
	}
}

/*data*/

/*add to the tada list*/

void push_data(data_ptr * data_head, int address, short data)
{
	data_ptr p1, p2, temp;
	
	temp = (data_ptr) malloc(sizeof(data_table));

	if(!temp)
	{
		printf("cannot allocate memory\n");
		return;
	}
	
	temp->address = address;
	temp->data = data;

	if(!(*data_head))
	{
		temp->next = NULL;
		*data_head = temp;
		return;
	}
	
	p1 = *data_head;
	while(p1)
	{
		p2 = p1;
		p1 = p1->next;
	}
	temp->next = p1;
	p2->next = temp;
}

/*print data list*/

void print_data(data_ptr p)
{
	while(p)
	{
		printf("%d \n", p->data);
		p = p->next;
	}
}

/*add ic value for data*/

void data_address(data_ptr * h)
{
	/*assignments*/

	data_ptr p1, p2;
	p1 = *h;

	while(p1)
	{
		p1->address += ic;
		p2 = p1;
		p1 = p2->next;
	}
}

/*free data list*/

void free_data(data_ptr * h)
{
	data_ptr p;
	while(*h)
	{
		p = *h;
		*h = p->next;
		free(p);
	}
}

/*insruction*/

/*add to the instruction list*/

void push_instruction(instruction_ptr * instruction_head, int address, short data)
{
	instruction_ptr p1, p2, temp;
	
	temp = (instruction_ptr) malloc(sizeof(instruction_table));

	if(!temp)
	{
		printf("cannot allocate memory\n");
		return;
	}
	
	temp->address = address;
	temp->data = data;

	if(!(*instruction_head))
	{
		temp->next = NULL;
		*instruction_head = temp;
		return;
	}
	
	p1 = *instruction_head;
	while(p1)
	{
		p2 = p1;
		p1 = p1->next;
	}
	temp->next = p1;
	p2->next = temp;
}

/*print instruction list*/

void print_instruction(instruction_ptr p)
{
	while(p)
	{
		printf("%05o \n", p->data);
		p = p->next;
	}
}

/*free instruction list*/

void free_instruction(instruction_ptr * h)
{
	instruction_ptr p;
	while(*h)
	{
		p = *h;
		*h = p->next;
		free(p);
	}
}

