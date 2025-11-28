#include <openssl/evp.h>
#include <string.h>
#define COUNTER_BYTES 4

int chacha20_encrypt(unsigned char *in_text, int plen, unsigned char *out_text, unsigned char *key, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, out_text, &plen, in_text, plen);
    EVP_EncryptFinal_ex(ctx, out_text + plen, &plen);
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);
}

int chacha20_send(unsigned char *plaintext, int plen, unsigned char *ciphertext, unsigned char *key, unsigned char *salt, unsigned int *counter) {
    unsigned char iv[16];
    memcpy(iv, counter, 4);
    memcpy(iv + COUNTER_BYTES, salt, 12);
    fprintf(stderr, "IV bytes: %02x %02x %02x %02x\n", iv[0], iv[1], iv[2], iv[3]);

    chacha20_encrypt(plaintext, plen, ciphertext + COUNTER_BYTES, key, iv);

    memcpy(ciphertext, counter, COUNTER_BYTES);

    (*counter)++;
    return plen + COUNTER_BYTES;
};

int chacha20_receive(unsigned char *ciphertext, int plen, unsigned char *plaintext, unsigned char *key, unsigned char *salt) {
    unsigned char iv[16];
    memcpy(iv, ciphertext, COUNTER_BYTES);
    memcpy(iv + COUNTER_BYTES, salt, 12);

    fprintf(stderr, "IV bytes: %02x %02x %02x %02x\n", iv[0], iv[1], iv[2], iv[3]);

    chacha20_encrypt(ciphertext + COUNTER_BYTES, plen - COUNTER_BYTES, plaintext, key, iv);
    return plen - COUNTER_BYTES;
};