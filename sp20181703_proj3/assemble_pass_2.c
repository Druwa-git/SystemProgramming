#include "20181703.h"

int second_assemble(){
	opt_ptr pmove = opt_head;
	sym_ptr sym_pmove;
	int byte_result;
	int opcode_integer;//exist for format 3 and 4 becasue of n,i register and opcode
	int operand_locctr;//save linkedlist address
	int operand_locctr_save;//save for format 3 base counter
	char operand[8];
	int str_byte;

	int operand_length;

	while(1){
		sym_pmove = sym_head;
		pmove = pmove->link;
		operand_locctr=-1;
		byte_result=0;
		str_byte=0;
		if(strcmp(pmove->opcode,"END")==0){
			break;
		}
		if(pmove->format == 0){
			if(strcmp(pmove->opcode,"BYTE")==0){
				if(pmove->char_or_hex == 1){
					str_byte = strlen(pmove->bytechar);
					for(int i=str_byte-1;i>=0;i--){
						byte_result=pmove->bytechar[str_byte-i-1];
						for(int j=i;j>0;j--){
							byte_result = byte_result*16*16;
						}
						pmove->obj_address += byte_result;
					}
				}
				else if(pmove->char_or_hex == 2){
					pmove->obj_address = pmove->hex;
				}
			}
			else if(strcmp(pmove->opcode, "WORD")==0){
				pmove->obj_address = pmove->hex;
			}//if opcode is word, just change the hex to address
		}
		else if(pmove->format == 1){
			pmove->obj_opcode = change_to_dec(pmove->opcode_hex);
		}
		else if(pmove->format == 2){
			pmove->obj_opcode = change_to_dec(pmove->opcode_hex);
			if(strcmp(pmove->operand,"A")==0)
				pmove->obj_r1 = 0;
			if(strcmp(pmove->operand,"X")==0)
				pmove->obj_r1 = 1;
			if(strcmp(pmove->operand,"L")==0)
				pmove->obj_r1 = 2;
			if(strcmp(pmove->operand,"B")==0)
				pmove->obj_r1 = 3;
			if(strcmp(pmove->operand,"S")==0)
				pmove->obj_r1 = 4;
			if(strcmp(pmove->operand,"T")==0)
				pmove->obj_r1 = 5;
			if(strcmp(pmove->operand,"F")==0)
				pmove->obj_r1 = 6;
			if(strcmp(pmove->operand,"PC")==0)
				pmove->obj_r1 = 8;
			if(strcmp(pmove->operand,"SW")==0)
				pmove->obj_r1 = 9;
			if(pmove->suboperand != '\0'){
				if(strcmp(pmove->suboperand,"A")==0)
					pmove->obj_r2 = 0;
				if(strcmp(pmove->suboperand,"X")==0)
					pmove->obj_r2 = 1;
				if(strcmp(pmove->suboperand,"L")==0)
					pmove->obj_r2 = 2;
				if(strcmp(pmove->suboperand,"B")==0)
					pmove->obj_r2 = 3;
				if(strcmp(pmove->suboperand,"S")==0)
					pmove->obj_r2 = 4;
				if(strcmp(pmove->suboperand,"T")==0)
					pmove->obj_r2 = 5;
				if(strcmp(pmove->suboperand,"F")==0)
					pmove->obj_r2 = 6;
				if(strcmp(pmove->suboperand,"PC")==0)
					pmove->obj_r2 = 8;
				if(strcmp(pmove->suboperand,"SW")==0)
					pmove->obj_r2 = 9;
			}//define the register use if.
		}
		else if(pmove->format == 3){
			if(pmove->operand == '\0'){
				opcode_integer = change_to_dec(pmove->opcode_hex);
				opcode_integer = opcode_integer - (opcode_integer%4);
				pmove->obj_opcode += (opcode_integer+3);
				continue;
			}
			if(pmove->operand[0] == '#'){
				pmove->obj_opcode += 1;//n,i is 01
				opcode_integer = change_to_dec(pmove->opcode_hex) ;
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}// it means n,i register is 0, 1
			else if(pmove->operand[0] == '@'){
				pmove->obj_opcode += 2;//n, i is 10
				opcode_integer = change_to_dec(pmove->opcode_hex) ;
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}// it means n,i register is 1, 0
			else{
				pmove->obj_opcode += 3;//n,i is 11
				opcode_integer = change_to_dec(pmove->opcode_hex) ;
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}
			if(pmove->operand[0] == '#' || pmove->operand[0] == '@'){
				strncpy(operand, pmove->operand+1, strlen(pmove->operand));
			}
			else{
				strcpy(operand, pmove->operand);
			}
			while(sym_pmove != NULL){
				if(strcmp(sym_pmove->label,operand)==0){
					operand_locctr = sym_pmove->locctr;
					break;
				}
				sym_pmove = sym_pmove->link;
			}
			if(operand_locctr != -1){//if symbol is exist in symbol table.
				operand_locctr_save = operand_locctr - (pmove->locctr+3);//gap of address
				if(operand_locctr_save >= 4096 || operand_locctr_save < -4096){//base count check
					pmove->obj_xbpe += 4;
					sym_pmove = sym_head;
					if(base_operand[0] == '\0'){
						printf("Base Operand Not exist.\n");
						return 1;
					}
					while(sym_pmove != NULL){
						if(strcmp(sym_pmove->label,base_operand)==0){
							pmove->obj_address = operand_locctr - sym_pmove->locctr;
							break;
						}
						sym_pmove = sym_pmove->link;
					}
					if(pmove->suboperand[0] == 'X'){
						pmove->obj_xbpe += 8;
					}
				}
				else if(operand_locctr_save < 0){
					pmove->obj_address = 4096 + operand_locctr_save;
					pmove->obj_xbpe += 2;
				}
				else{
					pmove->obj_address = operand_locctr_save;
					pmove->obj_xbpe += 2;
				}
			}
			else{//if symbol is not exist in symbol table(constant value or error)
				operand_length = strlen(operand);
				for(int i=0;i<operand_length;i++){
					if(operand[i] > '9' || operand[i] < '0'){
						printf("Wrong Variable in %06X address.\n", pmove->locctr); 
						return 1;
					}
				}
				pmove->obj_address = atoi(operand);
			}
		}
		else if(pmove->format == 4){
			if(pmove->operand[0] == '#'){
				pmove->obj_opcode += 1;
				opcode_integer = change_to_dec(pmove->opcode_hex);
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}
			else if(pmove->operand[0] == '@'){
				pmove->obj_opcode += 2;
				opcode_integer = change_to_dec(pmove->opcode_hex) ;
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}
			else{
				pmove->obj_opcode += 3;
				opcode_integer = change_to_dec(pmove->opcode_hex) ;
				opcode_integer = opcode_integer - (opcode_integer%4); //opcode last two position cut
				pmove->obj_opcode += opcode_integer;
			}
			if(pmove->operand[0] == '#' || pmove->operand[0] == '@'){
				strncpy(operand, pmove->operand+1, strlen(pmove->operand));
			}
			else{
				strcpy(operand, pmove->operand);
			}
			while(sym_pmove != NULL){
				if(strcmp(sym_pmove->label,operand)==0){
					operand_locctr = sym_pmove->locctr;
					break;
				}
				sym_pmove = sym_pmove->link;
			}
			if(operand_locctr != -1){//if operand exist in the symbol table.
				pmove->obj_address += operand_locctr;
				pmove->check_modify = 0;
			}
			else{
				operand_length = strlen(operand);
				for(int i=0;i<operand_length;i++){
					if(operand[i] > '9' || operand[i] < '0'){
						return 1;
					}
				}
				pmove->obj_address = atoi(operand);
				pmove->check_modify = 1;//because it is constant value, so not symobl and it didn't want to check the modify.
			}
			pmove->obj_xbpe = 1;
		}
	}
	return 0;
}


