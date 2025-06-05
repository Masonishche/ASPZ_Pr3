#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void test_file_size_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_FSIZE, &limit);
    printf("File size limit: %ld bytes\n", (long)limit.rlim_cur);

    int fd = open("large_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return;
    }

    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    for (int i = 0; i < 1000000; ++i) {
        ssize_t written = write(fd, buffer, sizeof(buffer));
        if (written < 0) {
            perror("write");
            break;
        }
    }

    close(fd);
}

void test_cpu_time_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_CPU, &limit);
    printf("CPU time limit: %ld seconds\n", (long)limit.rlim_cur);

    while (1) {
        volatile double x = 0;
        for (int i = 0; i < 1000000; ++i) {
            x += i * 0.5;
        }
    }
}

void test_memory_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_AS, &limit);
    printf("Virtual memory limit: %ld bytes\n", (long)limit.rlim_cur);

    while (1) {
        char *block = malloc(1024 * 1024);
        if (!block) {
            perror("malloc");
            break;
        }
        memset(block, 0, 1024 * 1024);
    }
}

void recursive_function(int depth) {
    char buffer[1024];
    memset(buffer, depth, sizeof(buffer));
    recursive_function(depth + 1);
}

void test_stack_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_STACK, &limit);
    printf("Stack size limit: %ld bytes\n", (long)limit.rlim_cur);

    recursive_function(1);
}

void test_open_files_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_NOFILE, &limit);
    printf("Open files limit: %ld\n", (long)limit.rlim_cur);

    for (int i = 0; i < limit.rlim_cur + 10; ++i) {
        char name[64];
        snprintf(name, sizeof(name), "temp_file_%d.txt", i);
        int fd = open(name, O_CREAT | O_WRONLY, 0644);
        if (fd < 0) {
            perror("open");
            break;
        }
    }
}

void run_test(void (*func)(), const char *name) {
    pid_t pid = fork();
    if (pid == 0) {
        printf("\n--- %s ---\n", name);
        func();
        exit(0);
    }
}

int main() {
    printf("Testing resource limits...\n");

    run_test(test_cpu_time_limit, "CPU Time Limit Test");
    run_test(test_file_size_limit, "File Size Limit Test");
    run_test(test_memory_limit, "Memory Limit Test");
    run_test(test_stack_limit, "Stack Size Limit Test");
    run_test(test_open_files_limit, "Open Files Limit Test");

    for (int i = 0; i < 5; ++i) wait(NULL);

    return 0;
}
