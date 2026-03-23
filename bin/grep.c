#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
	char line[1024];
	if(!operand[1]){
		perror("hatake: grep: operand is not enough");
		return -1;
	}
	while(fgets(line, sizeof(line), stdin)){
		for(int i = 0; i < strlen(line) - strlen(operand[1]); i ++){
			int is_cmp = 1;
			for(int j = 0; j < strlen(operand[1]); j ++){
				if(line[i+j] != operand[1][j]){
					is_cmp = 0;
					break;
				}
			}
			if(is_cmp){
				printf("%s", line);
			}
		}
	}
}
