//
// Created by zh33rmao on 11/11/22.
//
#include "csapp.h"
#define MAXARGS 128

/* Funtion prototype*/
void eval(char* cmdline);
int parseline(char* buf, char **argv);
int builtin_command(char **argv);

int main(int argc, char **argv){
    char cmdline[MAXARGS];

    while(1){
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if(feof(stdin)){
            exit(0);
            eval(cmdline);
        }
    }
}
