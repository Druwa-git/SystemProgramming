#include "20181703.h"
void first_linking(int programcount, char *input_program){
	FILE *fp;
	fp = fopen(input_program,"r");
	char input[100];
	char *input_value;
	char *subinput;//for the input


	char cs[10];//control section
	char sn[10];//symbol name
	char cs_main[10];//project first name

	char ch_address[9];//into char and change to decimal address(int)
	int address;//change ch_address to integer use change_to_dec
	int length;
	while(fgets(input, 99, fp)!=NULL){
		cs[0]='\0';
		sn[0]='\0';
		ch_address[0]='\0';
		if((subinput = strchr(input,'\n'))!=NULL)*subinput = '\0';//last array is NULL 
		if(input[0]=='H'){//program start point case
			input_value=strtok(input," ");
			strcpy(cs,input_value+1);
			input_value=strtok(NULL," ");
			strncpy(ch_address,input_value,6);
			ch_address[6] = '\0';
			address = change_to_dec(ch_address) + csaddr;
			strncpy(ch_address,input_value+6,6);
			ch_address[6] = '\0';
			length = change_to_dec(ch_address);
			es_push(cs,sn,address,length);
			strcpy(cs_main, cs);
		}
		else if(input[0]=='D'){//store symbol location
			input_value=strtok(input," ");
			input_value++;
			while(1){
				strcpy(sn,input_value);
				input_value=strtok(NULL," ");
				strncpy(ch_address, input_value, 6);
				ch_address[6] = '\0';
				address = change_to_dec(ch_address) + csaddr;
				es_push(cs,sn,address,length);
				if(input_value[6]=='\0'){
					break;
				}
				input_value+=6;
			}
		}
		else if(input[0]=='R'){//store other object file symbol because of modification
			r_push(programcount, 1, cs_main);
			input_value=strtok(input," ");
			input_value++;
			while(1){
				strncpy(ch_address,input_value,2);
				ch_address[2]='\0';
				address = change_to_dec(ch_address);
				strcpy(sn, input_value+2);
				r_push(programcount, address, sn);
				if((input_value=strtok(NULL," "))==NULL){
					break;
				}
			}
		}
	}
	csaddr += length;
	return;
}

void es_print(){
	printf("control\tsymbol\taddress\tlength\n");
	printf("section\tname\n");
	printf("-----------------------------\n");
	es_ptr pmove;
	int total_length=0;
	pmove = es_head;
	if(es_head==NULL){
		return;
	}
	else{
		while(pmove!=NULL){
			if(pmove->cs[0] == '\0'){
				printf("\t%s\t",pmove->sn);
			}
			else{
				printf("%s\t\t",pmove->cs);
			}
			printf("%04X\t",pmove->address);
			if(pmove->cs[0] != '\0'){
				printf("%04X", pmove->length);
				total_length += pmove->length;
			}
			printf("\n");
			pmove = pmove->link;
		}
	}
	printf("-----------------------------\n");
	printf("\ttotal length %4X\n", total_length);
}


void es_push(char *cs, char *sn, int address, int length){
	es_ptr padd, pmove;
	padd = (es_table *)malloc(sizeof(es_table));
	strcpy(padd->cs, cs);
	strcpy(padd->sn, sn);
	padd->address = address;
	padd->length = length;
	padd->link = NULL;
	if(es_head == NULL){
		es_head = padd;
	}
	else{
		pmove = es_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}

void r_push(int programcount, int number, char *symbol_name){
	r_ptr padd, pmove;
	padd = (r_table *)malloc(sizeof(r_table));
	padd->number = number;
	strcpy(padd->symbol_name, symbol_name);
	padd->link=NULL;
	if(r_head[programcount-1]==NULL){
		r_head[programcount-1] = padd;
	}
	else{
		pmove=r_head[programcount-1];
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}

//make es free r free
