#ifndef CHACHA20_H
#define CHACHA20_H

int chacha20_send(unsigned char *plaintext, int plen, unsigned char *ciphertext, unsigned char *key, unsigned char *salt, unsigned int *counter);

int chacha20_receive(unsigned char *ciphertext, int plen, unsigned char *plaintext, unsigned char *key, unsigned char *salt);

#endif