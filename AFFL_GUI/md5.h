#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED

#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int makeHash(const char *path_to_file, char *hash_result);

#endif // MD5_H_INCLUDED
