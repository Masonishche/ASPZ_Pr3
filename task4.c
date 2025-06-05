#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void generate_numbers(int count, int max) {
    int used[50] = {0};
    for (int i = 0; i < count; ) {
        int num = rand() % max + 1;
        if (!used[num]) {
            used[num] = 1;
            printf("%d ", num);
            i++;
        }
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    while (1) {
        generate_numbers(7, 49);
        generate_numbers(6, 36);
        sleep(1); 
    }
    return 0;
}
