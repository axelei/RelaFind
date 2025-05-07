//
// Created by axele on 07/05/2025.
//

#ifndef MAIN_H
#define MAIN_H
#include <stddef.h>

unsigned char* read_file(const char * filename, long* size);
int detect(const unsigned char * file, long position, const char * needle);
void shift_haystack(unsigned char haystack[], size_t size);

#endif //MAIN_H
