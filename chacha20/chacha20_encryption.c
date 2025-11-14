#include <openssl/evp.h>

int chacha20_encrypt(unsigned char *plaintext, int plen, unsigned char *ciphertext, unsigned char *key, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &plen, plaintext, plen);
    EVP_EncryptFinal_ex(ctx, ciphertext + plen, &plen);
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}