#include "20181703.h"

void edit_dump(int address, int value){
	dump[address/16][address%16] = value;
	return;
}//edit dump only one

void fill_dump(int start, int end, int value){
	for(int i=start/16;i<=end/16;i++){
		for(int j=0;j<16;j++){
			if(i==start/16 && j < start%16){
				continue;
			}
			if(i==end/16 && j > end%16){
				continue;
			}
			dump[i][j] = value;
		}
	}
	return;
}//fill dump with value
