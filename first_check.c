/*assembler project*/
/*maman 14*/
/*the first check part*/

#include "info.h"
#include "struct.h"

/*variable declaration*/

int ic = 100;
int dc = 0;
symbol_ptr symbol_head = NULL;
data_ptr data_head = NULL;

/*parsing the file*/

int assembler(char name[])
{
	/*assignments*/

	FILE *fp;			/*to open the files*/

	int i, j;
	int lineNumber = 0;		/*for error message*/
	int errorFlag = 0;

	char currentLine[LINE_SIZE];	/*for line / word*/
	char newLine[LINE_SIZE];
	char tempLine[LINE_SIZE];
	char newWord[WORD_SIZE];
	char tempWord[WORD_SIZE];

	char fileName[WORD_SIZE];	/*for file name and extension*/
	char fileExtension[] = ".as";	

	/*handling the file extension part*/

	fileName[0] = '\0';			/*initialize it every call*/
	strcat(fileName, name);			/*add the name*/
	strcat(fileName, fileExtension);	/*add the extension*/

	/*handling opening error*/

	if(!(fp = fopen(fileName, "r")))	/*fail to opening file*/
	{
		fprintf(stderr, "file %s can't be opened\n", fileName);
		return 1;
	}
	
	/*handling the file*/

	while(fgets(currentLine, LINE_SIZE, fp))
	{
		for(i=0;currentLine[i] != '\0';i++)
		{
			lineNumber++;

			/*remove spaces from the head of line*/

			for(i=0;currentLine[i] != '\0';i++)
			{
				if(!isspace(currentLine[i]))
					break;
			}

			for(j=0;currentLine[i] != '\0';i++,j++)
			{
				newLine[j] = currentLine[i];
			}
			newLine[j] = '\0';		/*save line without head spaces*/

			/*if this is a comment, skip it*/

			if(newLine[0] == ';')
				break;

			/*if it is a directive line*/

			if(newLine[0] == '.')
			{
				for(i=0;newLine[i] != ' ' && newLine[i] != '\t' && newLine[i] != '\0';i++)
					tempWord[i] = newLine[i];
				tempWord[i] = '\0';	/*save directive word*/

				if(isdirective(tempWord))
				{
					directive1(newLine);
					break;
				}

				printf("line: %d - illegal command\n", lineNumber);
				errorFlag++;
				break;
			}

			/*if it is an instruction*/

			if(islower(newLine[0]))
			{
				for(i=0;newLine[i] != ' ' && newLine[i] != '\t' && newLine[i] != '\0';i++)
					tempWord[i] = newLine[i];
				tempWord[i] = '\0';	/*save instruction word*/

				if(isinstruction(tempWord))
				{
					instruction1(newLine);
					break;
				}
				printf("line: %d - illegal command\n", lineNumber);
				errorFlag++;
				break;
			}

			/*if it is a label*/

			if(isupper(newLine[0]))
			{	
				for(i=0;newLine[i] != ':' && newLine[i] != '\0';i++)
					tempWord[i] = newLine[i];
				tempWord[i] = '\0';	/*save label name*/
				
				/*skip spaces*/

				for(i++;newLine[i] != '\0';i++)
				{
					if(!isspace(newLine[i]))
						break;
				}

				for(j=0;newLine[i] != '\0';i++,j++)	
					tempLine[j] = newLine[i];
				tempLine[j] = '\0';	/*save the line without the label*/	

				for(i=0;tempLine[i] != ' ' && tempLine[i] != '\t' && tempLine[i] != '\0';i++)
					newWord[i] = tempLine[i];
				newWord[i] = '\0';	/*save first word for check word*/

				if(isdirective(newWord))
				{
					push_symbol(&symbol_head, tempWord, dc, 1, 0);	/*save label name*/
					directive1(tempLine);					
					break;
				}
				if(isinstruction(newWord))
				{
					push_symbol(&symbol_head, tempWord, ic, 0, 0);	/*save label name*/
					instruction1(tempLine);
					break;
				}
				printf("line: %d - illegal command\n", lineNumber);
				errorFlag++;
				break;
			}
		}
	}
	
	/*if there is an error, exit*/

	if(errorFlag!=0)
	{
		/*free memory ellocated*/

		free_data(&data_head);		
		free_symbol(&symbol_head);	

		printf("there are errors in file %s\n\n", fileName);
		return 1;
	}
	
	/*if there are no errors*/

	var_address(&symbol_head);	/*add ic to variable address*/
	rewind(fp);			/*reset file pointer*/
	second_check(fp, name);		/*start the second check*/

	/*free memory ellocated*/

	fclose(fp);		
	free_data(&data_head);		
	free_symbol(&symbol_head);
	free_instruction(&instruction_head);

	return 0;	/*exit successfully*/
}

