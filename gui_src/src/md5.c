#include "md5.h"

#define MD5_BLOCK_SIZE 64

static int T[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static int S[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

static int f_id;
static uint64_t len = 0;

static inline void le2be(void *src, void *dst)
{
    int i;
    char *d = (char *)dst;
    char *s = (char *)src;

    for(d += i = sizeof(uint32_t) ; i--;) *--d = *s++;
}

static void resultToString(uint32_t A, uint32_t B, uint32_t C, uint32_t D, char *res)
{
    int i;
    uint32_t be_A, be_B, be_C, be_D;
    le2be(&A, &be_A); le2be(&B, &be_B); le2be(&C, &be_C); le2be(&D, &be_D);
    sprintf(res, "%8x%8x%8x%8x", be_A, be_B, be_C, be_D);
    for (i = 0; i<MD5_HASH_SIZE; i++) if (res[i] == ' ') res[i] = '0';
}

static inline int getNextBlock(char *block)
{
    int res;

    if ((res = read(f_id, block, MD5_BLOCK_SIZE)) < MD5_BLOCK_SIZE)
    {
        if (res == -1)  return -1;
        len += res;
        len *= 8;

        block[res++] = 0x80;
        for (; res<56; block[res++] = 0);

        char *plen = (char *)&len;
        for (; res<64; block[res++] = *plen++);

        return 0;
    }

    len += res;

    return res;
}

static inline uint32_t funcF(uint32_t X, uint32_t Y, uint32_t Z) {return (X&Y)|(~X&Z);}

static inline uint32_t funcG(uint32_t X, uint32_t Y, uint32_t Z) {return (X&Z)|(~Z&Y);}

static inline uint32_t funcH(uint32_t X, uint32_t Y, uint32_t Z) {return X^Y^Z;}

static inline uint32_t funcI(uint32_t X, uint32_t Y, uint32_t Z) {return Y^(~Z|X);}

int makeHash(const char *path_to_file, char *hash_result)
{
    uint32_t A = 0x67452301,
             B = 0xefcdab89,
             C = 0x98badcfe,
             D = 0x10325476;
    char block[MD5_BLOCK_SIZE];
    int bytes_read;
    uint32_t *X;
    uint32_t tmp_A, tmp_B, tmp_C, tmp_D;
    uint32_t R;
    int j;
    len = 0;

    if ((f_id = open(path_to_file, O_RDONLY)) <= 0)
        return -1;

    do
    {
        bytes_read = getNextBlock(block);
        if (bytes_read == -1)
            return -1;

        X = (uint32_t *)block;

        tmp_A = A; tmp_B = B; tmp_C = C; tmp_D = D;

        int i;
        for (i = 0; i<64; i++)
        {
            if (i<16)
            {
               R = funcF(B, C, D);
               j = i;
            }
            else if (i<32)
            {
                R = funcG(B, C, D);
                j = (5*i + 1) % 16;
            }
            else if (i<48)
            {
                R = funcH(B, C, D);
                j = (3*i + 5) % 16;
            }
            else
            {
                R = funcI(B, C, D);
                j = (7*i) % 16;
            }
            uint32_t stor = D;
            D = C; C = B;
            uint32_t tmp = A + R + T[i] + X[j];
            B = B + ((tmp << S[i]) | (tmp >> (32-S[i])));
            A = stor;
        }

        A += tmp_A; B += tmp_B; C += tmp_C; D += tmp_D;
    } while (bytes_read == MD5_BLOCK_SIZE);

    resultToString(A, B, C, D, hash_result);
    close(f_id);

    return 0;
}
