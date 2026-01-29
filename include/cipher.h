#ifndef CIPHER_H
#define CIPHER_H

#ifdef __cplusplus 

extern "C" { 
#endif

#include <stdint.h>
#include <stddef.h>

void generate_key(char *filename);

void read_key(char* filename, unsigned char* key_buffer);

void encrypt_file(char* source_file, char* key_file, char* output_bin, char* output_txt);

void decrypt_file(char* input_file, char* key_file, char* output_file);

void secure_memzero(void *ptr, size_t len);

uint32_t generate_nonce(uint32_t *counter);

void shuffle_round(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d);

void shuffle(uint32_t arr[4][4]);

#ifdef __cplusplus 
}

#endif

#endif