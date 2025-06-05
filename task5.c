#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        printf("Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    FILE *dst = fopen(argv[2], "w");
    if (!dst) {
        printf("Cannot open file %s for writing\n", argv[2]);
        fclose(src);
        return 1;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes, dst) < bytes) {
            perror("Write error (possibly file size limit reached)");
            break;
        }
    }

    fclose(src);
    fclose(dst);
    return 0;
}
