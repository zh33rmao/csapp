//
// Created by zh33rmao on 11/11/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int snooze(unsigned int secs);

unsigned int snooze(unsigned int secs){
    unsigned int rc = sleep(secs);
    printf("the program slept %d of %d secs.\n", secs -rc, secs);
    return rc;

}

int main (int argc, char** argv){
    unsigned int secs = atoi(argv[1]);
    snooze(secs);
    return 0;
}