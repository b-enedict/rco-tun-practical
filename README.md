# RCO TUN/TAP Practical

This is a test ground for trying diffrent encryption techniques.

## Cesar

All cesar related files implement the cesar cipher as specified in RCO tarea 3.
The file cesar_encryption.c thereby contains the code for encrypting whereas cipher_test_cesar.c contains a testing programm which also shows how the methods should be used and allows for encrypting and decrypting the content typed into the terminal.

**Compilation command:**
```
clang cipher_test_cesar.c ./cesar/cesar_encryption.c -o cesar.o
```

## ChaCha20

The code for the ChaCha20 encrption is bassed on openSSL, so this library has to be installed on the machine where it will be compilled first. When it is installed usage is similar to the cesar encryption example. Furthermore for the chacha20 streaming cipher a shared_secret.key file has to be present in the directory, where the programm is executed. An example shared key is added in the repository. Note that for ChaCha20 to work correctly for encryption and decryption, all additional parameters (key, block_count, nonce) have to be set to the same value.

**Compilation command:**
```
clang cipher_test_chacha20.c chacha20/chacha20_encryption.c -o chacha -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lcrypto -lssl
```

## Remarks
- The compilation commands may differ depending on the environment.
