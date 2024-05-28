//
// Created by zhaojieyi on 2024/5/28.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid;
    pid = fork();
    if (pid < 0)  // error
    {
        printf("there have something wrong\n");
    }
    if (pid > 0)  // father
    {
        printf("this is main process\n");
    }
    if (pid == 0)  // son
    {
        printf("this is new processs");
        // execl( "/root/fork/2","10",NULL);
        if (execl("new_process", "10", NULL) < 0) {
            perror("execv error ");
        }
    }
    return 0;
}