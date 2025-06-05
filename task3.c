#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *f = fopen("dice.txt", "w");
    if (!f) {
        perror("Cannot open file");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < 1000000; i++) {
        int roll = rand() % 6 + 1;
        if (fprintf(f, "Roll %d: %d\n", i + 1, roll) < 0) {
            perror("Write error (possibly file size limit reached)");
            break;
        }
    }

    fclose(f);
    return 0;
}
