#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chacha20/chacha20.h"

#define BUFSIZE 200
#define COUNTER_BYTES 4

int main() {
    unsigned char buffer[BUFSIZE];
    unsigned char cipherbuffer[BUFSIZE + COUNTER_BYTES];

    // ===== Key Reading =====
    unsigned char key[32];
    FILE *key_file = fopen("shared_secret.key", "rb");
    fread((char *) key, 1, 32, key_file);
    fclose(key_file);

    // ===== IV Definition =====
    unsigned char salt[12] = {0x42}; // This MUST be generated random in a production environment to be secure
    unsigned int block_counter = 0;

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

        // Encryption Logic
        printf("\nEncrypting a packet of length: %d, block count: %d", plen, block_counter);
        chacha20_send(buffer, plen, cipherbuffer, key, salt, &block_counter);

        // Output the encrypted packet
        printf(", Encrypted packet:\n\n%02x\n", cipherbuffer[0]);

        // Decryption Logic
        chacha20_receive(cipherbuffer, plen, buffer, key, salt);
        buffer[plen] = '\0';

        printf("Decrypted packet:\n\n%s\n\n", buffer);
    }
    return 0;
}