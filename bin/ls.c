#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
    WIN32_FIND_DATA fd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char path[MAX_PATH];

    if(operand[1]){
        sprintf(path, "%s\\*", operand[1]);
    }else{
        strcpy(path, "*");
    }
    if((hFind = FindFirstHandle(path, &findData)) == INVALID_HANDLE_VALUE){
        fprintf(stderr, "hatake: ls: %s: directory not find\n", path);
        return 1;
    }

    do{
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            printf("%s\\", findData.cFileName);
        }else{
            puts(findData.cFileName);
        }
    }while(FindNextFile(hFind, &findData));

    FindClose(hFind);
    return 0;
}
