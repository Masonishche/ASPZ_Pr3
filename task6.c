#include <stdio.h>

void recurse(int depth) {
    char buffer[1024];  // займає місце у стеку
    printf("Recursion depth: %d\n", depth);
    recurse(depth + 1);
}

int main() {
    recurse(1);
    return 0;
}
