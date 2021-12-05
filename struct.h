/*header file for structurs*/

/*Constants*/

#define MOV_BIT 0		/*op codes to set on specific bits*/
#define CMP_BIT 2048
#define ADD_BIT 4096
#define SUB_BIT 6144
#define LEA_BIT 8192
#define CLR_BIT 10240
#define NOT_BIT 12288
#define INC_BIT 14336
#define DEC_BIT 16384
#define JMP_BIT 18432
#define BNE_BIT 20480
#define RED_BIT 22528
#define PRN_BIT 24576
#define JSR_BIT 26624
#define RTS_BIT 28672
#define STOP_BIT 30720
#define A_BIT 4
#define R_BIT 2
#define E_BIT 1

/*typdefs declaration*/

typedef struct symbol_node * symbol_ptr;
struct symbol_node
{
	char name[WORD_SIZE];
	int address;
	int directive;
	int external;
	symbol_ptr next;
}symbol_table;

typedef struct data_node * data_ptr;
typedef struct data_node
{
	int address;
	short data;
	data_ptr next;
}data_table;

typedef struct instruction_node * instruction_ptr;
struct instruction_node
{
	int address;
	short data;
	instruction_ptr next;
}instruction_table;

/*variable declaration*/

extern int ic;
extern int dc;
extern int unknown_address;
extern symbol_ptr symbol_head;
extern data_ptr data_head;
extern instruction_ptr instruction_head;

/*functions declaration*/

void push_symbol(symbol_ptr *, char *, int , int , int);
void print_symbol(symbol_ptr);
void free_symbol(symbol_ptr *);
int check_symbol(symbol_ptr *, char *);
void make_ext_file(symbol_ptr, char *);

void push_data(data_ptr *, int, short);
void print_data(data_ptr);
void free_data(data_ptr *);

void push_instruction(instruction_ptr *, int, short);
void print_instruction(instruction_ptr);
void free_instruction(instruction_ptr *);
void make_ob_file(instruction_ptr, data_ptr, char *);

void var_address(symbol_ptr *);
void data_address(data_ptr *);

