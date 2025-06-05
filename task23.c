#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// Forward declaration of recursive_function
void recursive_function(int depth, volatile int* dummy);

void test_file_descriptors() {
    printf("\n=== Testing File Descriptor Limits ===\n");
    
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    printf("Current soft limit for file descriptors: %lld\n", (long long)rlim.rlim_cur);
    printf("Current hard limit for file descriptors: %lld\n", (long long)rlim.rlim_max);
    
    // Try to open maximum number of files
    int fd;
    int count = 0;
    while ((fd = open("/dev/null", O_RDONLY)) >= 0) {
        count++;
    }
    
    printf("Successfully opened %d file descriptors before hitting limit\n", count);
    printf("Error when trying to exceed limit: %s\n", strerror(errno));
    
    // Clean up
    for (int i = 3; i < count + 3; i++) {
        close(i);
    }
}

void test_cpu_time() {
    printf("\n=== Testing CPU Time Limits ===\n");
    
    struct rlimit rlim;
    getrlimit(RLIMIT_CPU, &rlim);
    printf("Current soft CPU time limit (seconds): %lld\n", (long long)rlim.rlim_cur);
    printf("Current hard CPU time limit (seconds): %lld\n", (long long)rlim.rlim_max);
    
    // Set a low CPU time limit for demonstration
    rlim.rlim_cur = 1;
    setrlimit(RLIMIT_CPU, &rlim);
    
    printf("Set CPU time limit to 1 second. Starting infinite loop...\n");
    while(1) {
        // This infinite loop should be terminated by the CPU time limit
    }
}

void test_memory_limits() {
    printf("\n=== Testing Memory Limits ===\n");
    
    struct rlimit rlim;
    getrlimit(RLIMIT_AS, &rlim);
    printf("Current soft address space limit (bytes): %lld\n", (long long)rlim.rlim_cur);
    printf("Current hard address space limit (bytes): %lld\n", (long long)rlim.rlim_max);
    
    // Try to allocate memory until we hit the limit
    size_t alloc_size = 1024 * 1024; // 1MB chunks
    void* ptr;
    size_t total_allocated = 0;
    
    while ((ptr = malloc(alloc_size)) != NULL) {
        total_allocated += alloc_size;
        // Touch each page to actually commit the memory
        memset(ptr, 0, alloc_size);
    }
    
    printf("Allocated %zu bytes before hitting memory limit\n", total_allocated);
    printf("Error when trying to exceed limit: %s\n", strerror(errno));
    
    // Note: We don't free the memory to demonstrate the limit
}

void test_stack_size() {
    printf("\n=== Testing Stack Size Limits ===\n");
    
    struct rlimit rlim;
    getrlimit(RLIMIT_STACK, &rlim);
    printf("Current soft stack size limit (bytes): %lld\n", (long long)rlim.rlim_cur);
    printf("Current hard stack size limit (bytes): %lld\n", (long long)rlim.rlim_max);
    
    // Recursive function to test stack limits
    printf("Testing stack limit with recursive function...\n");
    volatile int dummy; // To prevent tail recursion optimization
    recursive_function(0, &dummy);
}

void recursive_function(int depth, volatile int* dummy) {
    char buffer[1024]; // Allocate 1KB on stack each call
    *dummy = buffer[0]; // Use buffer to prevent optimization
    if (depth % 1000 == 0) {
        printf("Current recursion depth: %d\n", depth);
    }
    recursive_function(depth + 1, dummy);
}

void print_menu() {
    printf("\nResource Limits Test Program\n");
    printf("1. Test File Descriptor Limits\n");
    printf("2. Test CPU Time Limits\n");
    printf("3. Test Memory Limits\n");
    printf("4. Test Stack Size Limits\n");
    printf("5. Run All Tests\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    printf("Resource Limitation Methods Test Program\n");
    printf("This program demonstrates various resource limitation techniques in FreeBSD/Linux\n");
    
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                test_file_descriptors();
                break;
            case 2:
                test_cpu_time();
                break;
            case 3:
                test_memory_limits();
                break;
            case 4:
                test_stack_size();
                break;
            case 5:
                test_file_descriptors();
                test_cpu_time();
                test_memory_limits();
                test_stack_size();
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    
    return 0;
}
