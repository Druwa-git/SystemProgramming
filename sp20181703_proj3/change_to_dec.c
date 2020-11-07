#include "20181703.h"


int change_to_dec(char *input){
	int result=0;
	int length = strlen(input);
	int number;
	int breakflag=0;
	for(int i=0;i<length;i++){//we want to change hexademical to decimal
		switch(input[i]){
			case '0' : number = 0; break;
			case '1' : number = 1; break;
			case '2' : number = 2; break;
			case '3' : number = 3; break;
			case '4' : number = 4; break;
			case '5' : number = 5; break;
			case '6' : number = 6; break;
			case '7' : number = 7; break;
			case '8' : number = 8; break;
			case '9' : number = 9; break;
			case 'A' : case 'a' : number = 10; break;
			case 'B' : case 'b' : number = 11; break;
			case 'C' : case 'c' : number = 12; break;
			case 'D' : case 'd' : number = 13; break;
			case 'E' : case 'e' : number = 14; break;
			case 'F' : case 'f' : number = 15; break;
			default : breakflag=1; break;
		}
		if(breakflag==1){
			result = -1;
			return result;
		}
		for(int j=1;j<length-i;j++){
			number = number * 16;
		}
		result += number;
	}
	return result;
}
