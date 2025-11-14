int cesar_encrypt(char *plaintext, int plen, char *ciphertext) {
    for(int i = 0; i < plen; i++) {
        ciphertext[i] = (plaintext[i] + 62) % 256;
    }
    return 0;
}

int cesar_decrypt(char *ciphertext, int plen, char *plaintext) {
    for(int i = 0; i < plen; i++) {
        plaintext[i] = (ciphertext[i] + 256 - 62) % 256;
    }
    return 0;
}