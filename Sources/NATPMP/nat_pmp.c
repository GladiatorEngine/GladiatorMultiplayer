//
//  nat_pmp.c
//  
//
//  Created by Pavel Kasila on 11/29/20.
//

#include "nat_pmp.h"

char* send_nat_pmp(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol, uint32_t lifetime) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buf[MAX_LINE];
    memset(buf, 0, sizeof(buf)); // zeroing buffer
    
    // Build packet
    buf[1] = (int)mapping_protocol; // Protocol: 1 - UDP, 2 - TCP
    // Internal port
    buf[5] = internal_port & 0xFF;
    buf[6] = internal_port >> 8;
    // External port
    buf[7] = requested_external_port & 0xFF;
    buf[8] = requested_external_port >> 8;
    // Lifetime
    buf[9] =  (lifetime >> 0) & 0xFF;
    buf[10] = (lifetime >> 8) & 0xFF;
    buf[11] = (lifetime >> 16) & 0xFF;
    buf[12] = (lifetime >> 24) & 0xFF;
    // End build packet
    
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

uint16_t nat_map_external_port(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, NAT_PMP_LIFETIME);
    printf("Server : %s\n", rbuf);
    return 0;
}

bool nat_pmp_disable_mapping(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, 0);
    printf("Server : %s\n", rbuf);
    return false;
}
