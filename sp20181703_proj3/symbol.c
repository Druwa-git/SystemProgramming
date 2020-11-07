#include "20181703.h"

void symbol(){
	opt_ptr pmove;
	pmove = opt_head;
	if(pmove == NULL){
		return;
	}
	pmove = pmove->link; // COPY except
	while(pmove != NULL){
		if(pmove->label[0] != '\0'){//if label[0] = NULL, it didn't have a symbol
			sym_push(pmove->label, pmove->locctr);
		}
		pmove = pmove->link;
	}
	return;
}

int sort_symbol(){
	int locctr=0;
	char label[7];
	sym_ptr key, pmove1, pmove2;
	pmove1 = sym_head;
	if(pmove1 == NULL){
		return 0;
	}
	while(pmove1->link != NULL){
		key = pmove1;
		pmove2 = key->link;
		while(pmove2 != NULL){
			if(strcmp(pmove1->label,pmove2->label)==0){
				printf("%s %s\n", pmove1->label, pmove2->label);
				printf("Variable overlapping in %06X address.\n",pmove1->locctr);
				return 1;
			}//two of string correct, print error
			if(key->label[0] > pmove2->label[0]){
				key = pmove2;
			}
			else if(key->label[0] == pmove2->label[0]){
				if(key->label[1] > pmove2->label[1]){
					key = pmove2;
				}
			}
			pmove2 = pmove2->link;
		}
		strcpy(label, pmove1->label);
		strcpy(pmove1->label, key->label);
		strcpy(key->label, label);
		locctr = pmove1->locctr;
		pmove1->locctr = key->locctr;
		key->locctr = locctr;
		pmove1 = pmove1->link;//swap of more small alphabet just variable.
	}
	return 0;
}

void symbol_print(){
	sym_ptr pmove;
	pmove = sym_head;
	if(pmove == NULL){
		printf("No Symbol In Here.\n");
		return;
	}
	while(pmove != NULL){
		printf("\t%s \t %04X\n",pmove->label,pmove->locctr);
		pmove = pmove->link;
	}
}
