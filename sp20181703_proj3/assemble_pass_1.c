#include "20181703.h"

int first_assemble(char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r"))==NULL){
		printf("File is not exist.\n");
		return 1;
	}
	char text[256];//input line

	char *textptr;//strtok
	char *subtext;//fgets change last array NULL

	int sublocctr=0;//it just save the before address because address print before address
	int startlocctr=0;//START opcode's operand is startlocctr

	int locctr=0;
	char label[7];
	char opcode[7];
	char opcode_hex[3];
	char operand[8];
	int base;
	int format=0;
	int char_or_hex;
	int hex;
	char bytechar[4];
	char suboperand[3];
	char remark_text[256];//save for remark text

	int charcount;
	int lengthofbyte;
	
	while(fgets(text, 255, fp)!=NULL){
		sublocctr=locctr;
		label[0] = '\0';
		opcode[0] = '\0';
		opcode_hex[0] = '\0';
		operand[0] = '\0';
		base = 0;
		format = 0;
		char_or_hex = 0;
		hex = 0;
		bytechar[0] = '\0';
		suboperand[0] = '\0';
		remark_text[0] = '\0';

		if((subtext = strchr(text, '\n'))!=NULL)*subtext = '\0';//make fgets last array NULL because of Enter 
		if(text[0] == '.'){
			strcpy(remark_text,text);
			opt_push(-2,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);
			continue;
		}//if remark exist
		if(text[0] != ' '){
			textptr = strtok(text," ");
			strcpy(label, textptr);
			textptr = strtok(NULL," ");
			if(strcmp("START", textptr)==0){
				strcpy(opcode, textptr);
				if((textptr = strtok(NULL," "))==NULL){
					locctr = 0;
				}
				startlocctr = change_to_dec(textptr);
				locctr = startlocctr;
				strcpy(operand,textptr);
				opt_push(0,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);
				continue;
			}	
		}//if label's in here
		else{
			textptr = strtok(text," ");
		}//i fno label's in here
		strcpy(opcode, textptr);
		strcpy(opcode_hex, opcode_search_for_asm(opcode, &format));//opcode_search_for_asm is return string of express opcode at hex
		if(opcode_hex[0] != '\0'){//opcode is exist
			if(opcode[0] != '+'){//if it is not format 4
				locctr += format;
				if((textptr = strtok(NULL," "))!=NULL){
					if(textptr[strlen(textptr)-1] == ','){
						strcpy(operand,textptr);
						operand[strlen(textptr)-1] = '\0';
						textptr = strtok(NULL," ");
						strcpy(suboperand,textptr);
					}
					else{
						strcpy(operand,textptr);
					}
				}
			}
			else{//if it is format 4
				locctr += 4;
				format = 4;
				if((textptr = strtok(NULL," "))!=NULL){
					if(textptr[strlen(textptr)-1] == ','){
						strcpy(operand,textptr);
						operand[strlen(textptr)-1] = '\0';
						textptr = strtok(NULL," ");
						strcpy(suboperand,textptr);
					}
					else{
						strcpy(operand,textptr);
					}
				}
			}
		}
		else{
			if(strcmp(opcode,"BASE")==0){
				base = 1;
				textptr = strtok(NULL," ");
				strcpy(operand, textptr);
				strcpy(base_operand,operand);
				opt_push(-1,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);
				continue;
			}//nobase add
			else if(strcmp(opcode,"NOBASE")==0){
				base = 0;
				textptr = strtok(NULL," ");
				strcpy(operand, textptr);
				base_operand[0] = '\0';
				opt_push(-1,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);
				continue;
			}//cancel the base operand
			else if(strcmp(opcode,"RESW")==0){
				textptr = strtok(NULL," ");
				locctr = locctr+(atoi(textptr)*3);
			}//RESW is just multiply 3 address field in one charicter
			else if(strcmp(opcode,"RESB")==0){
				textptr = strtok(NULL," ");
				locctr = locctr+atoi(textptr);
			}//RESB means Byte res, so it just multiply one.
			else if(strcmp(opcode,"BYTE")==0){
				textptr = strtok(NULL," ");
				if(textptr[0] == 'C'){//if byte type is char
					charcount=2;
					lengthofbyte=0;
					while(1){
						if(textptr[charcount]==39){
							break;
						}
						bytechar[lengthofbyte] = textptr[charcount];
						lengthofbyte++;
						charcount++;
					}//copy the charicter in byte
					bytechar[lengthofbyte] = '\0';
					locctr = locctr+lengthofbyte;
					char_or_hex = 1;
				}
				else if(textptr[0] == 'X'){//if byte is hexademical
					charcount=2;
					lengthofbyte=0;
					while(1){
						if(textptr[charcount]==39){
							break;
						}
						bytechar[lengthofbyte] = textptr[charcount];
						lengthofbyte++;
						charcount++;
					}
					bytechar[lengthofbyte] = '\0';
					locctr = locctr+(lengthofbyte/2);
					hex = change_to_dec(bytechar);
					char_or_hex = 2;// it means byte is hexademical
				}
				else{
					printf("Wrong Byte Type in %06X address.\n", locctr);
					return 1;
				}
			}
			else if(strcmp(opcode,"WORD")==0){
				textptr = strtok(NULL," ");
				char_or_hex = 2;
				hex = atoi(textptr);
				locctr += 3;
			}
			else if(strcmp(opcode,"END")==0){
				programlength = locctr - startlocctr;
				textptr = strtok(NULL," ");
				strcpy(operand, textptr);
				break;
			}
			else{
				printf("Wrong Instructions in %06X address.\n", locctr);
				return 1;
			}
			strcpy(operand, textptr);
		}
		opt_push(sublocctr,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);

	}
	opt_push(-1,label,opcode,operand,suboperand,base,format,opcode_hex,char_or_hex,hex,bytechar,remark_text);//-1 locctr means didn't have location like BASE, NOBASE or END
	return 0;
}


