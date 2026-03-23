#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_LEN 1024
#define MAX_OPERAND_LEN 64

void run(char *op, char **operand)
{
	// built-in cmd
	if(!op){
		return;
	}else if(strcmp(op, "cd") == 0){
		SetCurrentDirectory(operand[0] ? operand[0] : getenv("USERPROFILE"))
		return;
	}else if(strcmp(op, "exit") == 0){
		exit(operand[0] ? atoi(operand[0]) : 0);
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};
	int is_redirect_input = 0;
	int is_redirect_output = 0;
	int is_redirect_error = 0;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));

	// redirect
	if(operand[0]) for(int i = 0; operand[i+1]; i ++){
		if(strcmp(operand[i], "-0") == 0){
			si.hStdInput = CreateFile(operand[i+1], GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_input = 1;
			for(int j = i+2; operand[j]; j ++){
				operand[i] = operand[j]
			}
			continue;
		}else if(strcmp(operand[i], "-1") == 0){
			si.hStdOutput = CreateFile(operand[i+1], GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_output = 1;
			for(int j = i+2; operand[j]; j ++){
				operand[i] = operand[j]
			}
			continue;
		}else if(strcmp(operand[i], "-2") == 0){
			si.hStdError = CreateFile(operand[i+1], GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_error = 1;
			for(int j = i+2; operand[j]; j ++){
				operand[i] = operand[j]
			}
			continue;
		}
	}
	if(!is_redirect_input) si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	if(!is_redirect_output) si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if(!is_redirect_error) si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	// set arg
	char arg[MAX_PATH + MAX_CMD_LEN];
	GetModuleFileName(NULL, arg, MAX_PATH);
	for(int i = strlen(arg)-1; i >= 0; i --){
		if(arg[i] == '\\'){
			arg[i+1] = '\0';
			break;
		}
	}
	strcat(arg, "bin\\");
	strcat(arg, op);
	strcat(arg, ".exe ");
	for(int i = 0; operand[i] != NULL; i ++){
		strcat(arg, operand[i]);
		strcat(arg, " ");
	}

	if(!CreateProcess(NULL, arg, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
		fprintf(stderr, "hatake: %s: command not found\n", op);
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if(is_redirect_input) CloseHandle(si.hStdInput);
	if(is_redirect_output) CloseHandle(si.hStdOutput);
	if(is_redirect_error) CloseHandle(si.hStdError);
}

int main(int argc, char **argv)
{
	char cmd[MAX_CMD_LEN];
	char pwd[MAX_PATH];
	char *op;
	char *operand[MAX_OPERAND_LEN];

	SetCurrentDirectory(getenv("USERPROFILE"));

	if(argc > 1){
		if(argc >= MAX_OPERAND_LEN){
			perror("hatake: fatal: command is failure");
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
		if(GetCurrentDirectory(MAX_PATH, pwd)){
			printf("%s:\n $ ", pwd);
		}else{
			printf(" $ ");
		}
		if(!gets(cmd)){
			perror("hatake: fatal: command is failure");
			continue;
		}

		// divide cmd
		op = strtok(cmd, " \t\n\r");
		int i;
		for(i = 0; 1; i ++){
			if(i >= MAX_OPERAND_LEN){
				perror("hatake: fatal: command is failure");
				continue;
			}
			if((operand[i] = strtok(NULL, " \t\n\r")) == NULL){
				break;
			}
		}

		// run cmd
		run(op, operand);
		puts("");
	}

	return 0;
}
