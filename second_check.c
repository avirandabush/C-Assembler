/*assembler project*/
/*maman 14*/
/*the second check part*/

#include "info.h"
#include "struct.h"

/*variable declaration*/

int ic;
int dc;
int unknown_address;
symbol_ptr symbol_head;
data_ptr data_head;
instruction_ptr instruction_head = NULL;

/*second review over the file*/

void second_check(FILE *fp, char *name)
{
	/*assignments*/

	int i, j;
	int errorFlag = 0;		/*for error message*/
	int lineNumber = 1;

	char currentLine[LINE_SIZE];	/*for line / word*/
	char newLine[LINE_SIZE];
	char tempLine[LINE_SIZE];
	char newWord[WORD_SIZE];
	char tempWord[WORD_SIZE];

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
					directive2(newLine, name);
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
					instruction2(newLine);
					break;
				}
				printf("line: %d - illegal command\n", lineNumber);
				errorFlag++;
				break;
			}

			/*if it is a label*/

			if(isupper(newLine[0]))
			{	
				/*save label name*/

				for(i=0;newLine[i] != ':' && newLine[i] != '\0';i++);	
				
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
					directive2(tempLine, name);					
					break;
				}
				if(isinstruction(newWord))
				{
					instruction2(tempLine);
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

		printf("there are errors in file %s\n\n", name);
		return;
	}

	/*if there are no errors*/

	data_address(&data_head);				/*add ic to variable address*/
	make_ob_file(instruction_head, data_head, name);	/*make the object file*/
	make_ext_file(symbol_head, name);			/*make the external file*/
	
	return;		/*go back successfully*/
}

/*the instruction parse*/

