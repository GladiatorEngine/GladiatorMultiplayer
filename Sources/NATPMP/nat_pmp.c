//
//  nat_pmp.c
//  
//
//  Created by Pavel Kasila on 11/29/20.
//

#include "nat_pmp.h"

char* send_nat_pmp(int16_t internal_port, int16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol, uint64_t lifetime) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    char* buf = "123";
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(NAT_PMP_PORT);
    inet_pton(AF_INET, gateway, &(servaddr.sin_addr));
    
    sendto(sfd, buf, strlen(buf), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    
    char* rbuf = malloc(sizeof(char[MAX_LINE]));
    socklen_t n, len;
    n = recvfrom(sfd, rbuf, MAX_LINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    rbuf[n] = '\0';
    
    close(sfd);
    
    return rbuf;
}

int16_t nat_map_external_port(int16_t internal_port, int16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, NAT_PMP_LIFETIME);
    printf("Server : %s\n", rbuf);
    return 0;
}

bool nat_pmp_disable_mapping(int16_t internal_port, int16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, 0);
    printf("Server : %s\n", rbuf);
    return false;
}
