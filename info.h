/*statements*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Constants*/

#define LINE_SIZE 80
#define WORD_SIZE 20
#define RESERVED_WORD_NUM 28
#define INSTRUCTION_WORD_NUM 16
#define REGISTER_NUM 8
#define INS_GROUP1 5
#define INS_GROUP2 9
#define INS_GROUP3 2 
#define INS_NO_REG_POS 14

/*function declaration*/

int assembler(char[]);			/*main function*/
void second_check(FILE *, char *);	/*the main second check*/

void directive1(char *);		/*handle directive lines in first check*/
void directive2(char *, char *);	/*handle directive lines in second check*/
void instruction1(char *);		/*handle insruction lines in first check*/
void instruction2(char *);		/*handle insruction lines in second check*/

int reserved_words(char word[]);	/*check if variable name is reserved word*/
void remuve_spaces(char *);		/*remuve space from string*/
int isdirective(char *);		/*check word if its directive*/
int isinstruction(char *);		/*check word if its insruction*/
int is_register(char *);		/*check word if its register name*/

