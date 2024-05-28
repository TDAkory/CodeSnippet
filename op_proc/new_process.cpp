//
// Created by zhaojieyi on 2024/5/28.
//

#include <pthread.h>
#include <stdio.h>
#include <cstdlib>

int sum;
void *runner(void *param);

int main(int argc, char *argv[]) {
    printf("this is new process");
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);
    printf("the sum is %d\n", sum);
    return 0;
}

void *runner(void *param) {
    int i, upper = atoi((char *)param);
    sum = 0;
    for (i = 0; i < upper; i++) {
        sum += i;
    }
    pthread_exit(0);
}