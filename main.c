#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SHIFTS 0xff
#define TITLE "RelaFind by Krusher - Licensed under GPLv3\n"

static unsigned char* haystack;
static bool silent = false;
static bool count = false;

int main(const int argc, const char *argv[]) {


    if (argc < 3) {
        printf(TITLE);
        printf("Usage: %s <file> <needle> [flags]\n", argv[0]);
        printf("Flags go all in the same word and can be:\n");
        printf("s: Silent mode: display only occurrences.\n");
        printf("c: Count mode: display only the number of occurrences.\n");
        return 1;
    }

    const char* filename = argv[1];
    const char* needle = argv[2];
    if (argc > 3) {
        const char* flags = argv[3];
        const size_t flags_len = strlen(flags);
        for (int i = 0; i < flags_len; i++) {
            switch (flags[i]) {
                case 's':
                    silent = true;
                break;
                case 'c':
                    count = true;
                break;
                default:
                    printf("Unknown flag: %c\n", flags[i]);
                    return 1;
            }
        }
    }

    if (!silent && !count) {
        printf(TITLE);
    }

    long size = 0;
    unsigned char* file = read_file(filename, &size);

    if (needle[0] == '\0') {
        printf("Empty needle\n");
        return 1;
    }
    if (size == 0 || size == -1) {
        printf("File is empty or could not be read\n");
        return 1;
    }


    if (!silent && !count) {
        printf("File: %s, size: %ld, needle: %s\n", filename, size, needle);
    }

    const size_t needle_len = strlen(needle);
    haystack = malloc(needle_len);

    int coincidences = 0;
    for (long i = 0; i < size; i++) {
        const int offset = detect(file, i, needle);
        if (offset != -1) {
            coincidences++;
            if (!count) {
                printf("\"%s\" Found at \"%s\", position: %ld (0x%lx) Offset was: %d\n", needle, filename, i, i, offset);
            }
        }
    }

    free(haystack);
    free(file);

    if (!silent) {
        printf("Coincidences: %d\n", coincidences);
    }

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

