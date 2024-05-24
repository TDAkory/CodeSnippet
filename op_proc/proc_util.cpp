//
// Created by zhaojieyi on 2023/3/30.
//

#include "proc_util.h"
#include <fcntl.h>
#include <string.h>
#include <string>

size_t physical_memory_used_by_process() {
    FILE *file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    if (!file) {
        return -1;
    }

    while (fgets(line, 128, file) != nullptr) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            size_t len = strlen(line);

            const char *p = line;
            for (; std::isdigit(*p) == false; ++p) {
            }

            line[len - 3] = 0;
            result = atoi(p);

            break;
        }
    }
    fclose(file);
    return static_cast<size_t>(result);
}