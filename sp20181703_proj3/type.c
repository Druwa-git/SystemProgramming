#include "20181703.h"

void type_print(char *filename){//print file note
	FILE *fp;
	char text[256];
	if((fp = fopen(filename, "r"))==NULL){
		printf("File not exist!!\n");
		return;
	}
	while(fgets(text,255,fp)!=NULL){
		printf("%s", text);
	}
	return;
}
