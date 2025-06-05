#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>

void test_file_limits() {
    printf("\n[1] Testing file descriptor limits...\n");
    
    struct rlimit limits;
    getrlimit(RLIMIT_NOFILE, &limits);
    printf("Current limits: %lld (soft), %lld (hard)\n", 
          (long long)limits.rlim_cur, (long long)limits.rlim_max);
    
    int fd, count = 0;
    while ((fd = open("/dev/null", O_RDONLY)) >= 0) {
        count++;
    }
    printf("Opened %d files before error: %s\n", count, strerror(errno));
    
    for (int i = 3; i < count + 3; i++) close(i);
}

void test_memory_limits() {
    printf("\n[2] Testing memory allocation limits...\n");
    
    struct rlimit limits;
    getrlimit(RLIMIT_DATA, &limits);
    printf("Current limits: %lld (soft), %lld (hard)\n",
          (long long)limits.rlim_cur, (long long)limits.rlim_max);
    
    size_t total = 0, block = 1024*1024; // 1MB blocks
    while (malloc(block)) {
        total += block;
        if (total % (100*1024*1024) == 0) {
            printf("Allocated %zu MB\n", total/(1024*1024));
        }
    }
    printf("Failed after allocating %zu MB: %s\n", total/(1024*1024), strerror(errno));
}

void simple_recursion(int depth) {
    char buffer[1024]; // Use 1KB stack per call
    if (depth % 1000 == 0) printf("Depth: %d\n", depth);
    simple_recursion(depth + 1);
}

void test_stack_limits() {
    printf("\n[3] Testing stack size limits...\n");
    
    struct rlimit limits;
    getrlimit(RLIMIT_STACK, &limits);
    printf("Current limits: %lld (soft), %lld (hard)\n",
          (long long)limits.rlim_cur, (long long)limits.rlim_max);
    
    printf("Starting recursive function...\n");
    simple_recursion(0);
}

int main() {
    printf("Resource Limits Tester (Simplified for FreeBSD)\n");
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Test file descriptor limits\n");
        printf("2. Test memory allocation limits\n");
        printf("3. Test stack size limits\n");
        printf("0. Exit\n");
        printf("Choose test: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: test_file_limits(); break;
            case 2: test_memory_limits(); break;
            case 3: test_stack_limits(); break;
            case 0: return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
