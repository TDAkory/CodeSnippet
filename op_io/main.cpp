//
// Created by zhaojieyi on 24-6-13.
//

#include <sys/uio.h>
#include <cstdio>
#include <cstring>

int main() {
    {
        // example of writev
        char *str0 = "hello ";
        char *str1 = "world\n";
        ssize_t nwritten;
        struct iovec iov[2];

        iov[0].iov_base = str0;
        iov[0].iov_len = strlen(str0);
        iov[1].iov_base = str1;
        iov[1].iov_len = strlen(str1);

        nwritten = writev(0, iov, 2);
    }
    return 0;
}