void print_assemble_obj(char *filename){
	FILE *fp;
	int file_leng = strlen(filename);
	for(int i=0;i<file_leng-3;i++){
		if(filename[i] == '.'){
			filename[i+1] = 'o';
			filename[i+2] = 'b';
			filename[i+3] = 'j';
			break;
		}
	}//change the file name .obj
	fp = fopen(filename,"w");
	opt_ptr pmove, pmove_print, pmove_mod;
	sym_ptr sym_pmove;
	pmove = opt_head;
	int check_length;//count the length of text(object code length)
	int personal_length;//define text length of my own property
	int res_flag;//if value is RESW or RESB, we don't care abount this.
	fprintf(fp,"H%s  %06X%06X\n",pmove->label,pmove->link->locctr,programlength);
	pmove = pmove->link;//first pmove use just count the text size(it's really important)
	while(strcmp(pmove->opcode,"END")!=0){
		pmove_print = pmove;
		check_length = 0;
		res_flag = 0;
		personal_length = 60;
		while(1){
			if(pmove->link == NULL){
				break;
			}
			if(strcmp(pmove->opcode,"RESB")!=0&& strcmp(pmove->opcode,"RESW")!=0 && res_flag == 1 && pmove->locctr != -2){
				break;
			}
			if(pmove->format == 0){
				if(strcmp(pmove->opcode,"BYTE")==0){
					if(pmove->char_or_hex == 1){
						check_length += (strlen(pmove->bytechar)*2);
						if(check_length > personal_length){
							check_length -= (strlen(pmove->bytechar)*2);
							break;
						}
					}
					else if(pmove->char_or_hex == 2){
						check_length += (strlen(pmove->bytechar));
						if(check_length > personal_length){
							check_length -= (strlen(pmove->bytechar));
							break;
						}
					}
				}
				else if(strcmp(pmove->opcode,"WORD")==0){
					check_length += 6;
					if(check_length > personal_length){
						check_length -= 6;
						break;
					}
				}
				else if(strcmp(pmove->opcode,"RESB")==0 || strcmp(pmove->opcode,"RESW")==0 || pmove->locctr == -2){
					res_flag = 1;
					pmove = pmove->link;
					continue;
				}
			}
			else if(pmove->format == 1){
				check_length += 2;
				if(check_length > personal_length){
					check_length -= 2;
					break;
				}
			}
			else if(pmove->format == 2){
				check_length += 4;
				if(check_length > personal_length){
					check_length -= 4;
					break;
				}
			}
			else if(pmove->format == 3){
				check_length += 6;
				if(check_length > personal_length){
					check_length -= 6;
					break;
				}
			}
			else if(pmove->format == 4){
				check_length += 8;
				if(check_length > personal_length){
					check_length -= 8;
					break;
				}
			}
			pmove = pmove->link;
		}
		fprintf(fp,"T%06X%02X",pmove_print->locctr,check_length/2);
		personal_length = check_length;//limit length
		check_length = 0;
		res_flag = 0;
		while(1){//second pmove_print is just print the object code text by check_length.
			if(pmove_print->link == NULL){
				break;
			}
			if(pmove_print->format == 0){
				if(strcmp(pmove_print->opcode,"BYTE")==0){
					if(pmove_print->char_or_hex == 1){
						check_length += (strlen(pmove_print->bytechar)*2);
						if(check_length > personal_length){
							check_length -= (strlen(pmove_print->bytechar)*2);
							break;
						}
						fprintf(fp,"%X", pmove_print->obj_address);
					}
					else if(pmove_print->char_or_hex == 2){
						check_length += (strlen(pmove_print->bytechar));
						if(check_length > personal_length){
							check_length -= (strlen(pmove_print->bytechar));
							break;
						}
						fprintf(fp,"%s", pmove_print->bytechar);
					}
				}
				else if(strcmp(pmove_print->opcode,"WORD")==0){
					check_length += 6;
					if(check_length > personal_length){
						check_length -= 6;
						break;
					}
					fprintf(fp,"%06X", pmove_print->hex);
				}
				else if(strcmp(pmove_print->opcode,"RESB")==0 || strcmp(pmove_print->opcode,"RESW")==0){
					res_flag = 1;
					pmove_print = pmove_print->link;
					break;
				}
			}
			else if(pmove_print->format == 1){
				check_length += 2;
				if(check_length > personal_length){
					check_length -= 2;
					break;
				}
				fprintf(fp,"%X", pmove_print->obj_opcode);
			}
			else if(pmove_print->format == 2){
				check_length += 4;
				if(check_length > personal_length){
					check_length -= 4;
					break;
				}
				fprintf(fp,"%02X%X%X", pmove_print->obj_opcode, pmove_print->obj_r1, pmove_print->obj_r2);
			}
			else if(pmove_print->format == 3){
				check_length += 6;
				if(check_length > personal_length){
					check_length -= 6;
					break;
				}
				fprintf(fp,"%02X%X%03X", pmove_print->obj_opcode, pmove_print->obj_xbpe, pmove_print->obj_address);
			}
			else if(pmove_print->format == 4){
				check_length += 8;
				if(check_length > personal_length){
					check_length -= 8;
					break;
				}
				fprintf(fp,"%02X%X%05X", pmove_print->obj_opcode, pmove_print->obj_xbpe, pmove_print->obj_address);
			}
			pmove_print = pmove_print->link;
		}
		fprintf(fp,"\n");
	}
	pmove_mod = opt_head;
	sym_pmove = sym_head;
	while(pmove_mod != NULL){//print the modify address
		if(pmove_mod->format == 4){
			while(sym_pmove != NULL){
				if(strncmp(sym_pmove->label, pmove_mod->operand+1, strlen(sym_pmove->label))){
					if(pmove_mod->check_modify == 0){
						fprintf(fp,"M%06X05\n", pmove_mod->locctr + 1);
					}
					break;
				}
				sym_pmove = sym_pmove->link;
			}
		}
		pmove_mod = pmove_mod->link;
	}
	fprintf(fp,"E%06X\n",opt_head->link->locctr);
	fclose(fp);
	return;
}
