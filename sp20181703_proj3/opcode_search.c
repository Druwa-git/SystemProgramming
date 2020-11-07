#include "20181703.h"

void opcode_search(char *input){//This function search the opcode by command opcode mnemonic
	op_ptr pmove;
	int breaknum=0;
	for(int i=0;i<20;i++){
		pmove = op_list[i];
		if(pmove != NULL){
			while(pmove != NULL){
				if(strcmp(pmove->mnemonic,input)==0){
					breaknum = 1;
					break;
				}
				pmove = pmove->link;
			}
		}
		if(breaknum == 1){
			break;
		}
	}
	if(breaknum == 0){
		printf("This mnemonic is not to enroll opcode.txt\n");
	}
	else{
		printf("opcode is %s\n",pmove->opcode);
	}
	return;
}

char *opcode_search_for_asm(char *input, int *format){
	op_ptr pmove;
	int breaknum=0;
	char *opcode;
	char subinput[7];
	int length = strlen(input);
	opcode = (char *)malloc(sizeof(char)*3);
	if(input[0] == '+'){
		for(int i=0; i<length ; i++){
			subinput[i] = input[i+1];
		}
	}
	else{
		strcpy(subinput, input);
	}
	for(int i=0;i<20;i++){
		pmove = op_list[i];
		if(pmove != NULL){
			while(pmove != NULL){
				if(strcmp(pmove->mnemonic,subinput)==0){
					breaknum = 1;
					break;
				}
				pmove = pmove->link;
			}
		}
		if(breaknum == 1){
			break;
		}
	}
	if(breaknum == 0){
		opcode[0] = '\0';
		return opcode;
	}
	else{
		strcpy(opcode, pmove->opcode);
		if(strcmp(pmove->hash, "1")==0)
			*format = 1;
		else if(strcmp(pmove->hash, "2")==0)
			*format = 2;
		else if(strcmp(pmove->hash, "3/4")==0)
			*format = 3;
	}
	return opcode;
}