void instruction2(char *current_line)
{
	/*assignments*/

	int i, j;
	int var;

	int twoRegFlag = 0;
	int symbolCheck = 0;
	static int iic = 100;

	short a = A_BIT;		/*the A in ARE. the third bit*/
	short r = R_BIT;		/*the R in ARE. the second bit*/
	short e = E_BIT;		/*the E in ARE. the first bit*/
	short firstWord = 0;		/*the first info word*/
	short secondWord = 0;		/*the second info word*/
	short thirdWord = 0;		/*the third info word*/
	short one = 1;

	int instructionValue[] = {MOV_BIT, CMP_BIT, ADD_BIT, SUB_BIT, LEA_BIT, CLR_BIT, NOT_BIT, INC_BIT, DEC_BIT, JMP_BIT, 
						BNE_BIT, RED_BIT, PRN_BIT, JSR_BIT, RTS_BIT, STOP_BIT};
	char instructionNmae[INSTRUCTION_WORD_NUM][WORD_SIZE] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", 
						"inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
	char tempLine[LINE_SIZE];
	char tempReg[LINE_SIZE];
	char regVal[WORD_SIZE];
	char *ptr;
	strcpy(tempLine, current_line);
	ptr = strtok(current_line, ",, ,\t");
	remuve_spaces(tempLine);

	for(i=0;i<INSTRUCTION_WORD_NUM;i++)						/*loop to find the right op*/
	{
		if(!strcmp(ptr, instructionNmae[i]))			/*if there is a match*/
		{
			firstWord |= instructionValue[i];		/*set on bits 12-15 by the op value*/
			firstWord |= a;					/*set on bit 2, absolute, for the insruction*/

			if(i<INS_GROUP1)						/*op with two operands*/
			{
				for(i=0;tempLine[i] != ' ';i++);	/*skip the op word*/

				for(i++,j=0;tempLine[i] != ',';i++,j++)	/*save the line without the op word*/
				{
					tempReg[j] = tempLine[i];
				}
				tempReg[j] = '\0';			/*make it a string*/
				remuve_spaces(tempReg);

				/*handle the source operand*/

				/*immidiate operand*/

				if(tempReg[0] == '#')
				{
					for(i=0;i<7;i++)
						one = one << 1;
					firstWord |= one;
					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
						{
							regVal[j] = tempReg[i];
							j++;
						}
					}
					regVal[j] = '\0';
					var = atoi(regVal);		/*extract the number*/
					for(i=0;i<3;i++)
						var = var << 1;
					secondWord |= var;		/*add to instruction table*/
					secondWord |= a;		/*set on bit 2, absolute, immidiate method*/
				}

				/*direct operand*/

				if(isupper(tempReg[0]))	
				{
					for(i=0;i<8;i++)
						one = one << 1;
					firstWord |= one;	
					symbolCheck = check_symbol(&symbol_head, tempReg);
					if(symbolCheck == -1)		/*no match to eny label*/
					{
						printf("error! label name dosn't exsist\n");
						return;
					}
					if(symbolCheck == -2)		/*external label*/
					{
						secondWord |= e;	/*set on bit 0, external*/
					}
					if(symbolCheck >= 0)		/*relocatable label*/
					{
						for(i=0;i<3;i++)
							symbolCheck = symbolCheck << 1;	
						secondWord |= symbolCheck;	
						secondWord |= r;	/*set on bit 1, relocatable*/
					}
				}

				/*indirect register*/

				if(tempReg[0] == '*')
				{
					for(i=0;i<9;i++)
						one = one << 1;
					firstWord |= one;	
					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}
					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/
					for(i=0;i<6;i++)
						var = var << 1;		
					secondWord |= var;		/*add to instruction table*/
					secondWord |= a;		/*set on bit 2, absolute, immidiate method*/
					twoRegFlag++;			/*inform that one operand is register*/
				}

				/*direct register*/

				if(islower(tempReg[0]))
				{
					for(i=0;i<10;i++)
						one = one << 1;
					firstWord |= one;		
					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}
					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/
					for(i=0;i<6;i++)
						var = var << 1;	
					secondWord |= var;	
					secondWord |= a;		/*set on bit 2, absolute*/
					twoRegFlag++;			/*inform that one operand is register*/
				}

				/*skip the op word and the spaces following*/

				for(i=0;tempLine[i] != ' ' && tempLine[i] != '\t' && tempLine[i] != '\0';i++);
				for(;(tempLine[i] == ' ' || tempLine[i] == '\t') && (tempLine[i] != '\0');i++);
 
				/*skip the reg name and the spaces following*/

				for(i++;tempLine[i] != ',' && tempLine[i] != '\0';i++);
				for(i++;(tempLine[i] == ' ' || tempLine[i] == '\t') && (tempLine[i] != '\0');i++);
				
				for(j=0;tempLine[i] != '\0';i++,j++)
					tempReg[j] = tempLine[i];
				tempReg[j] = '\0';			/*svae second operand*/

				one = 1;

				/*handle the target operand*/

				if(tempReg[0] == '#')			/*immidiate operand*/
				{
					for(i=0;i<3;i++)
						one = one << 1;
					firstWord |= one;	
					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
						{
							regVal[j] = tempReg[i];
							j++;
						}
					}
					regVal[j] = '\0';
					var = atoi(regVal);		/*extract the number*/

					for(i=0;i<3;i++)
						var = var << 1;
					thirdWord |= var;		/*add to instruction table*/
					thirdWord |= a;			/*set on bit 2, absolute*/
									/*add to instruction table*/
					push_instruction(&instruction_head, iic, firstWord);	
					iic++;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
					push_instruction(&instruction_head, iic, thirdWord);	
					iic++;	
				}

				if(tempReg[0] == '*')			/*indirect register*/
				{
					for(i=0;i<5;i++)
						one = one << 1;
					firstWord |= one;
					push_instruction(&instruction_head, iic, firstWord);	
					iic++;

					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}
					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/	

					for(i=0;i<3;i++)
						var = var << 1;	

					if(twoRegFlag)			/*they're both registers*/
					{
						secondWord |= var;
						push_instruction(&instruction_head, iic, secondWord);	
						iic++;	
						return;
					}
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;

					thirdWord |= var;
					thirdWord |= a;		/*set on bit 2, absolute*/
					push_instruction(&instruction_head, iic, thirdWord);	
					iic++;	
				}

				if(isupper(tempReg[0]))			/*direct operand*/
				{
					for(i=0;i<4;i++)
						one = one << 1;
					firstWord |= one;		
					symbolCheck = check_symbol(&symbol_head, tempReg);

					if(symbolCheck == -1)		/*no match to eny label*/
					{
						printf("error! label name dosn't exsist\n");
						return;
					}

					if(symbolCheck == -2)		/*external label*/
					{
						thirdWord |= e;	/*set on bit 0, external*/
					}

					if(symbolCheck >= 0)		/*relocatable label*/
					{
						for(i=0;i<3;i++)
							symbolCheck = symbolCheck << 1;	
						thirdWord |= symbolCheck;	
						thirdWord |= r;	/*set on bit 1, relocatable*/
					}

					push_instruction(&instruction_head, iic, firstWord);	
					iic++;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
					push_instruction(&instruction_head, iic, thirdWord);	
					iic++;	
				}

				if(islower(tempReg[0]))			/*direct register*/
				{
					for(i=0;i<6;i++)
						one = one << 1;
					firstWord |= one;
					push_instruction(&instruction_head, iic, firstWord);	
					iic++;

					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}

					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/

					for(i=0;i<3;i++)
						var = var << 1;	

					if(twoRegFlag)			/*they're both registers*/
					{
						secondWord |= var;
						push_instruction(&instruction_head, iic, secondWord);	
						iic++;	
						return;
					}
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
					thirdWord |= var;
					thirdWord |= a;		/*set on bit 2, absolute*/
					push_instruction(&instruction_head, iic, thirdWord);	
					iic++;	
				}
				break;
			}	

			if(i>=INS_GROUP1 && i<INS_NO_REG_POS)		/*instruction with one operand*/
			{
				for(i=0;tempLine[i] != ' ';i++);	/*skip the op word*/

				for(j=0;tempLine[i] != '\0';i++,j++)	/*get the target operand*/
				{
						if(tempLine[i] == ',' || tempLine[i] == ' ')
						{
							j--;
							continue;
						}
						tempReg[j] = tempLine[i];
				}

				tempReg[j] = '\0';			/*make it a string*/
				one = 1;

				/*handle the second operand*/

				if(tempReg[0] == '#')			/*immidiate operand*/
				{
					for(i=0;i<3;i++)
						one = one << 1;
					firstWord |= one;	

					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
						{
							regVal[j] = tempReg[i];
							j++;
						}
					}
					regVal[j] = '\0';
					var = atoi(regVal);		/*extract the number*/

					for(i=0;i<3;i++)
						var = var << 1;
					secondWord |= var;		/*add to instruction table*/
					secondWord |= a;		/*set on bit 2, absolute*/
									/*add to instruction table*/
					push_instruction(&instruction_head, iic, firstWord);	
					iic++;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
				}

				if(tempReg[0] == '*')			/*indirect register*/
				{
					for(i=0;i<5;i++)
						one = one << 1;
					firstWord |= one;

					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}
					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/	

					for(i=0;i<3;i++)
						var = var << 1;	

					secondWord |= var;
					secondWord |= a;		/*set on bit 2, absolute*/

					push_instruction(&instruction_head, iic, firstWord);	
					iic++;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
				}

				if(isupper(tempReg[0]))			/*direct operand*/
				{
					for(i=0;i<4;i++)
						one = one << 1;
					firstWord |= one;		
					symbolCheck = check_symbol(&symbol_head, tempReg);

					if(symbolCheck == -1)		/*no match to eny label*/
					{
						printf("error! label name dosn't exsist\n");
						return;
					}

					if(symbolCheck == -2)		/*external label*/
					{
						secondWord |= e;	/*set on bit 0, external*/
					}

					if(symbolCheck >= 0)		/*relocatable label*/
					{
						for(i=0;i<3;i++)
							symbolCheck = symbolCheck << 1;	
						secondWord |= symbolCheck;	
						secondWord |= r;	/*set on bit 1, relocatable*/
					}

					push_instruction(&instruction_head, iic, firstWord);	
					iic++;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;
				}

				if(islower(tempReg[0]))			/*direct register*/
				{
					for(i=0;i<6;i++)
						one = one << 1;
					firstWord |= one;
					push_instruction(&instruction_head, iic, firstWord);	
					iic++;

					for(i=0,j=0;tempReg[i] != '\0';i++)
					{
						if(isdigit(tempReg[i]))
							regVal[j] = tempReg[i];
					}

					regVal[j+1] = '\0';
					var = atoi(regVal);		/*extract the number*/

					for(i=0;i<3;i++)
						var = var << 1;	

					secondWord |= var;
					push_instruction(&instruction_head, iic, secondWord);	
					iic++;	
				}
				break;
			}

			if(i>=INS_NO_REG_POS)				/*instruction with no operands*/
			{
				/*no operands. all operands bits are zero*/

				push_instruction(&instruction_head, iic, firstWord);	/*add it to the instruction table*/
				iic++;
				break;
			}
		}
	}
	
}

