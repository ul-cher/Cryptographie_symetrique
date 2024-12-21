#ifndef CRYPTAGE
#define CRYPTAGE
#include <stdlib.h>

extern void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len);

#endif