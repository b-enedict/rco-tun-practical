#ifndef CHACHA20_H
#define CHACHA20_H

int chacha20_encrypt(unsigned char *plaintext, int plen, unsigned char *ciphertext, unsigned char *key, unsigned char *iv);

#endif