#include "shaComp.h"

int init_sha3_socket(void) {

    struct sockaddr_alg sa;
    memset(&sa, 0, sizeof(sa));

    sa.salg_family = AF_ALG;
    strcpy((char *)sa.salg_type, "hash");
    strcpy((char *)sa.salg_name, "xilinx-keccak-384");

    //    struct sockaddr_alg sa = {
    //.salg_family = AF_ALG,
    //.salg_type = "hash",
    //.salg_name = "sha3-384"
    //};
    

    int sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }
    return sockfd; //DONOTRETURN 0
}

// ===================== Execution =====================
int compute_sha3_hash(int sockfd, const void *input, size_t len, unsigned char *digest) {
    int fd = accept(sockfd, NULL, 0); // Does this need to be in the init?
    if (fd < 0) {
        perror("accept");
        return -1;
    }

    if (write(fd, input, len) != (ssize_t)len) {
        perror("write");
        close(fd);
        return -1;
    }

    if (read(fd, digest, SHA384_DIGEST_SZ) != SHA384_DIGEST_SZ) {
        perror("read");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}