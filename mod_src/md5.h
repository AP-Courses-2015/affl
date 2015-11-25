#ifndef MD5_H_INCLUDED
#define MD5_H_INCLUDED

#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>

#define MD5_HASH_SIZE 32

int makeHash(const char *path_to_file, char *hash_result);

#endif // MD5_H_INCLUDED
