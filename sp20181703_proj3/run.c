#include "20181703.h"

void bp_push(int point_loc){
	bp_ptr padd, pmove;
	padd = (bp_table *)malloc(sizeof(bp_table));
	padd->point_loc = point_loc;
	padd->link = NULL;
	if(bp_head==NULL){
		bp_head = padd;
	}
	else{
		pmove = bp_head;
		while(pmove->link!=NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}

void bp_print(){//breakpoint print
	bp_ptr pmove;
	printf("\t\tbreakpoint\n");
	printf("\t\t----------\n");
	pmove = bp_head;
	while(pmove != NULL){
		printf("\t\t%X\n",pmove->point_loc);
		pmove = pmove->link;
	}
}

void bp_free(){
	bp_ptr pfree;
	if(bp_head == NULL){
		return;
	}
	else{
		while(bp_head != NULL){
			pfree = bp_head;
			bp_head = bp_head->link;
			free(pfree);
		}
	}
	return;
}


void run(){
	int reg1;//만약 register을 이용하는 opcode가 있으면 이 변수 이용
	int breakpoint;
	int end_program = 0;//만약 프로그램이 끝나면 값이 1이 된다.
	int a=0,x=0,l,pc,b=0,s=0,t=0,cc;//cc is compare varialble, so 0 is "=", 1 is "<", 2 is ">"
	l = es_head->length;
	pc = es_head->address;//값 초기화
	run_ptr pmove, pmove_store;
	int length_address=0;
	int buffer_address=0;
	while(1){//or pc counter is overflow
		pmove = run_head;
		if(pc == es_head->length){
			end_program = 1;
			break;
		}//만약 pc가 끝까지 간다면 exit하게 된다.
		while(pmove != NULL){
			if(pmove->location == pc){
				break;
			}
			pmove=pmove->link;
		}
		if(pmove == NULL){
			return;//error가 발생했을 경우 즉, location이 잘못 입력된 경우등을 의미함.
		}
		if(pmove->breakpoint == 1){
			pmove->breakpoint = 0;
			breakpoint = pmove->location;
			break;
		}//if it is breakpoint
		if(pmove->format == 0){
			pc += 3;
			continue;
		}//byte인 경우 "EOF"
		else if(pmove->format == 5){//bytesize is just 1
			pc += 1;
			continue;
		}
		if(pmove->format == 2){
			if(pmove->opcode==180){//CLEAR
				reg1 = pmove->address;
				reg1 = reg1/16;
				if(reg1 == 0) a=0;
				else if(reg1 == 1) x=0;
				else if(reg1 == 2) l=0;
				else if(reg1 == 3) b=0;
				else if(reg1 == 4) s=0;
				else if(reg1 == 5) t=0;
				else if(reg1 == 8) pc=0;
				pc += 2;
			}
			else if(pmove->opcode==160){//COMPR
				cc = 0;
				pc += 2;
			}
			else if(pmove->opcode==184){//TIXR
				x += 1;
				reg1 = pmove->address;
				reg1 = reg1/16;
				if(reg1 == 0) a=0;
				else if(reg1 == 1) reg1 = x;
				else if(reg1 == 2) reg1 = l;
				else if(reg1 == 3) reg1 = b;
				else if(reg1 == 4) reg1 = s;
				else if(reg1 == 5) reg1 = t;
				else if(reg1 == 8) reg1 = pc;

				if(x > reg1) cc = 2;
				else if( x < reg1) cc = 1;
				else cc = 0;

				pc += 2;
			}
		}
		else if(pmove->format == 4){
			if(pmove->opcode == 72){//JSUB
				pc += 4;
				l = pc;
				pc = pmove->address;
			}
			else if(pmove->opcode == 116){
				t = pmove->address;
				pc += 4;
			}//LDT
			else{
				pc += 4;
			}
		}//not pc relative so don't care about it
		else if(pmove->format == 3){
			if(pmove->opcode == 104){//LDB
				b = pmove->address + pmove->location + 3;
				pc += 3;
			}
			else if(pmove->opcode == 0){//LDA
				a = pmove->address + pmove->location + 3;
				if(pmove->address == 38){
					a = length_address;
				}
				else if(pmove->address == 16){
					a = 4542278;
				}//EOF의 값을 의미함
				else if(pmove->address == 3){
					a = 3;
				}
				pc += 3;
			}
			else if(pmove->opcode == 40){//COMP
				if(a > pmove->address + pmove->location + 3) cc = 2;
				else if(a < pmove->address) cc = 1;
				else cc = 0;
				pc += 3;
			}
			else if(pmove->opcode == 48){
				if(cc==0){
					pc = pmove->address + pmove->location + 3;
					if(pc >= 8192){
						pc -= 4096;
					}//if it is negative
				}
				else{
					pc += 3;
				}
			}//JEQ
			else if(pmove->opcode == 60){
				pc = pmove->address + pmove->location + 3;
				if(pc >= 4096){
					pc -= 4096;
				}//if it is negative
				if(pmove->address == 3){
					pc = es_head->length;
					end_program = 1;
					break;
				}//if the code is end. because i didn't use dump, so this case is just use pc change to length when j address have RETADR
			}//J
			else if(pmove->opcode == 224){
				cc=1;
				pc += 3;
			}//TD(설명에 >로 설정하라고 나옴
			else if(pmove->opcode == 56){
				if(cc==1){
					pc = pmove->address + pmove->location +3;
					if(pc >= 8192){
						pc -= 4096;
					}//if it is negative
				}
				else{
					pc += 3;
				}
			}//JLT
			else if(pmove->opcode == 76){
				pc = l;
			}//RSUB
			else if(pmove->opcode == 116){
				t = pmove->address + pmove->location +3;
				if(pmove->address == 0){
					t = length_address;
				}
				pc += 3;
			}//LDT
			else if(pmove->opcode == 80){
				if(x==0){
					a = buffer_address/(16*16*16*16);
				}
				else if(x==1){
					a = buffer_address%(16*16*16*16)/(16*16);
				}
				else if(x==2){
					a = buffer_address%(16*16);
				}
				pc += 3;
			}//LDCH
			else if(pmove->opcode == 16){//STX
				pmove_store = run_head;
				if(pmove->check_base_relative == 1){
					while(pmove_store != NULL){
						if(pmove_store->location == b){
							length_address = x;
						}
						pmove_store = pmove_store->link;
					}
				}
				pc += 3;
			}
			else if(pmove->opcode == 12){//STA
				if(pmove->address == 22){//if buffer address
					buffer_address = a;
				}
				else if(pmove->address == 13){
					length_address = a;
				}
				else if(pmove->address == 35){
					buffer_address = a;
				}
				pc += 3;
			}
			else{
				pc += 3;
			}
		}
	}
	printf("A : %06X\tX : %06X\nL : %06X\tPC : %06X\nB : %06X\tS : %06X\nT : %06X\n", a,x,l,pc,b,s,t);
	if(end_program == 1){//if the program is end
		printf("\t\tEnd Program\n");
	}
	else{
		printf("%d %d\n", pc, es_head->length);
		printf("\t\tStop at checkpoint[%X]\n",breakpoint);
	}
	return;
}


void make_run_table(){
	FILE *fp;
	fp = fopen(input_program1,"r");
	
	char *input;
	input = (char *)malloc(100*sizeof(char));
	char *subinput;//for the input, Enter is Change NULL

	int location;
	char store_loc[7]={0};//int와 char은 한 쌍으로, char형을 int형으로 바꿔주는 역할을 하게되는 변수

	char store_address[7]={0};
	int address;

	char store_opcode[3]={0};
	int opcode;

	char check_format_store[2]={0};
	int check_format;

	int check_imm_indr;
	int check_base_relative;

	int before_location;//it use when push the run_table

	int breakpoint;

	int check_byte=0;//if 0, byte is not exist, 1 is exist

	location = es_head->length;//set location

	while(fgets(input, 99, fp)!=NULL){
		//TODO location break
		if((subinput = strchr(input,'\n'))!=NULL)*subinput = '\0';//last array is NULL
		check_byte = 0;
		if(input[0]=='T'){
			input++;
			strncpy(store_loc,input,6);
			location = change_to_dec(store_loc);
			input+=8;
			while(1){
				before_location = location;
				breakpoint = 0;
				check_imm_indr = 0;
				check_base_relative = 0;
				if(input[0] == '\0'){
					break;
				}
				if(strncmp(input,"454F46",6)==0){
					strncpy(store_address,input,6);
					address=change_to_dec(store_address);
					check_format = 0;
					run_push(location, opcode, check_format, address, breakpoint,check_imm_indr,check_base_relative);
					input+=6;
					location+=3;
					continue;
				}//except EOF byte
				else if(check_byte == 1){
					strncpy(store_address,input,2);
					address=change_to_dec(store_address);
					check_format = 5;
					run_push(location, opcode, check_format, address, breakpoint,check_imm_indr,check_base_relative);
					input+=2;
					location+=1;
					check_byte=0;
					continue;
				}//except the byte;
				strncpy(store_opcode,input,2);
				opcode = change_to_dec(store_opcode);
				check_imm_indr = opcode%4;
				opcode = opcode - check_imm_indr;
				input+=2;
				if(opcode == 180){//CLEAR
					strncpy(store_address,input,2);
					store_address[2]='\0';
					address = change_to_dec(store_address);
					check_format = 2;
					input += 2;
					location += 2;
				}
				else if(opcode == 160){//COMPR
					strncpy(store_address,input,2);
					store_address[2]='\0';
					address = change_to_dec(store_address);
					check_format = 2;
					input += 2;
					location += 2;
				}
				else if(opcode == 184){//TIXR
					strncpy(store_address,input,2);
					store_address[2]='\0';
					address = change_to_dec(store_address);
					check_format = 2;
					input += 2;
					location += 2;
				}//format 2
				else{//format 3,4 or byte
					check_format_store[0] = input[0];
					check_format = change_to_dec(check_format_store);
					check_base_relative = check_format/4;

					input++;
					if(check_format%2==1){
						strncpy(store_address, input, 5);
						store_address[5]='\0';
						address = change_to_dec(store_address);
						check_format = 4;
						location += 4;
						input += 5;
					}
					else{
						strncpy(store_address,input,3);
						store_address[3]='\0';
						address = change_to_dec(store_address);
						check_format = 3;
						location += 3;
						input += 3;
						if(opcode == 76){
							check_byte = 1;
						}//RSUB because byte exist after the RSUB
					}
				}
				run_push(before_location, opcode, check_format, address,breakpoint,check_imm_indr,check_base_relative);
			}
		}
	}
	return;
}

void run_push(int location, int opcode, int format, int address, int breakpoint, int check_imm_indr, int check_base_relative){
	run_ptr padd, pmove;
	padd = (run_table *)malloc(sizeof(run_table));
	padd->location = location;
	padd->opcode = opcode;
	padd->format = format;
	padd->address = address;
	padd->breakpoint = breakpoint;
	padd->check_imm_indr = check_imm_indr;
	padd->check_base_relative = check_base_relative;
	padd->link = NULL;
	if(run_head == NULL){
		run_head = padd;
	}
	else{
		pmove = run_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}


void push_bp_in_run(int breakpoint){
	run_ptr pmove;
	pmove = run_head;
	while(pmove != NULL){
		if(pmove->location == breakpoint){//find location of breakpoint
			pmove->breakpoint = 1;
		}
		pmove = pmove->link;
	}
	return;
}
