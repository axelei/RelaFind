#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define SHIFTS 0xff

static unsigned char* haystack;

int main(const int argc, const char *argv[]) {

    printf("RelaFind by Krusher - Licensed under GPLv3\n");

    if (argc < 3) {
        printf("Usage: %s <file> <needle>\n", argv[0]);
        return 1;
    }

    const char* needle = argv[2];
    long size = 0;
    unsigned char* file = read_file(argv[1], &size);

    if (needle[0] == '\0') {
        printf("Empty needle\n");
        return 1;
    }
    if (size == 0 || size == -1) {
        printf("File is empty or could not be read\n");
        return 1;
    }


    printf("File: %s, size: %ld, needle: %s\n", argv[1], size, needle);

    const size_t needle_len = strlen(needle);
    haystack = malloc(needle_len);

    int coincidences = 0;
    for (long i = 0; i < size; i++) {
        const int offset = detect(file, i, needle);
        if (offset != -1) {
            coincidences++;
            printf("Found at position: %ld (0x%lx) Offset was: %d\n", i, i, offset);
        }
    }

    free(haystack);
    free(file);

    printf("Coincidences: %d\n", coincidences);

    return 0;
}

unsigned char * read_file(const char *filename, long *size) {
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char *buffer = malloc(*size);
    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

int detect(const unsigned char *file, const long position, const char *needle) {

    const size_t needle_len = strlen(needle);

    if (needle_len > position) {
        return -1;
    }

    memcpy(haystack, file + position, needle_len);

    for (int i = 0; i < SHIFTS; i++) {
        if (strncmp(needle, haystack, needle_len) == 0) {
            return i;
        }
       shift_haystack(haystack, needle_len);
    }

    return -1;
}

void shift_haystack(unsigned char haystack[], const size_t size) {
    for (int i = 0; i < size; i++) {
        haystack[i]++;
    }
}

