/*assembler project*/
/*maman 14*/
/*the other functions part*/

#include "info.h"

/*recevie a word, and check if its a reserved-for-the-language word*/ 

int reserved_words(char word[])
{
	/*assignments*/

	int i;
						/*set reserved values*/
	char reserved[RESERVED_WORD_NUM][LINE_SIZE] = {
				"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
				"mov", "cmp", "add", "sub", "lea", "clr", 
				"not", "inc", "dec", "jmp", "bne", "red",
				"prn", "jsr", "rts", "stop", ".data", ".string",
				".entry", ".extern"};
	
	for(i = 0;i < RESERVED_WORD_NUM;i++)
	{					/*loop through the words*/
		if(!(strcmp(word, reserved[i])))
		{
			printf("you try to use reserved word\n");
			return 1;		/*if you find a match*/
		}
	}
	return 0;				/*if the word is ok to use*/
}

/*recevie a paragraph and returning it without the spaces at the beginning*/

void remuve_spaces(char *current_line)
{
	/*assignments*/

	char *ptr = current_line;

	while(isspace(*ptr))			/*check for spaces*/
		ptr++;				/*increase pointer*/
	strcpy(current_line, ptr);		/*make the free spaces one*/
}

/*check if it is a directive line*/

int isdirective(char *n)
{
	/*assignments*/

	char data[] = {".data"};
	char ent[] = {".entry"};
	char ext[] = {".extern"};
	char str[] = {".string"};

	if(!strcmp(n, data)||!strcmp(n, ent)||!strcmp(n, ext)||!strcmp(n, str))
		return 1;
	return 0;
}

/*check if it is a insruction line*/

int isinstruction(char *n)
{
	/*assignments*/

	int i;
	char instruction_words[INSTRUCTION_WORD_NUM][LINE_SIZE] = 
					{"mov", "cmp", "add", "sub", "lea", "clr", 
					"not", "inc", "dec", "jmp", "bne", "red",
					"prn", "jsr", "rts", "stop"};

	for(i=0;i<INSTRUCTION_WORD_NUM;i++)
	{
		if(!strcmp(n, instruction_words[i]))
			return 1;
	}
	return 0;
}

/*check if a given word is a register*/

int is_register(char *n)
{
	/*assignments*/

	int i;
	char register_name[REGISTER_NUM][LINE_SIZE] = 
					{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

	for(i=0;i<REGISTER_NUM;i++)
	{
		printf("reg %s\n", register_name[i]);
		printf("n %s", n);
		if(strcmp(n, register_name[i]))
			return 1;
	}
	return 0;
}

