#include "20181703.h"

void history(){
	hi_ptr pmove;
	pmove = hi_head;
	while(pmove != NULL){
		printf("\t%d %s\n", pmove->number, pmove->command);
		pmove = pmove->link;
	}//print the history
	return;
}

void hi_push(char *input){//make history
	hi_count++;
	hi_ptr padd, pmove;
	padd = (hi_link *)malloc(sizeof(hi_link));
	padd->number = hi_count;
	strcpy(padd->command, input);
	padd->link = NULL;
	if(hi_head == NULL){
		hi_head = padd;
	}
	else{
		pmove = hi_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
	return;
}

void hi_free(){//free history memory
	hi_ptr pfree;
	if(hi_head == NULL){
		return;
	}
	else{
		while(hi_head!=NULL){
			pfree = hi_head;
			hi_head = hi_head->link;
			free(pfree);
		}
	}
	return;
}