/*the directive parse*/

void directive2(char *currentLine, char *name)
{
	/*assignments*/

	FILE *entfp;
	int i, j;
	int label;
	static fpos_t p1;
	static int entryFlag = 0;
	char newWord[WORD_SIZE];
	char tempWord[WORD_SIZE];
	char ent[] = {".entry"};

	char fileName[WORD_SIZE];	/*for file name and extension*/
	char fileExtension[] = ".ent";	

	/*handling the file extension part*/

	fileName[0] = '\0';			/*initialize it every call*/
	strcat(fileName, name);			/*add the name*/
	strcat(fileName, fileExtension);	/*add the extension*/
	
	for(i=0;currentLine[i] != ' ' && currentLine[i] != '\t' && currentLine[i] != '\0';i++)
		tempWord[i] = currentLine[i];
	tempWord[i] = '\0';	/*save directive word*/

	/*skip the directive word and the spaces following*/

	for(i=0;currentLine[i] != ' ' && currentLine[i] != '\t' && currentLine[i] != '\0';i++);
	for(;(currentLine[i] == ' ' || currentLine[i] == '\t') && (currentLine[i] != '\0');i++);

	for(j=0;currentLine[i] != '\0';i++,j++)
	{
		newWord[j] = currentLine[i];
	}
	newWord[j] = '\0';	/*save line without the directive word*/
	
	/*if it is entry*/

	if(!strcmp(ent, tempWord))
	{
		if(entryFlag == 0)
		{
			/*handling opening error*/

			if(!(entfp = fopen(fileName, "w")))	/*fail to create file*/
			{
				fprintf(stderr, "Unable to create file\n");
				return;
			}
		}

		label = check_symbol(&symbol_head, newWord);

		if(label == -1)
		{
			printf("this entry label dont exist\n");
			return;
		}

		fsetpos(entfp, &p1);	/*get current position in the file*/
		fprintf(entfp, "%s\t%04d\n", newWord, label);
		fgetpos(entfp, &p1);	/*svae position in the file*/

		return;
	}

	/*the other directives alredy were handled*/

	return;
}

