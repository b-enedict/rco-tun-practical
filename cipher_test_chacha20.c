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
    unsigned char iv[16] = {0};

    printf("Starting ChaCha20 Encryption with iv: %s\n", iv);
    printf("Key: %s\n", key);

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