/*the instruction parse*/

void instruction1(char *currentLine)
{
	/*assignments*/

	int i, j;
	char tempWord[WORD_SIZE];

	char group3[INS_GROUP3][WORD_SIZE] = 
					{"rts", "stop"};
	char group2[INS_GROUP2][WORD_SIZE] = 
					{"clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
	char group1[INS_GROUP1][WORD_SIZE] = 
					{"mov", "cmp", "add", "sub", "lea"};


	ic++;			/*increase ic for the instruction*/

	for(i=0;currentLine[i] != ' ' && currentLine[i] != '\t' && currentLine[i] != '\0';i++)
		tempWord[i] = currentLine[i];
	tempWord[i] = '\0';	/*save instruction word*/

	for(i=0;i<INS_GROUP3;i++)
	{
		if(!strcmp(tempWord, group3[i]))
			return;	/*no extra space is needed*/
	}
	for(i=0;i<INS_GROUP2;i++)
	{
		if(!strcmp(tempWord, group2[i]))
		{
			ic++;	/*save space for one operand*/
			return;
		}
	}

	for(i=0;i<INS_GROUP1;i++)
	{
		if(!strcmp(tempWord, group1[i]))
		{
			ic++;	/*save space for one operand*/
		
			/*skip the op word and the spaces following*/

			for(i=0;currentLine[i] != ' ' && currentLine[i] != '\t' && currentLine[i] != '\0';i++);
			for(;(currentLine[i] == ' ' || currentLine[i] == '\t') && (currentLine[i] != '\0');i++);

			if(currentLine[i] == '*' || currentLine[i] == 'r')
			{
				/*skip the reg name and the spaces following*/

				for(i++;currentLine[i] != ',' && currentLine[i] != '\0';i++);
				for(i++;(currentLine[i] == ' ' || currentLine[i] == '\t') && (currentLine[i] != '\0');i++);

					if(currentLine[i] == '*' || currentLine[i] == 'r')
						return;
			}
			ic++;		/*save space for one operand*/
			return;		
		}
	}
}

/*the directive parse*/

void directive1(char *currentLine)
{
	/*assignments*/

	int i, j;
	int var;

	char *ptr;
	char newLine[LINE_SIZE];
	char tempWord[WORD_SIZE];
	char dataVal[WORD_SIZE];

	char data[] = {".data"};	/*words to check with*/
	char ent[] = {".entry"};
	char ext[] = {".extern"};
	char str[] = {".string"};

	for(i=0;currentLine[i] != ' ' && currentLine[i] != '\t' && currentLine[i] != '\0';i++)
		tempWord[i] = currentLine[i];
	tempWord[i] = '\0';	/*save directive word*/

	/*if it is antry*/

	if(!strcmp(ent, tempWord))
		return;		/*skip entry in first check*/

	/*if it is a string*/
	if(!strcmp(str, tempWord))
	{
		for(i=0;currentLine[i] != '\"';i++);	/*jump to head of the string*/
		for(i++;currentLine[i] != '\"';i++)	/*loop through the string*/
		{
			var = (int) currentLine[i];
			push_data(&data_head, dc, var);	/*add it to the data table*/
			dc++;
		}
		var = 0;				/*end of string*/
		push_data(&data_head, dc, var);		/*add it to the data table*/
		dc++;
		return;
	}

	ptr = strtok(currentLine, ",, ,\t");

	/*if it is data*/

	if(!strcmp(data, tempWord))
	{	
		while(ptr!=NULL)
		{
			var = atoi(ptr);		/*extract the number*/
			push_data(&data_head, dc, var);	/*add it to the data table*/
			dc ++;				/*increase dc*/
			ptr = strtok(NULL, ",, ,\t");	/*promote ptr*/
		}
		return;
	}

	
	/*if it is extern*/

	if(!strcmp(ext, tempWord))
	{
		ptr = strtok(NULL, " ,\t,");		/*skip the word extern*/
		push_symbol(&symbol_head, ptr, 0, 0, 1);
		return;
	}

}