void print_assemble_lst(char *filename){
	FILE *fp;
	int file_leng = strlen(filename);
	for(int i=0;i<file_leng-3;i++){
		if(filename[i] == '.'){
			filename[i+1] = 'l';
			filename[i+2] = 's';
			filename[i+3] = 't';
			break;
		}
	}
	fp = fopen(filename,"w");
	opt_ptr pmove;
	pmove = opt_head;
	int line_cnt = 0;
	while(pmove != NULL){
		line_cnt += 5;
		fprintf(fp, "%d\t", line_cnt);
		if(pmove->locctr >= 0){
			fprintf(fp,"%04X\t", pmove->locctr);
		}//print the location
		else if(pmove->locctr == -2){//if this part is remark, print remark_text
			fprintf(fp, "\t%s\n", pmove->remark_text);
			pmove = pmove->link;
			continue;
		}
		else{
			fprintf(fp, "\t");
		}//pmove->locctr = -1 case
		if(pmove->label[0] != '\0'){
			fprintf(fp, "%s\t", pmove->label);
		}
		else{
			fprintf(fp, "\t");
		}
		fprintf(fp, "%s\t",pmove->opcode);
		if(pmove->operand != '\0'){
			fprintf(fp, "%s", pmove->operand);
		}
		else{
			fprintf(fp, "\t");
		}
		if(pmove->suboperand[0] != '\0'){
			fprintf(fp, ",  %s\t", pmove->suboperand);
		}
		else{
			fprintf(fp, "\t\t");
		}//if suboperand not exist.

		if(pmove->format == 0){
			if(strcmp(pmove->opcode,"BYTE")==0){
				if(pmove->char_or_hex == 1){
					fprintf(fp, "%X", pmove->obj_address);
				}
				else if(pmove->char_or_hex == 2){
					fprintf(fp, "%s", pmove->bytechar);
				}//if byte is hexademical, just print it.
			}
			else if(strcmp(pmove->opcode, "WORD")==0){
				fprintf(fp, "%06X", pmove->hex);
			}//please just print it.
		}
		else if(pmove->format == 1){
			fprintf(fp, "%X", pmove->obj_opcode);
		}
		else if(pmove->format == 2){
			fprintf(fp, "%02X%X%X", pmove->obj_opcode, pmove->obj_r1, pmove->obj_r2);
		}
		else if(pmove->format == 3){
			fprintf(fp, "%02X%X%03X", pmove->obj_opcode, pmove->obj_xbpe, pmove->obj_address);
		}
		else if(pmove->format == 4){
			fprintf(fp, "%02X%X%05X", pmove->obj_opcode, pmove->obj_xbpe, pmove->obj_address);
		}
		fprintf(fp, "\n");
		pmove = pmove->link;
	}
	fclose(fp);
	return;
}


void opt_push(int locctr, char *label, char *opcode, char *operand, char *suboperand, int base, int format, char *opcode_hex, int char_or_hex, int hex, char *bytechar, char *remark_text){
	opt_ptr pmove, padd;
	padd = (op_table *)malloc(sizeof(op_table));
	padd->locctr = locctr;
	strcpy(padd->label, label);
	strcpy(padd->opcode, opcode);
	strcpy(padd->operand, operand);
	strcpy(padd->suboperand,suboperand);
	padd->base = base;
	padd->format = format;
	strcpy(padd->opcode_hex,opcode_hex);
	padd->char_or_hex = char_or_hex;
	padd->hex = hex;
	strcpy(padd->bytechar,bytechar);
	padd->obj_address = 0; padd->obj_opcode = 0;
	padd->obj_r1 = 0; padd->obj_r2 = 0;
	padd->obj_xbpe = 0;
	strcpy(padd->remark_text, remark_text);


	padd->link = NULL;
	if(opt_head == NULL){
		opt_head = padd;
	}
	else{
		pmove = opt_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}


void sym_push(char *label, int locctr){
	sym_ptr padd, pmove;
	padd = (sym_table *)malloc(sizeof(sym_table));
	padd->locctr = locctr;
	strcpy(padd->label, label);
	padd->link = NULL;
	if(sym_head == NULL){
		sym_head = padd;
	}
	else{
		pmove = sym_head;
		while(pmove->link != NULL){
			pmove = pmove->link;
		}
		pmove->link = padd;
	}
}


