#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<math.h>

#define MAX_INPUT_SIZE 100
//History Data Structure
typedef struct _HI_LINK *hi_ptr;
typedef struct _HI_LINK{
	int number;
	char command[100];
	hi_ptr link;
}hi_link;
hi_ptr hi_head;
int hi_count;

//Opcode Data Structure
typedef struct _OP_LINK *op_ptr;
typedef struct _OP_LINK{
	char opcode[3];
	char mnemonic[7];
	char hash[4];
	op_ptr link;
}op_link;
op_ptr op_list[20];
int dump[65536][16];
int dump_count;

typedef struct _SYM_TABLE *sym_ptr;
typedef struct _SYM_TABLE{
	char label[7];
	int locctr;
	sym_ptr link;
}sym_table;
sym_ptr sym_head;

typedef struct _OP_TABLE *opt_ptr;
typedef struct _OP_TABLE{
	int locctr;//END or BASE : -1, REMARK : -2
	char label[7];
	char opcode[7];
	char operand[8];
	char opcode_hex[3];
	int base; //if it is base, value is one
	int format; //if operand need suboperand, value is one.
	int char_or_hex;//(char=1 / hex=2)
	int hex;
	char bytechar[10];
	char suboperand[3];
	int obj_opcode;//object
	int obj_r1;
	int obj_r2;
	int obj_xbpe;
	int obj_address;//byte address or format 3,4
	int check_modify;//if value is 1, constant value, else not
	char remark_text[256];//remark text
	opt_ptr link;
}op_table;
opt_ptr opt_head;
char base_operand[8];
int programlength;

//homework 3
typedef struct _ES_TABLE *es_ptr;
typedef struct _ES_TABLE{
	char cs[10];
	char sn[10];
	int address;
	int length;
	es_ptr link;
}es_table;
es_ptr es_head;
typedef struct _R_TABLE *r_ptr;
typedef struct _R_TABLE{
	int number;
	char symbol_name[10];
	r_ptr link;
}r_table;
r_ptr r_head[3];
typedef struct _T_TABLE *t_ptr;
typedef struct _T_TABLE{
	int location;
	char objcode[9];
	int format;
	t_ptr link;
}t_table;
t_ptr t_head;
int progaddr;
int csaddr;
char input_program1[MAX_INPUT_SIZE];//if the input_program is right.
char input_program2[MAX_INPUT_SIZE];
char input_program3[MAX_INPUT_SIZE];



//run
typedef struct _BP_TABLE *bp_ptr;
typedef struct _BP_TABLE{
	int point_loc;
	bp_ptr link;
}bp_table;
bp_ptr bp_head;
typedef struct _RUN_TABLE *run_ptr;
typedef struct _RUN_TABLE{
	int location;
	int opcode;
	int format;
	int address;
	int breakpoint;//if it is breakpoint, value is one
	int check_imm_indr;//1 is imm, 2 is indr, 3 is normal
	int check_base_relative;//1 is only base, 3 is x and base
	run_ptr link;
}run_table;
run_ptr run_head;



void directory();

void history();
void hi_push(char *input);
void hi_free();

void make_op_list();
void op_push(char *opcode, char *mnemonic, int hashnum, char *hash);
void op_free();
void print_oplist();
void opcode_search(char *input);

int change_to_dec(char *input);

void reset_dump();
void print_dump(int start, int end);
void edit_dump(int address, int value);
void fill_dump(int start, int end, int value);

void type_print(char *filename);

//assemble
char *opcode_search_for_asm(char *input, int *format);
int first_assemble(char *filename);
void opt_push(int locctr, char *label, char *opcode, char *operand, char *suboperand, int base, int format, char *opcode_hex, int char_or_hex, int hex, char *bytechar, char *remark_text);
void sym_push(char *label, int locctr);

int second_assemble();
void print_assemble_lst(char *filename);
void print_assemble_obj(char *filename);


void symbol();
int sort_symbol();
void symbol_print();


void sym_free();
void opt_free();

//linnking_loader
void first_linking(int programcount, char *input_program);
void es_push(char *cs, char *sn, int address, int length);
void r_push(int programcount, int number, char *symbol_name);
void es_print();

void second_linking(int programcount, char *input_program);
void store_dump();
void t_push(int location, char *objcode, int format);


void t_free();
void r_free();
void es_free();

//run
void bp_push(int point_loc);
void bp_print();
void bp_free();
void push_bp_in_run(int breakpoint);

void run();
void make_run_table();
void run_push(int location, int opcode, int format, int address, int breakpoint, int check_imm_idr, int check_base_relative);
