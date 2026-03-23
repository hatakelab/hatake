#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **operand)
{
    if(!operand[1]){
        perror("hatake: rm: missing operand");
        return -1;
    }else if(remove(operand[1]) != 0){
        fprintf(stderr, "hatake: rm: %s: file not found\n", operand[1]);
        return -1;
    }
    return 0;
}
