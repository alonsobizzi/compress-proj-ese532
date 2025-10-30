#include "shaComp.h"
// Demo
#define NUMTESTHASH (4)
int main(void) {
    const char *input[NUMTESTHASH] = {
        "Hellhash1",
        "Chriswuzhere",
        "Legos",
        "wormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormwormworm"
    };
    unsigned char outhash[NUMTESTHASH][SHA384_DIGEST_SZ];
    //Hellhasssssssssssssssssss
    int sockfd = init_sha3_socket();
    if (sockfd < 0)
        return -222; // Error with socket
    for (int i = 0; i < NUMTESTHASH; i++){
    compute_sha3_hash(sockfd, input[i], strlen(input[i]), outhash[i]);
    }
    
    //This comes after we are done all hashes
    close(sockfd);
    for (int in = 0; in < NUMTESTHASH; in++){
        printf("SHA3-384 digest of \"%s\":\n", input[in]);
        for (int i = 0; i < SHA384_DIGEST_SZ; i++)
            printf("%02x", outhash[in][i]);
        printf("\n");
        }
    return 0;
}

