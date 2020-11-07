#include "20181703.h"

void sym_free(){//free symbol table memory 
	sym_ptr pfree;
	if(sym_head == NULL){
		return;
	}
	else{
		while(sym_head!=NULL){
			pfree = sym_head;
			sym_head = sym_head->link;
			free(pfree);
		}
	}
	return;
}

void opt_free(){//free opcode table memory
	opt_ptr pfree;
	if(opt_head == NULL){
		return;
	}
	else{
		while(opt_head!=NULL){
			pfree = opt_head;
			opt_head = opt_head->link;
			free(pfree);
		}
	}
	return ;
}

void t_free(){
	t_ptr pfree;
	if(t_head == NULL){
		return;
	}
	else{
		while(t_head!=NULL){
			pfree = t_head;
			t_head = t_head->link;
			free(pfree);
		}
	}
	return;
}

void r_free(){
	r_ptr pfree;
	for(int i=0;i<3;i++){
		if(r_head[i] == NULL){
			continue;
		}
		else{
			while(r_head[i]!=NULL){
				pfree = r_head[i];
				r_head[i] = r_head[i]->link;
				free(pfree);
			}
		}
	}
	return;
}

void es_free(){
	es_ptr pfree;
	if(es_head == NULL){
		return;
	}
	else{
		while(es_head!=NULL){
			pfree = es_head;
			es_head = es_head->link;
			free(pfree);
		}
	}
	return;
}


