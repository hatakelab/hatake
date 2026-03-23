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
	}else if(strcmp(op, ":") == 0){
		return;
	}else if(strcmp(op, "cd") == 0){
		if(!SetCurrentDirectory(operand[0] ? operand[0] : getenv("USERPROFILE"))){
			fprintf(stderr, "hatake: cd: %s: folder no find\n", operand[0] ? operand[0] : getenv("USERPROFILE"));
		}
		return;
	}else if(strcmp(op, "echo") == 0){
		for(int i = 0; operand[i]; i ++){
			printf("%s", operand[i]);
		}
		printf("\n");
		return;
	}else if(strcmp(op, "exit") == 0){
		exit(operand[0] ? atoi(operand[0]) : 0);
	}else if(strcmp(op, "pwd") == 0){
		char dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, dir);
		printf("%s\n", dir);
		return;
	}else if(strcmp(op, "test") == 0){
		if(operand[0] && operand[1] && operand[2]){
			if(strcmp(operand[1], "-eq") == 0){
				printf("%d\n", atoi(operand[0]) == atoi(operand[2]));
			}else if(strcmp(operand[1], "-ne") == 0){
				printf("%d\n", atoi(operand[0]) != atoi(operand[2]));
			}else if(strcmp(operand[1], "-lt") == 0){
				printf("%d\n", atoi(operand[0]) < atoi(operand[2]));
			}else if(strcmp(operand[1], "-le") == 0){
				printf("%d\n", atoi(operand[0]) <= atoi(operand[2]));
			}else if(strcmp(operand[1], "-gt") == 0){
				printf("%d\n", atoi(operand[0]) > atoi(operand[2]));
			}else if(strcmp(operand[1], "-ge") == 0){
				printf("%d\n", atoi(operand[0]) >= atoi(operand[2]));
			}else{
				fprintf(stderr, "hatake: test: %s: operator not found\n", operand[1]);
				return;
			}
		}else{
			perror("hatake: test: operand is not enough");
		}
		return;
	}else if(strcmp(op, "type") == 0){
		for(int i=0; operand[i]; i ++){
			if(
				strcmp(operand[i], ":") == 0 ||
				strcmp(operand[i], "cd") == 0 ||
				strcmp(operand[i], "echo") == 0 ||
				strcmp(operand[i], "exit") == 0 ||
				strcmp(operand[i], "pwd") == 0 ||
				strcmp(operand[i], "test") == 0 ||
				strcmp(operand[i], "type") == 0
			){
				printf("%s is a shell builtin\n", operand[i]);
			}else{
				char path[MAX_PATH];
				GetModuleFileName(NULL, path, MAX_PATH);
				for(int i = strlen(path)-1; i >= 0; i --){
					if(path[i] == '\\'){
						path[i+1] = '\0';
						break;
					}
				}
				strcat(path, "bin\\");
				strcat(path, op);
				strcat(path, ".exe");
				FILE *fp = fopen(path, "r");
				if(fp){
					printf("%s : %s\n", operand[i], path);
				}else{
					printf("%s not found\n", operand[i]);
				}
			}
		}
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char arg[MAX_PATH + MAX_CMD_LEN];
	SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	int is_redirect_input = 0;
	int is_redirect_output = 0;
	int is_redirect_error = 0;
/*
    if (output_file) {
        // 出力ファイルをオープン
        HANDLE hFile = CreateFile(output_file, GENERIC_WRITE, 0, NULL, 
                                  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        si.hStdOutput = hFile;
        si.hStdError = hFile; // エラーもファイルへ
    }

    // CreateProcessで子プロセスを作成
    CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    
    // ハンドルを閉じる
    if (output_file) CloseHandle(si.hStdOutput);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


*/



	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));

	// redirect
	if(operand[0]) for(int i = 0; operand[i+1] != NULL; i ++){
		if(strcmp(operand[i], "-0") == 0){
			si.hStdInput = CreateFile(in_file, GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_input = 1;
			i ++;
		}else if(strcmp(operand[i], "-1") == 0){
			si.hStdOutput = CreateFile(operand[i+1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_output = 1;
			i ++;
		}else if(strcmp(operand[i], "-2") == 0){
			si.hStdError = CreateFile(operand[i+1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			is_redirect_error = 1;
			i ++;
		}
	}

	// set arg
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

	if(!CreateProcess(NULL, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
		fprintf(stderr, "hatake: %s: command not found\n", op);
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
			printf("%s $ ", pwd);
		}else{
			printf(" $ ");
		}
		if(!fgets(cmd, sizeof(cmd), stdin)){
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
	}

	return 0;
}
