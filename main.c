#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_OPERAND_LEN 64

void run(char *op, char **operand)
{
	// built-in cmd
	if(!op)
		return;
	else if(strcmp(op, "exit") == 0)
		exit(0);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char cmd[MAX_PATH + MAX_CMD_LEN];

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	GetModuleFileName(NULL, cmd, MAX_PATH);
	for(int i = strlen(cmd)-1; i >= 0; i --){
		if(cmd[i] == '\\'){
			cmd[i+1] = '\0';
			break;
		}
	}
	strcat(cmd, "bin\\");
	strcat(cmd, op);
	strcat(cmd, ".exe ");
	for(int i = 0; operand[i] != NULL; i ++){
		strcat(cmd, operand[i]);
		strcat(cmd, " ");
	}

	if(!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
		fprintf(stderr, "error: failed %s\n", op);
		exit(-1);
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
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
		run(op, operand);
	}

	return 0;
}
