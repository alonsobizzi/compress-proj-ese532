#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIN_SIZE 48       // Larger window for better boundaries
#define PRIME 31
#define MODULUS 4096      // 4KB average chunks  
#define TARGET 0
#define MAX_CHUNK 8192

uint64_t initial_hash(unsigned char *input, unsigned int start_pos) {
    uint64_t hash = 0;
    for(int i = 0; i < WIN_SIZE; i++) {
        hash = hash * PRIME + input[start_pos + i];
    }
    return hash;
}

void cdc(unsigned char *buff, unsigned int buff_size) {
    if(buff_size < WIN_SIZE) return;
    
    unsigned int chunk_start = 0;
    
    // Pre-calculate PRIME^WIN_SIZE
    uint64_t prime_power = 1;
    for(int j = 0; j < WIN_SIZE; j++) {
        prime_power *= PRIME;
    }
    
    // Calculate initial hash for first window
    uint64_t hash = initial_hash(buff, 0);
    
    // Check first window  
    if((hash % MODULUS) == TARGET) {
        send_to_sha(&buff[chunk_start], WIN_SIZE);
        chunk_start = WIN_SIZE;
    }
    
    // Rolling hash through remaining positions
    for(unsigned int i = WIN_SIZE; i < buff_size; i++) {
        // Rolling update: remove buff[i-WIN_SIZE], add buff[i]
        hash = hash * PRIME - buff[i - WIN_SIZE] * prime_power + buff[i];
        
        unsigned int chunk_len = i - chunk_start + 1;
        
        if(((hash % MODULUS) == TARGET) || chunk_len >= MAX_CHUNK) {
            send_to_sha(&buff[chunk_start], chunk_len);
            chunk_start = i + 1;
        }
    }
    
    // Handle final chunk
    if(chunk_start < buff_size) {
        send_to_sha(&buff[chunk_start], buff_size - chunk_start);
    }
}
