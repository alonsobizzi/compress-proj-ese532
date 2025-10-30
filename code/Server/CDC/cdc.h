#ifndef CDC_H
#define CDC_H

#include <stdint.h>
#include <stdbool.h>

#define WIN_SIZE 48       // Window size for rolling hash
#define PRIME 31          // Prime for rolling hash
#define MODULUS 4096      // 4KB average chunks  
#define TARGET 0          // Target hash value for chunk boundary
#define MAX_CHUNK 8192    // Maximum chunk size
#define CDC_BUFFER_SIZE (3 * MAX_CHUNK)  // 24KB circular buffer

typedef struct {
    // Data persistence (essential for streaming)
    unsigned char *buffer;      // Circular buffer for data
    unsigned int buffer_size;   // Buffer size
    unsigned int write_pos;     // Where to write new data
    unsigned int read_pos;      // Where CDC is processing
    
    // CDC state (essential for rolling hash)
    uint64_t rolling_hash;      // Current hash value
    unsigned int chunk_start;   // Start of current chunk
    bool hash_initialized;      // First window processed?
    
    // Precomputed (performance optimization)
    uint64_t prime_power;       // PRIME^WIN_SIZE
    
    // Additional tracking
    unsigned int available_data;    // Bytes ready for processing
    unsigned int current_chunk_len; // Current chunk length
} CDC;

// Function declarations
void cdc_init(CDC *cdc);
void cdc_cleanup(CDC *cdc);
int cdc_add_data(CDC *cdc, unsigned char *data, int len, bool final);
unsigned char cdc_get_byte(CDC *cdc, unsigned int pos);
void cdc_process_chunk(unsigned char *chunk_data, unsigned int chunk_size);

#endif // CDC_H
