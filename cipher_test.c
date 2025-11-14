#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cesar.h"

#define BUFSIZE 200

int main() {
    char *buffer = malloc(BUFSIZE);

    while(1) {
        printf("Enter your text (or press Enter to quit): ");

        if (!fgets(buffer, BUFSIZE, stdin)) {
            // EOF or error
            break;
        }

        // If user just presses Enter, quit
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }
        
        int plen = (int) strlen(buffer);
        char *temp = malloc(plen + 1);
        
        // Encryption Logic
        printf("\nEncrypting a packet of length: %d", plen);
        cesar_encrypt(buffer, plen, temp);
        temp[plen] = '\0';

        // Output the encrypted packet
        printf(", Encrypted packet:\n\n%s\n", temp);

        // Decryption Logic
        cesar_decrypt(temp, plen, buffer);
        buffer[plen] = '\0';

        printf("Decrypted packet:\n\n%s\n\n", buffer);
        free(temp);
    }
    cleanup:
    free(buffer);
    return 0;
}