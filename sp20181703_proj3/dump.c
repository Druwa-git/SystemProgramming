#include "20181703.h"


void reset_dump(){
	for(int i=0;i<65536;i++){
		for(int j=0;j<16;j++){
			dump[i][j] = 0;
		}
	}
}

void print_dump(int start, int end){
	char hex_mem[3];//store value of hexademical temporarily
	char hex_dir[5];//store the address temporarily
	int sub_dump_count;//want to change decimal to hex so borrow mem for a while
	int print_dump_count;// same as overside
	int index;
	for(int i=start/16;i<=end/16;i++){
		sub_dump_count = i;
		index=3;//i store the address in reverse because big number in the leftmost side
		for(int j=0;j<4;j++){
			hex_dir[j] = '0';
		}//initialize the variable to zero
		hex_dir[4] = '\0';//prevent the unfortune error
		while(sub_dump_count/16 != 0){
			print_dump_count = sub_dump_count%16;
			sub_dump_count = sub_dump_count/16;
			if(print_dump_count >= 0  && print_dump_count <= 9){
				hex_dir[index--] = print_dump_count + '0';
			}
			else{
				hex_dir[index--] = print_dump_count + 'A' - 10;
			}
		}
		if(sub_dump_count >= 0  && sub_dump_count <= 9){
			hex_dir[index] = sub_dump_count + '0';
		}
		else{
			hex_dir[index] = sub_dump_count + 'A' - 10;
		}
		printf("%s0 ", hex_dir);//last position of address is always zero.

		for(int j=0;j<16;j++){
			if(i==start/16 && j < start%16){
				printf("   ");
				continue;
			}
			if(i==end/16 && j > end%16){
				printf("   ");
				continue;
			}//if the range not include this address, just express the blank.
			index = 1;
			sub_dump_count = dump[i][j];
			print_dump_count = sub_dump_count%16;//second position of hex mem
			sub_dump_count = sub_dump_count/16;//firstposition of hex mem
			if(print_dump_count >= 0  && print_dump_count <= 9){
				hex_mem[index--] = print_dump_count + '0';
			}
			else{
				hex_mem[index--] = print_dump_count + 'A' - 10;
			}	
			if(sub_dump_count >= 0  && sub_dump_count <= 9){
				hex_mem[index] = sub_dump_count + '0';
			}
			else{
				hex_mem[index] = sub_dump_count + 'A' - 10;
			}
			printf("%s ", hex_mem);
		}
		printf("; ");
		for(int j=0;j<16;j++){//express ASCII code
			if(i==start/16 && j < start%16){
				printf(".");
				continue;
			}
			if(i==end/16 && j > end%16){
				printf(".");
				continue;
			}
			if(dump[i][j] < 20 || dump[i][j] > 126){
				printf(".");
			}//except case
			else{
				printf("%c", dump[i][j]);
			}
		}
		printf("\n");
	}
	return;
}	