/*make the object file*/

void make_ob_file(instruction_ptr ip, data_ptr dp, char *name)
{
	/*assignments*/

	FILE *fp;

	char fileName[WORD_SIZE];	/*for file name and extension*/
	char fileExtension[] = ".ob";	

	/*handling the file extension part*/

	fileName[0] = '\0';			/*initialize it every call*/
	strcat(fileName, name);			/*add the name*/
	strcat(fileName, fileExtension);	/*add the extension*/

	/*handling opening error*/

	if(!(fp = fopen(fileName, "w")))/*fail to create file*/
	{
		fprintf(stderr, "Unable to create file\n");
		return;
	}

	fprintf(fp, " %d %d\n", ic, dc);

	while(ip)
	{
		fprintf(fp, "%04d %05o \n", ip->address, ip->data);
		ip = ip->next;		/*push data to the file*/
	}

	while(dp)
	{
		fprintf(fp, "%04d %05o \n", dp->address, dp->data);
		dp = dp->next;		/*push data to the file*/
	}

	return;
}

/*make the external file*/

void make_ext_file(symbol_ptr p, char *name)
{
	/*assignments*/

	FILE *fp;
	symbol_ptr p1, p2;

	char fileName[WORD_SIZE];	/*for file name and extension*/
	char fileExtension[] = ".ext";	

	/*handling the file extension part*/

	fileName[0] = '\0';			/*initialize it every call*/
	strcat(fileName, name);			/*add the name*/
	strcat(fileName, fileExtension);	/*add the extension*/

	/*handling opening error*/

	if(!(fp = fopen(fileName, "w")))/*fail to create file*/
	{
		fprintf(stderr, "Unable to create file\n");
		return;
	}

	p1 = p;

	while(p1)
	{
		if(p1->external == 1)
		{
			fprintf(fp, "%s\t%04d\n", p1->name, p1->address);
		}
		p2 = p1;
		p1 = p2->next;
	}

	return;
}

