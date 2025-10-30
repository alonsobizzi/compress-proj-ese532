#include "cdc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cdc_init(CDC *cdc) {
    cdc->buffer = (unsigned char*)malloc(CDC_BUFFER_SIZE);
    cdc->buffer_size = CDC_BUFFER_SIZE;
    cdc->write_pos = 0;
    cdc->read_pos = 0;
    cdc->rolling_hash = 0;
    cdc->chunk_start = 0;
    cdc->hash_initialized = false;
    cdc->available_data = 0;
    cdc->current_chunk_len = 0;
    
    // Pre-calculate PRIME^WIN_SIZE
    cdc->prime_power = 1;
    for(int i = 0; i < WIN_SIZE; i++) {
        cdc->prime_power *= PRIME;
    }
}

void cdc_cleanup(CDC *cdc) {
    if (cdc->buffer) {
        free(cdc->buffer);
        cdc->buffer = NULL;
    }
}

unsigned char cdc_get_byte(CDC *cdc, unsigned int pos) {
    return cdc->buffer[pos % cdc->buffer_size];
}

void cdc_process_chunk(unsigned char *chunk_data, unsigned int chunk_size) {
    // TODO: Replace with actual SHA + LZW/deduplication processing
    printf("CDC: Chunk ready - %u bytes\n", chunk_size);
    
    // Debug: print first few bytes
    printf("First bytes: ");
    for(int i = 0; i < (chunk_size < 8 ? chunk_size : 8); i++) {
        printf("%02x ", chunk_data[i]);
    }
    printf("\n");
}

static void cdc_send_chunk(CDC *cdc, unsigned int start_pos, unsigned int length) {
    // Extract chunk from circular buffer
    unsigned char *chunk_data = (unsigned char*)malloc(length);
    
    for(unsigned int i = 0; i < length; i++) {
        chunk_data[i] = cdc_get_byte(cdc, start_pos + i);
    }
    
    cdc_process_chunk(chunk_data, length);
    free(chunk_data);
    
    // Update read position and available data
    cdc->read_pos = (cdc->read_pos + length) % cdc->buffer_size;
    cdc->available_data -= length;
}

static void cdc_process_available_data(CDC *cdc, bool final) {
    // Initialize hash for first window
    if (!cdc->hash_initialized && cdc->available_data >= WIN_SIZE) {
        cdc->rolling_hash = 0;
        for(int i = 0; i < WIN_SIZE; i++) {
            cdc->rolling_hash = cdc->rolling_hash * PRIME + 
                               cdc_get_byte(cdc, cdc->read_pos + i);
        }
        cdc->hash_initialized = true;
        cdc->chunk_start = cdc->read_pos;
        cdc->current_chunk_len = WIN_SIZE;
        
        // Check if first window is boundary
        if ((cdc->rolling_hash % MODULUS) == TARGET) {
            cdc_send_chunk(cdc, cdc->chunk_start, WIN_SIZE);
            cdc->chunk_start = cdc->read_pos;
            cdc->current_chunk_len = 0;
            cdc->hash_initialized = false; // Reset for next chunk
            return;
        }
    }
    
    // Process with rolling hash
    while (cdc->available_data > WIN_SIZE || 
           (final && cdc->current_chunk_len > 0)) {
        
        if (cdc->available_data > WIN_SIZE && cdc->hash_initialized) {
            // Calculate new hash position
            unsigned int new_pos = cdc->read_pos + cdc->current_chunk_len;
            unsigned int old_pos = new_pos - WIN_SIZE;
            
            // Rolling hash update
            unsigned char old_byte = cdc_get_byte(cdc, old_pos);
            unsigned char new_byte = cdc_get_byte(cdc, new_pos);
            
            cdc->rolling_hash = cdc->rolling_hash * PRIME - 
                               old_byte * cdc->prime_power + new_byte;
            
            cdc->current_chunk_len++;
            
            // Check for chunk boundary or max size
            if (((cdc->rolling_hash % MODULUS) == TARGET) || 
                cdc->current_chunk_len >= MAX_CHUNK) {
                
                cdc_send_chunk(cdc, cdc->chunk_start, cdc->current_chunk_len);
                cdc->chunk_start = cdc->read_pos;
                cdc->current_chunk_len = 0;
                cdc->hash_initialized = false; // Reset for next chunk
                break;
            }
        } else if (final && cdc->current_chunk_len > 0) {
            // Send final chunk
            cdc_send_chunk(cdc, cdc->chunk_start, cdc->current_chunk_len);
            cdc->current_chunk_len = 0;
            break;
        } else {
            break;
        }
    }
}

int cdc_add_data(CDC *cdc, unsigned char *data, int len, bool final) {
    // Check buffer space
    if (cdc->available_data + len > cdc->buffer_size) {
        printf("CDC buffer overflow!\n");
        return -1;
    }
    
    // Add data to circular buffer
    for(int i = 0; i < len; i++) {
        cdc->buffer[cdc->write_pos] = data[i];
        cdc->write_pos = (cdc->write_pos + 1) % cdc->buffer_size;
    }
    
    cdc->available_data += len;
    
    // Process available data
    cdc_process_available_data(cdc, final);
    
    return 0;
}
