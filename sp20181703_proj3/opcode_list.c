#include "20181703.h"


void make_op_list(){
	//Initialize op_list
	for(int i=0;i<20;i++){
		op_list[i] = NULL;
	}

	FILE *fp;
	char opcode[3];
	char mnemonic[7];
	char hash[4];
	fp = fopen("opcode.txt", "r");
	while(fscanf(fp, "%s", opcode) != EOF){
		fscanf(fp, "%s", mnemonic);
		fscanf(fp, "%s", hash);//hash is not use in this homework
		switch(opcode[0]){//store the opcode with first opcode position
			case '0': op_push(opcode, mnemonic, 0, hash); break;
			case '1': op_push(opcode, mnemonic, 1, hash); break;
			case '2': op_push(opcode, mnemonic, 2, hash); break;
			case '3': op_push(opcode, mnemonic, 3, hash); break;
			case '4': op_push(opcode, mnemonic, 4, hash); break;
			case '5': op_push(opcode, mnemonic, 5, hash); break;
			case '6': op_push(opcode, mnemonic, 6, hash); break;
			case '7': op_push(opcode, mnemonic, 7, hash); break;
			case '8': op_push(opcode, mnemonic, 8, hash); break;
			case '9': op_push(opcode, mnemonic, 9, hash); break;
			case 'A': op_push(opcode, mnemonic, 10, hash); break;
			case 'B': op_push(opcode, mnemonic, 11, hash); break;
			case 'C': op_push(opcode, mnemonic, 12, hash); break;
			case 'D': op_push(opcode, mnemonic, 13, hash); break;
			case 'E': op_push(opcode, mnemonic, 14, hash); break;
			case 'F': op_push(opcode, mnemonic, 15, hash); break;
		}//only 16 hash table
	}

	fclose(fp);
	return;
}

void op_push(char *opcode, char *mnemonic, int hashnum, char *hash){
	op_ptr pmove, padd;
	padd = (op_link *)malloc(sizeof(op_link));
	strcpy(padd->opcode, opcode);
	strcpy(padd->mnemonic, mnemonic);
	strcpy(padd->hash, hash);
	padd->link = NULL;
	if(op_list[hashnum] == NULL){
		op_list[hashnum] = padd;
	}
	else{
		pmove = op_list[hashnum];
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}

void op_free(){
	op_ptr pfree, pmove;
	for(int i=0;i<16;i++){
		pmove = op_list[i];
		while(pmove != NULL){
			pfree = pmove;
			pmove = pmove->link;
			free(pfree);
		}
	}
}

void print_oplist(){//print the opcode list by command opcodelist
	op_ptr pmove;
	for(int i=0;i<20;i++){
		pmove = op_list[i];
		printf("%d : ", i);
		if(op_list[i] != NULL){
			printf("[%s,%s]", pmove->mnemonic, pmove->opcode);
			while(pmove->link != NULL){
				pmove = pmove->link;
				printf("->[%s,%s]", pmove->mnemonic, pmove->opcode);
			}
		}
		printf("\n");
	}
	return;
}




