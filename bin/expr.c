#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
	if(argc >= 4){
		if(strcmp(operand[2], "+") == 0){
			println("%d\n", atoi(operand[1]) + atoi(operand[3]));
		}else if(strcmp(operand[2], "-") == 0){
			println("%d\n", atoi(operand[1]) - atoi(operand[3]));
		}else if(strcmp(operand[2], "*") == 0){
			println("%d\n", atoi(operand[1]) * atoi(operand[3]));
		}else if(strcmp(operand[2], "/") == 0){
			println("%d\n", atoi(operand[1]) / atoi(operand[3]));
		}else if(strcmp(operand[2], "%") == 0){
			println("%d\n", atoi(operand[1]) % atoi(operand[3]));
		}else{
			fprintf(stderr, "hatake: expr: %s: operator not found", operand[2]);
			return -1;
		}
	}else{
		perror("hatake: expr: operand is not enough");
	}
	return 0;
}