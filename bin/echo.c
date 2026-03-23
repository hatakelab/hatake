#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
    if(argc > 1){
        for(int i = 1; i < argc; i ++){
            printf("%s", operand[i]);
        }
        puts("");
    }else{
        char line[1024];
        while(gets(line)){
            puts(line);
        }
    }
}
