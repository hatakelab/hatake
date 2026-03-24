#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
	if(!operand[1]){
		perror("hatake: md: missing operand");
		return -1;
	}else if(!CreateDirectory(operand[1], NULL)){
		fprintf(stderr, "hatake: md: %s: cannot make directory\n", operand[1]);
	}
	return 0;
}
