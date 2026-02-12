#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher.h"

void print_usage(const char *prog_name) {
    printf("Simple Shuffle Cipher - Command Line Interface\n");
    printf("\nUsage:\n");
    printf("  %s generate <key_file>                    Generate a new key file\n", prog_name);
    printf("  %s encrypt <input> <key> <out.bin> <out.txt>  Encrypt a file\n", prog_name);
    printf("  %s decrypt <input.bin> <key> <output>     Decrypt a file\n", prog_name);
    printf("\nExamples:\n");
    printf("  %s generate mykey.key\n", prog_name);
    printf("  %s encrypt plaintext.txt mykey.key encrypted.bin hex.txt\n", prog_name);
    printf("  %s decrypt encrypted.bin mykey.key decrypted.txt\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "generate") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: generate requires <key_file> argument\n");
            return 1;
        }
        printf("Generating key file: %s\n", argv[2]);
        generate_key(argv[2]);
        printf("Key generated successfully!\n");
    }
    else if (strcmp(argv[1], "encrypt") == 0) {
        if (argc != 6) {
            fprintf(stderr, "Error: encrypt requires <input> <key> <out.bin> <out.txt> arguments\n");
            return 1;
        }
        printf("Encrypting: %s\n", argv[2]);
        printf("Key file: %s\n", argv[3]);
        printf("Binary output: %s\n", argv[4]);
        printf("Hex output: %s\n", argv[5]);
        encrypt_file(argv[2], argv[3], argv[4], argv[5]);
    }
    else if (strcmp(argv[1], "decrypt") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Error: decrypt requires <input.bin> <key> <output> arguments\n");
            return 1;
        }
        printf("Decrypting: %s\n", argv[2]);
        printf("Key file: %s\n", argv[3]);
        printf("Output: %s\n", argv[4]);
        decrypt_file(argv[2], argv[3], argv[4]);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}