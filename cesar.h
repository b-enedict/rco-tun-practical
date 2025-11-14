#ifndef CESAR_H
#define CESAR_H

int cesar_encrypt(char *plaintext, int plen, char *ciphertext);
int cesar_decrypt(char *ciphertext, int plen, char *plaintext);

#endif