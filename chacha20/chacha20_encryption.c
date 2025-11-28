#include <openssl/evp.h>
#include <string.h>
#define COUNTER_BYTES 4

int chacha20_encrypt(unsigned char *in_text, int plen, unsigned char *out_text, unsigned char *key, unsigned char *iv) {
    int outl;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, out_text, &outl, in_text, plen);
    EVP_EncryptFinal_ex(ctx, out_text + plen, &outl);
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);
    return outl;
}

int chacha20_send(unsigned char *plaintext, int plen, unsigned char *ciphertext, unsigned char *key, unsigned char *salt, unsigned int *counter) {
    unsigned char iv[16];
    memcpy(iv, counter, 4);
    memcpy(iv + COUNTER_BYTES, salt, 12);

    int outl = chacha20_encrypt(plaintext, plen, ciphertext + COUNTER_BYTES, key, iv);
    
    memcpy(ciphertext, counter, COUNTER_BYTES);
    
    (*counter)++;
    return outl + COUNTER_BYTES;
};

int chacha20_receive(unsigned char *ciphertext, int plen, unsigned char *plaintext, unsigned char *key, unsigned char *salt) {
    unsigned char iv[16];
    memcpy(iv, ciphertext, COUNTER_BYTES);
    memcpy(iv + COUNTER_BYTES, salt, 12);

    int outl = chacha20_encrypt(ciphertext + COUNTER_BYTES, plen - COUNTER_BYTES, plaintext, key, iv);
    return outl;
};