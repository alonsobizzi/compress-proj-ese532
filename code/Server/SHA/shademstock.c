/******************************************************************************
* Copyright (c) 2021 Xilinx, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
// Reference file. Unused but kept it. Struct def mandates gcc compile

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <linux/socket.h>
  
#define SHA384_DIGEST_SZ 48
  
int main(void)
{
        struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "hash",
        .salg_name = "sha3-384"
        };
        unsigned char digest[SHA384_DIGEST_SZ];
        char *input = "Hellhash"; /* Input Data should be multiple of 4-bytes */
        int i, sockfd, fd;
  
        sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
        /* Bind with SHA driver */
        bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
        fd = accept(sockfd, NULL, 0);
        /* Send Sha3 hash request with input data to driver */
        write(fd, input, strlen(input));
        /* Read the Sha3 digest output */
        read(fd, digest, SHA384_DIGEST_SZ);
        close(fd);
        close(sockfd);
        for (i = 0; i < SHA384_DIGEST_SZ; i++)
        printf("%02x", digest[i]);
        printf("\n");
        return 0;
}
