#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#ifdef _WIN32
    #include <windows.h>
    #define SECURE_ZERO(ptr, len) SecureZeroMemory(ptr, len)
#else
    static void secure_memzero_impl(void *ptr, size_t len) {
        volatile unsigned char *p = (volatile unsigned char *)ptr;
        while (len--) {
            *p++ = 0;
        }
    }
    #define SECURE_ZERO(ptr, len) secure_memzero_impl(ptr, len)
#endif

void secure_memzero(void *ptr, size_t len) {
    if (ptr != NULL && len > 0) {
        SECURE_ZERO(ptr, len);
    }
}

void generate_key(const char* filename) {
    uint32_t gen_key[4][4];
    uint32_t counter = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            counter++;
            gen_key[i][j] = generate_nonce(&counter);
        }
    }

    shuffle(gen_key);

    FILE *fd = fopen(filename, "wb");
    if (fd == NULL) {
        fprintf(stderr, "Error: Cannot create key file '%s'\n", filename);
        return;
    }

    for (int i = 0; i < 4; i++) {
        fwrite(gen_key[i], sizeof(uint32_t), 4, fd);
    }
    fclose(fd);

    secure_memzero(gen_key, sizeof(gen_key));
}

void read_key(const char* filename, unsigned char* key_buffer) {
    FILE *fd = fopen(filename, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Error: Cannot open key file '%s'\n", filename);
        return;
    }

    size_t bytes_read = fread(key_buffer, 1, 64, fd);
    fclose(fd);

    if (bytes_read != 64) {
        fprintf(stderr, "Warning: Expected 64 bytes key, read %zu bytes\n", bytes_read);
    }
}

void encrypt_file(const char* source_file, const char* key_file, const char* output_bin, const char* output_txt) {
    unsigned char key[64];
    unsigned char buffer[64];

    read_key(key_file, key);

    FILE *input_fd = fopen(source_file, "rb");
    if (input_fd == NULL) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", source_file);
        return;
    }

    FILE *out_bin = fopen(output_bin, "wb");
    if (out_bin == NULL) {
        fprintf(stderr, "Error: Cannot create output binary file '%s'\n", output_bin);
        fclose(input_fd);
        return;
    }

    FILE *out_txt = fopen(output_txt, "w");
    if (out_txt == NULL) {
        fprintf(stderr, "Error: Cannot create output text file '%s'\n", output_txt);
        fclose(input_fd);
        fclose(out_bin);
        return;
    }

    size_t bytes_read;
    size_t total_bytes = 0;

    while ((bytes_read = fread(buffer, 1, 64, input_fd)) > 0) {
        total_bytes += bytes_read;

        if (bytes_read < 64) {
            memset(buffer + bytes_read, 0, 64 - bytes_read);
        }

        for (size_t i = 0; i < 64; i++) {
            buffer[i] ^= key[i];
        }

        fwrite(buffer, 1, 64, out_bin);

        for (size_t i = 0; i < 64; i++) {
            fprintf(out_txt, "%02X", buffer[i]);
            if ((i + 1) % 8 == 0 && i != 63) {
                fprintf(out_txt, " ");
            }
        }
        fprintf(out_txt, "\n");
    }

    printf("Encryption completed successfully!\n");
    printf("Total bytes processed: %zu\n", total_bytes);

    fclose(input_fd);
    fclose(out_bin);
    fclose(out_txt);

    secure_memzero(key, sizeof(key));
}

void decrypt_file(const char* input_file, const char* key_file, const char* output_file) {
    unsigned char key[64];
    unsigned char buffer[64];

    read_key(key_file, key);

    FILE *input_fd = fopen(input_file, "rb");
    if (input_fd == NULL) {
        fprintf(stderr, "Error: Cannot open input file '%s'\n", input_file);
        return;
    }

    FILE *output_fd = fopen(output_file, "wb");
    if (output_fd == NULL) {
        fprintf(stderr, "Error: Cannot create output file '%s'\n", output_file);
        fclose(input_fd);
        return;
    }

    size_t bytes_read;
    size_t total_bytes = 0;

    while ((bytes_read = fread(buffer, 1, 64, input_fd)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            buffer[i] ^= key[i];
        }

        fwrite(buffer, 1, bytes_read, output_fd);
        total_bytes += bytes_read;
    }

    printf("File decrypted successfully: %s\n", output_file);
    printf("Total bytes processed: %zu\n", total_bytes);

    fclose(input_fd);
    fclose(output_fd);

    secure_memzero(key, sizeof(key));
}

uint32_t generate_nonce(uint32_t *counter) {
    uint32_t random_value = 0;

    FILE *fd = fopen("/dev/urandom", "rb");
    if (fd) {
        if (fread(&random_value, sizeof(uint32_t), 1, fd) != 1) {
            random_value = (uint32_t)time(NULL);
        }
        fclose(fd);
    } else {
        random_value = (uint32_t)time(NULL);
    }

    uint32_t gen_counter = htole32((*counter)++);
    uint32_t nonce = random_value ^ (gen_counter & 0x0000000F);

    return nonce;
}

void shuffle_round(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
    *a += *b;
    *d ^= *a;
    *d = (*d << 16) | (*d >> 16);

    *c += *d;
    *b ^= *c;
    *b = (*b << 12) | (*b >> 20);

    *a += *b;
    *d ^= *a;
    *d = (*d << 8) | (*d >> 24);

    *c += *d;
    *b ^= *c;
    *b = (*b << 7) | (*b >> 25);
}

void shuffle(uint32_t arr[4][4]) {
    for (int t = 0; t < 20; t++) {
        for (int i = 0; i < 4; i++) {
            shuffle_round(&arr[0][i], &arr[1][i], &arr[2][i], &arr[3][i]);
        }

        for (int i = 0; i < 4; i++) {
            shuffle_round(&arr[i][0], &arr[i][1], &arr[i][2], &arr[i][3]);
        }

        shuffle_round(&arr[0][0], &arr[1][1], &arr[2][2], &arr[3][3]);
        shuffle_round(&arr[0][1], &arr[1][2], &arr[2][3], &arr[3][0]);
        shuffle_round(&arr[0][2], &arr[1][3], &arr[2][0], &arr[3][1]);
        shuffle_round(&arr[0][3], &arr[1][0], &arr[2][1], &arr[3][2]);
    }
}
