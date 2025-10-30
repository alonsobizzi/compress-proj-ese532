#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <linux/socket.h>

#define SHA384_DIGEST_SZ 48 // This is 384 bits

int init_sha3_socket(void);
int compute_sha3_hash(int sockfd, const void *input, size_t len, unsigned char *digest);
