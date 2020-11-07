#include "20181703.h"

void second_linking(int programcount, char *input_program){
	FILE *fp;
	fp = fopen(input_program,"r");
	char input[255]={0};
	char *input_value;
	char *subinput;//for the input
	char store_input[9];

	int location=0;
	int mod_location;//modify location
	int format;
	int record_num;
	int mod_result;
	int minus_or_plus;//minus=0 plus=1
	char record_num_store[3];

	char text_size[3]={0};
	int count_text_size=0;
	int count_location;

	r_ptr pmove_r;
	t_ptr pmove_t;
	es_ptr pmove_es;
	while(fgets(input, 255, fp)!=NULL){
		format=0;
		count_location=0;
		if((subinput = strchr(input,'\n'))!=NULL)*subinput = '\0';//change enter to NULL
		input_value = strtok(input," ");
		if(input_value[0] == 'T'){//if it is text, store the value in variable and push it.
			input_value++;
			strncpy(store_input,input_value,6);
			store_input[6]='\0';
			location=change_to_dec(store_input)+csaddr;
			input_value+=6;
			strncpy(text_size,input_value,2);
			count_text_size = change_to_dec(text_size);
			input_value+=2;

			while(1){
				if(input_value[2]=='1'){//if the format is 4
					format=4;
					strncpy(store_input,input_value,8);
					store_input[8]='\0';
					t_push(location, store_input, format);
					location+=4;
					input_value+=8;
					count_location+=4;
				}
				else if(input_value[2]=='2'){//if format is 3
					format=3;
					strncpy(store_input,input_value,6);
					store_input[6]='\0';
					t_push(location, store_input, format);
					location+=3;
					input_value+=6;
					count_location+=3;
				}
				else if(input_value[0]=='\0'){
					break;
				}
				else{//if format is byte
					format=0;
					strncpy(store_input,input_value,6);
					store_input[6]='\0';
					t_push(location, store_input, format);
					location+=3;
					input_value+=6;
					count_location+=3;
				}
				if(count_text_size < count_location){
					break;
				}
			}
		}
		if(input_value[0] == 'M'){
			minus_or_plus=0;
			input_value++;
			strncpy(store_input,input_value,6);
			store_input[6]='\0';
			mod_location = change_to_dec(store_input) + csaddr;
			pmove_t = t_head;
			while(pmove_t != NULL){
				if(pmove_t->format == 4){
					if(pmove_t->location == mod_location-1){
						break;//because format 4 is modified just 2.5byte
					}
				}
				else{
					if(pmove_t->location == mod_location){
						break;//find the mod_location
					}
				}
				pmove_t = pmove_t->link;
			}
			input_value+=8;
			if(input_value[0]=='+'){
				minus_or_plus=1;
			}
			input_value++;
			strcpy(record_num_store, input_value);
			record_num=change_to_dec(record_num_store);
			pmove_r = r_head[programcount-1];//use record table
			while(pmove_r != NULL){
				if(pmove_r->number==record_num){
					break;
				}
				pmove_r = pmove_r->link;
			}
			pmove_es = es_head;
			while(pmove_es != NULL){
				if(strcmp(pmove_r->symbol_name,pmove_es->sn)==0){
					break;
				}
				else if(strcmp(pmove_r->symbol_name,pmove_es->cs)==0){
					break;
				}
				pmove_es = pmove_es->link;
			}
			if(minus_or_plus==1){
				mod_result = change_to_dec(pmove_t->objcode) + pmove_es->address;
			}
			else{
				mod_result = change_to_dec(pmove_t->objcode) - pmove_es->address;
			}	
			if(pmove_t->format == 4){
				sprintf(pmove_t->objcode,"%08X",mod_result);
			}
			else{
				if(mod_result >= 16777216){
					mod_result -= 16777216;
				}//if it is negative
				sprintf(pmove_t->objcode,"%06X",mod_result);//store objcode
			}
		}
	}
	csaddr = location;
}

void store_dump(){//store value of text portion in the dump
	t_ptr pmove;
	pmove = t_head;
	int location;
	int address;
	char store_obj[3]={0};
	while(pmove != NULL){
		location = pmove->location;
		address = location/16;
		location = location%16;
		if(address == 3){
			printf("%s\n", pmove->objcode);
		}
		if(pmove->format==4){
			for(int i=0;i<4;i++){
				strncpy(store_obj, pmove->objcode+(2*i),2);//cut objcode 2 of them
				dump[address][location+i] = change_to_dec(store_obj);
			}
		}
		else{
			for(int i=0;i<3;i++){
				strncpy(store_obj, pmove->objcode+(2*i),2);
				dump[address][location+i] = change_to_dec(store_obj);
			}
		}
		pmove = pmove->link;
	}
}
void t_push(int location, char *objcode, int format){
	t_ptr padd, pmove;
	padd = (t_table *)malloc(sizeof(t_table));
	padd->link = NULL;
	padd->location = location;
	strcpy(padd->objcode, objcode);
	padd->format = format;
	if(t_head==NULL){
		t_head = padd;
	}
	else{
		pmove = t_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}

