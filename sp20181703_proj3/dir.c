#include "20181703.h"

void directory(){
	DIR *dir_ptr=NULL;
	struct dirent *file = NULL;
	struct stat info;
	if((dir_ptr = opendir(".")) == NULL){
		printf("Directory의 정보를 읽어올 수 없습니다.\n");
		return;
	}
	printf("\t");
	while((file = readdir(dir_ptr)) != NULL){
		if(stat(file->d_name, &info) == -1){
			printf("error\n");
			return;
		}
		if(strcmp(file->d_name,".")!=0 && strcmp(file->d_name,"..")!=0){
			if(S_ISDIR(info.st_mode)!=0){//check the directory
				printf("%s/\t", file->d_name);
			}
			else{
				if(info.st_mode & 01001001){//check the execution file
					printf("%s*\t", file->d_name);
				}
				else{//just object file
					printf("%s\t", file->d_name);
				}
			}
		}
	}
	printf("\n");
	closedir(dir_ptr);
	return;
}
