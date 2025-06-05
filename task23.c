#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>

void check_file_size_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_FSIZE, &limit);
    std::cout << "File size limit: " << limit.rlim_cur << " bytes\n";

    std::ofstream file("large_output.txt");
    try {
        for (int i = 0; i < 1000000; ++i) {
            file << "This is a test line to exceed file size limit.\n";
        }
    } catch (...) {
        std::cerr << "File size limit exceeded\n";
    }
    file.close();
}

void check_cpu_time_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_CPU, &limit);
    std::cout << "CPU time limit: " << limit.rlim_cur << " seconds\n";

    while (true) {
        volatile double x = 0;
        for (int i = 0; i < 1000000; ++i) {
            x += i * 0.5;
        }
    }
}

void check_memory_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_AS, &limit);
    std::cout << "Virtual memory limit: " << limit.rlim_cur << " bytes\n";

    try {
        std::vector<char*> allocations;
        while (true) {
            char* block = new char[1024 * 1024];
            memset(block, 0, 1024 * 1024);
            allocations.push_back(block);
        }
    } catch (...) {
        std::cerr << "Memory allocation failed or limit reached\n";
    }
}

void check_stack_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_STACK, &limit);
    std::cout << "Stack size limit: " << limit.rlim_cur << " bytes\n";

    std::function<void(int)> recurse = [&](int depth) {
        char buffer[1024];
        memset(buffer, depth, sizeof(buffer));
        recurse(depth + 1);
    };

    try {
        recurse(1);
    } catch (...) {
        std::cerr << "Stack overflow or limit reached\n";
    }
}

void check_open_files_limit() {
    struct rlimit limit;
    getrlimit(RLIMIT_NOFILE, &limit);
    std::cout << "Open files limit: " << limit.rlim_cur << "\n";

    std::vector<std::ofstream> files;
    try {
        for (int i = 0; i < limit.rlim_cur + 10; ++i) {
            std::ofstream f("temp_file_" + std::to_string(i) + ".txt");
            files.push_back(std::move(f));
        }
    } catch (...) {
        std::cerr << "Too many open files\n";
    }
}

int main() {
    std::cout << "Testing resource limits...\n";

    pid_t pid = fork();
    if (pid == 0) {
        std::cout << "\n--- CPU Time Limit Test ---\n";
        check_cpu_time_limit();
        exit(0);
    }

    pid = fork();
    if (pid == 0) {
        std::cout << "\n--- File Size Limit Test ---\n";
        check_file_size_limit();
        exit(0);
    }

    pid = fork();
    if (pid == 0) {
        std::cout << "\n--- Memory Limit Test ---\n";
        check_memory_limit();
        exit(0);
    }

    pid = fork();
    if (pid == 0) {
        std::cout << "\n--- Stack Size Limit Test ---\n";
        check_stack_limit();
        exit(0);
    }

    pid = fork();
    if (pid == 0) {
        std::cout << "\n--- Open Files Limit Test ---\n";
        check_open_files_limit();
        exit(0);
    }

    return 0;
}
