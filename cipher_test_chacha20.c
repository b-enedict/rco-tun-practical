#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chacha20/chacha20.h"

#define BUFSIZE 200

int main() {
    unsigned char *buffer = malloc(BUFSIZE);

    // ===== Key Reading =====
    unsigned char key[32];
    FILE *key_file = fopen("shared_secret.key", "rb");
    fread((char *) key, 1, 32, key_file);
    fclose(key_file);

    // ===== IV Definition =====
    unsigned char nonce[12] = {0x42}; // This MUST be generated random in a production environment to be secure
    int block_counter = 0;
    unsigned char iv[16] = {0};

    printf("Starting ChaCha20 Encryption with iv: %s\n", iv);
    printf("Key: %s\n", key);
    printf("Nonce: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", nonce[0], nonce[1], nonce[2], nonce[3], nonce[4], nonce[5], nonce[6], nonce[7], nonce[8], nonce[9], nonce[10], nonce[11]);
    printf("Block Counter: %d\n", block_counter);

    while(1) {
        printf("Enter your text (or press Enter to quit): ");

        if (!fgets((char *) buffer, BUFSIZE, stdin)) {
            // EOF or error
            break;
        }

        // If user just presses Enter, quit
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }
        
        int plen = (int) strlen((char *) buffer);
        unsigned char *temp = malloc(plen + 1);
        
        // Update IV
        block_counter++;
        memcpy(iv, &block_counter, 4);
        memcpy(iv + 4, nonce, 12);

        // Encryption Logic
        printf("\nEncrypting a packet of length: %d", plen);
        chacha20_encrypt(buffer, plen, temp, key, iv);
        temp[plen] = '\0';

        // Output the encrypted packet
        printf(", Encrypted packet:\n\n%s\n", temp);

        // Decryption Logic
        chacha20_encrypt(temp, plen, buffer, key, iv);
        buffer[plen] = '\0';

        printf("Decrypted packet:\n\n%s\n\n", buffer);
        free(temp);
    }
    cleanup:
    free(buffer);
    return 0;
}