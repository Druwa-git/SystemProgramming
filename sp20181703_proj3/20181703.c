#include "20181703.h"

int main(){
	char input[MAX_INPUT_SIZE];
	char inputing[MAX_INPUT_SIZE];//temporarily store the input and save the history.
	char *input_mnemonic;//store the mnemonic input
	char *subinput;//remove the enter end of the line
	char input_range[100];//dump input
	char *input_dump;//use pointer in strtok

	char *input_value;//use pointer in strtok
	char input_address[100];//use in fill_dump

	int programcount;
	int input_point_loc;
	int start, end;
	int address, value;
	int error_in_asm;//if occur error in running assemble

	hi_head=NULL;
	sym_head=NULL;
	opt_head=NULL;
	es_head=NULL;
	for(int i=0;i<3;i++){
		r_head[i]=NULL;
	}
	run_head=NULL;
	t_head=NULL;
	hi_count=0;
	dump_count=0;
	base_operand[0] = '\0';
	programlength=0;
	progaddr=0;
	programcount=0;

	make_op_list();
	reset_dump();

	while(1){
		printf("sicsim > ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		if((subinput = strchr(input, '\n'))!= NULL)*subinput = '\0';//remove the enter end of the line
		strcpy(inputing, input);//temporarily store input for history
		if(strcmp(input, "quit")==0 || strcmp(input, "q")==0){
			hi_free();
			op_free();
			sym_free();
			opt_free();
			t_free();
			es_free();
			r_free();
			return 0;
		}
		else if(strcmp(input, "help")==0 || strcmp(input, "h")==0){
			printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] adress, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\n");
		}
		else if(strcmp(input, "dir")==0 || strcmp(input, "d")==0){
			directory();
		}
		else if(strcmp(input, "history")==0 || strcmp(input, "hi")==0){
			hi_push(input);	
			history();//history.c
			continue;
		}
		else if(strcmp(input, "opcodelist")==0){
			print_oplist();//opcode_list.c
		}
		else if(strncmp(input, "opcode ", 5)==0){
			if((input_mnemonic = strtok(input," "))==NULL){
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			if((input_mnemonic = strtok(NULL," "))==NULL){
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			opcode_search(input_mnemonic);//opcode_search.c
		}
		else if(strncmp(input, "dump", 4)==0 || strncmp(input, "du", 2)==0){
			if((input_dump = strtok(input," "))==NULL){//Save the unfortune error 
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			if((input_dump = strtok(NULL," "))==NULL){//only dump
				if(strcmp(input, "dump")!=0 && strcmp(input, "du")!=0){//express like dumps, dumpsdawda is not command
					printf("This command is not valid. Try to command help.\n");
					continue;
				}
				if(dump_count>=65536){//if we went to the end of dump, return to zero position dump
					dump_count = 0;
				}
				print_dump(dump_count*16, (dump_count+10)*16-1);//dump.c
				dump_count += 10;
			}
			else{
				strcpy(input_range,input_dump);
				if((input_dump = strtok(NULL," "))==NULL){//dump start
					start = change_to_dec(input_range);
					if(start == -1){
						printf("Input is not hexademical.\n");
						continue;//if start input is not hex, error print.
					}
					if(start+159 >= 1048576){//if start address close to end, overlap range is not available. 
						end = 1048575;
					}
					else{//i want use only one function to print the dump, so that we print 160 mem 
						end = start+159;
					}
					if(start >= 1048576 || start < 0){
						printf("Out of Range!!\n");
						continue;
					}//range check
					print_dump(start, end);
				}
				else{//dump start, end
					input_range[strlen(input_range)-1] = '\0';
					start = change_to_dec(input_range);
					end = change_to_dec(input_dump);
					if(start == -1 || end == -1){//same as upward
						printf("Input is not hexademical.\n");
						continue;
					}
					if(start >= 1048576 || start < 0 || start > end || end >= 1048576 || end < 0){
						printf("Out of Range!!\n");
						continue;
					}
					print_dump(start, end);
				}
			}
		}
		else if(strncmp(input, "edit", 4)==0 || strncmp(input, "e", 1)==0){
			if((input_value = strtok(input," "))==NULL){
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			if((input_value = strtok(NULL," "))==NULL){//I want to prevent error that only command edit is input case.
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			strcpy(input_address, input_value);
			input_address[strlen(input_address)-1] = '\0';
			if((input_value = strtok(NULL," "))==NULL){//same as upward
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			address = change_to_dec(input_address);
			value = change_to_dec(input_value);
			if(address == -1 || value == -1){
				printf("Input is not hexademical.\n");
				continue;
			}
			if(address >= 1048576 || value >= 256 || address < 0 || value < 0){
				printf("Out of Range!!\n");
				continue;
			}
			edit_dump(address, value);//edit_dump.c
		}
		else if(strncmp(input, "fill", 4)==0 || strncmp(input, "f", 1)==0){
			//range problem exist
			if((input_value = strtok(input," "))==NULL){//I want to prevent error that only command fill is input case.
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			if((input_value = strtok(NULL," "))==NULL){// same as upward
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			strcpy(input_range, input_value);
			input_range[strlen(input_range)-1] = '\0';
			if((input_value = strtok(NULL," "))==NULL){
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			strcpy(input_address, input_value);
			input_address[strlen(input_address)-1] = '\0';
			if((input_value = strtok(NULL," "))==NULL){
				printf("This command is not valid. Try to command help.\n");
				continue;
			}
			start = change_to_dec(input_range);
			end = change_to_dec(input_address);
			value = change_to_dec(input_value);
			if(start == -1||end == -1||value == -1){
				printf("Input is not hexademical.\n");
				continue;
			}
			if(start >= 1048576 || start < 0 || start > end || end >= 1048576 || end < 0 || value >= 256 || value < 0){
				printf("Out of Range!!\n");
				continue;
			}
			fill_dump(start, end, value);//edit_dump.c
		}
		else if(strcmp(input, "reset")==0){
			reset_dump();//dump.c
		}
		//homework2
		else if(strncmp(input, "type", 4)==0){
			input_value = strtok(input," ");
			input_value = strtok(NULL," ");
			type_print(input_value);
		}
		else if(strncmp(input, "assemble", 8)==0){
			error_in_asm = 0;
			input_value = strtok(input," ");
			input_value = strtok(NULL," ");//filename strtok
			if((error_in_asm = first_assemble(input_value))==1){
				opt_free();
				continue;
			}//Wrong Instruction
			symbol();
			if((error_in_asm = sort_symbol())==1){
				opt_free();
				sym_free();
				//symbol free, optable free
				continue;
			}//Symbol overlapping
			if((error_in_asm = second_assemble())==1){
				opt_free();
				sym_free();
				continue;
			}//undefined symbol use
			print_assemble_lst(input_value);
			print_assemble_obj(input_value);
		}
		else if(strcmp(input, "symbol")==0){
			symbol_print();
		}
		else if(strncmp(input, "progaddr", 8)==0){
			input_value = strtok(input," ");
			input_value = strtok(NULL," ");//addr input
			progaddr = change_to_dec(input_value);
		}
		else if(strncmp(input, "loader", 6)==0){
			input_program1[0] = '\0';
			input_program2[0] = '\0';
			input_program3[0] = '\0';
			csaddr = progaddr;
			input_value = strtok(input," ");
			input_value = strtok(NULL," ");
			strcpy(input_program1,input_value);
			if((input_value = strtok(NULL," "))!=NULL){
				strcpy(input_program2,input_value);
			}
			if((input_value = strtok(NULL," "))!=NULL){		
				strcpy(input_program3,input_value);
			}
			programcount=1;
			first_linking(programcount,input_program1);
			if(input_program2[0]!='\0'){
				programcount++;
				first_linking(programcount,input_program2);
				if(input_program3[0]!='\0'){
					programcount++;
					first_linking(programcount,input_program3);
				}
			}
			es_print();
			csaddr = progaddr;
			second_linking(1,input_program1);
			if(programcount==2){
				second_linking(2,input_program2);
			}
			if(programcount==3){
				second_linking(2,input_program2);
				second_linking(3,input_program3);
			}
			store_dump();
			if(!strcmp(input_program1, "copy.obj")){
				make_run_table();
			}
			/*t_free();
			es_free();
			r_free();*/ //not use because of run
		}
		else if(strncmp(input, "bp", 2)==0){
			input_value = strtok(input," ");
			if((input_value = strtok(NULL," "))==NULL){
				bp_print();
			}
			else{
				if(strcmp(input_value, "clear")==0){
					bp_free();
					printf("\t\t[ok] clear all breakpoints\n");
				}
				else{
					input_point_loc = change_to_dec(input_value);
					bp_push(input_point_loc);
					push_bp_in_run(input_point_loc);
					printf("\t\t[ok] create breakpoint %s\n", input_value);
				}
			}
		}
		else if(strcmp(input, "run")==0){
			run();
		}
		else{
			printf("This command is not valid. Try to command help.\n");
			continue;
		}
		hi_push(inputing);//store history
	}
}
