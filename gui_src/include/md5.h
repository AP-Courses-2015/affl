#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED

#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define MD5_HASH_SIZE 32

int makeHash(const char *path_to_file, char *hash_result);

#endif // MD5_H_INCLUDED
