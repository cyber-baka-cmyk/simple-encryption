#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void generate_key(char* filename) {
    uint32_t gen_key[4][4];
    uint32_t counter;

    counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            counter++;
            gen_key[i][j] = generate_nonce(&counter);
        }
    }

    shuffle(gen_key);

    FILE *fd = fopen(filename, "wb");

    for (int i = 0; i < 4; i++) {
            fwrite(gen_key[i], sizeof(uint32_t), 4, fd);
    }
    fclose(fd);
}

void read_key(char* filename, unsigned char* key_buffer) {
    FILE *fd = fopen(filename, "rb");

    fseek(fd, 0, SEEK_END);
    long file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    fread(key_buffer, 1, 64, fd);
    fclose(fd);
}

void encrypt_file(char* source_file, char* key_file, char* output_bin, char* output_txt) {
    unsigned char key[64];
    unsigned char key_buffer[64];

    read_key(key_file, key);

    FILE *input_fd = fopen(source_file, "rb");
    FILE *out_bin = fopen(output_bin, "wb");
    FILE *out_txt = fopen(output_txt, "w");

    size_t bytes_read;
    int block_count = 0;
    size_t total_bytes = 0;

    while ((bytes_read = fread(key_buffer, 1, 64, input_fd)) > 0) {
        total_bytes += bytes_read;

        if (bytes_read < 64) memset(key_buffer + bytes_read, 0, 64 - bytes_read);

        for (int i = 0; i < 64; i++) {
            key_buffer[i] ^= key[i]; 
        }

        fwrite(key_buffer, 1, 64, out_bin);
        for (int i = 0; i < 64; i++) {
            fprintf(out_txt, "%02X", key_buffer[i]);
            if ((i + 1) % 8 == 0 && i != 63) {
                fprintf(out_txt, " ");
            }
        }
    }
    fprintf(out_txt, "\n");

    printf("Encryption completed successfully!\n");
    
    fclose(input_fd);
    fclose(out_bin);
    fclose(out_txt);
}

void decrypt_file(char* input_file, char* key_file, char* output_file) {
    unsigned char key[64];
    unsigned char buffer[64];

    read_key(key_file, key);

    FILE *input_fd = fopen(input_file, "rb");
    FILE *output_fd = fopen(output_file, "wb");

    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, 64, input_fd)) > 0) {
        for (int i = 0; i < 64; i++) {
            buffer[i] ^= key[i]; 
        }

        fwrite(buffer, 1, bytes_read, output_fd);
    }  

    printf("File decrypted successfully: %s\n", output_file);

    fclose(input_fd);
    fclose(output_fd);
}

uint32_t generate_nonce(uint32_t *counter) {
    uint8_t random_byte = 0;

    FILE *fd = fopen("/dev/urandom", "rb");
    if (fd) {
        fread(&random_byte, 1, 4, fd);
        fclose(fd);
    }

    uint32_t gen_counter = htole32((*counter)++);
    uint32_t nounce = ((uint32_t)random_byte << 24) ^ (gen_counter & 0x0000000F);

    return nounce;
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
        for (int i = 0; i < 4; i++) shuffle_round(&arr[0][i], &arr[1][i], &arr[2][i], &arr[3][i]);

        for (int i = 0; i < 4; i++) shuffle_round(&arr[i][0], &arr[i][1], &arr[i][2], &arr[i][3]);

        shuffle_round(&arr[0][0], &arr[1][1], &arr[2][2], &arr[3][3]);
        shuffle_round(&arr[0][1], &arr[1][2], &arr[2][3], &arr[3][0]);
        shuffle_round(&arr[0][2], &arr[1][3], &arr[2][0], &arr[3][1]);
        shuffle_round(&arr[0][3], &arr[1][0], &arr[2][1], &arr[3][2]);
    }
}