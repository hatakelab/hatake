#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_OPERAND_LEN 64

void run(char *op, char **operand)
{
	if(strcmp(op, ":") == 0){
		printf("");
	}else if(strcmp(op, "echo") == 0){
		for(int i = 0; operand[i] != NULL; i ++)
		{
			printf("%s", operand[i]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	char cmd[MAX_CMD_LEN];
	char *op;
	char *operand[MAX_OPERAND_LEN];

	if(argc > 1){
		if(argc >= MAX_OPERAND_LEN){
			fprintf(stderr, "error: intpu too long operator\n");
			return -1;
		}
		op = argv[1];
		int i;
		for(i = 2; i < argc; i ++){
			operand[i - 2] = argv[i];
		}
		operand[i - 2] = NULL;
		run(op, operand);
		return 0;
	}

	while(1){
		// input cmd
		printf("hatake $ ");
		if(fgets(cmd, sizeof(cmd), stdin) == NULL){
			fprintf(stderr, "error: input too long cmd\n");
			continue;
		}

		// divide cmd
		op = strtok(cmd, " \t\n\r");
		int i;
		for(i = 0; 1; i ++){
			if(i >= MAX_OPERAND_LEN){
				fprintf(stderr, "error: input too long arg\n");
				continue;
			}
			if((operand[i] = strtok(NULL, " \t\n\r")) == NULL){
				break;
			}
		}

		// run cmd
		if(!op)
			continue
		else if(strcmp(op, "exit") == 0)
			break;
		run(op, operand);
	}

	return 0;
}
