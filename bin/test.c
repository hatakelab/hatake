#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	if(operand[1] && operand[2] && operand[3]){
		if(strcmp(operand[2], "-eq") == 0){
			printf("%d\n", atoi(operand[1]) == atoi(operand[3]));
		}else if(strcmp(operand[2], "-ne") == 0){
			printf("%d\n", atoi(operand[1]) != atoi(operand[3]));
		}else if(strcmp(operand[2], "-lt") == 0){
			printf("%d\n", atoi(operand[1]) < atoi(operand[3]));
		}else if(strcmp(operand[2], "-le") == 0){
			printf("%d\n", atoi(operand[1]) <= atoi(operand[3]));
		}else if(strcmp(operand[2], "-gt") == 0){
			printf("%d\n", atoi(operand[1]) > atoi(operand[3]));
		}else if(strcmp(operand[2], "-ge") == 0){
			printf("%d\n", atoi(operand[1]) >= atoi(operand[3]));
		}else{
			fprintf(stderr, "hatake: test: %s: operator not found\n", operand[1]);
			return;
		}
	}else{
		perror("hatake: test: missing operand");
	}
}
