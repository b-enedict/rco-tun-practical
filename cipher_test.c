#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *buffer = malloc(200);

    while(1) {
        printf("Enter your text: ");
        fgets(buffer, sizeof(buffer), stdin);
        
        int plen = strlen(buffer);
        char *temp = malloc(plen);
        
        // Encryption Logic
        printf("\nEncrypting a packet of length: %d", plen);
        memcpy(temp, buffer, plen);

        // Output the encrypted packet
        printf(", Encrypted packet:\n\n%s\n", temp);

        // Decryption Logic
        memcpy(buffer, temp, plen);

        printf("Decrypted packet:\n\n%s\n\n", buffer);
        free(temp);
    }
    cleanup:
    free(buffer);
    return 0;
}