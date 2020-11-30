//
//  nat_pmp.c
//  
//
//  Created by Pavel Kasila on 11/29/20.
//

#include "nat_pmp.h"

char* send_nat_pmp(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol, uint32_t lifetime) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buf[PMP_REQUEST_SIZE];
    memset(buf, 0, sizeof(buf)); // zeroing buffer
    
    // Build packet
    buf[1] = (uint8_t)mapping_protocol; // Protocol: 1 - UDP, 2 - TCP
    // Internal port
    buf[5] = (internal_port >> 0) & 0xFF;
    buf[4] = (internal_port >> 8) & 0xFF;
    // External port
    buf[7] = (requested_external_port >> 0) & 0xFF;
    buf[6] = (requested_external_port >> 8) & 0xFF;
    // Lifetime
    buf[11] =  (lifetime >> 0) & 0xFF;
    buf[10] = (lifetime >> 8) & 0xFF;
    buf[9] = (lifetime >> 16) & 0xFF;
    buf[8] = (lifetime >> 24) & 0xFF;
    // End build packet
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(NAT_PMP_PORT);
    inet_pton(AF_INET, gateway, &(servaddr.sin_addr));
    
    sendto(sfd, buf, strlen(buf), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    
    char* rbuf = malloc(sizeof(char[PMP_RESPONSE_SIZE]));
    socklen_t len;
    recvfrom(sfd, rbuf, PMP_RESPONSE_SIZE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    
    close(sfd);
    
    return rbuf;
}

NATPMPResponse* parse_nat_pmp_response(const char* rbuf, enum MappingProtocol mapping_protocol) {
    NATPMPResponse* r = malloc(sizeof(NATPMPResponse));
    
    r->opcode = rbuf[1];
    r->resultCode = (unsigned char)(rbuf[3]) << 8 |
                    (unsigned char)(rbuf[2]);
    r->epochTime = (unsigned char)(rbuf[7]) << 24 |
                   (unsigned char)(rbuf[6]) << 16 |
                   (unsigned char)(rbuf[5]) << 8 |
                   (unsigned char)(rbuf[4]);
    r->internalPort = (unsigned char)(rbuf[9]) << 8 |
                      (unsigned char)(rbuf[8]);
    r->externalPort = (unsigned char)(rbuf[11]) << 8 |
                      (unsigned char)(rbuf[10]);
    r->lifetime = (unsigned char)(rbuf[15]) << 24 |
                  (unsigned char)(rbuf[14]) << 16 |
                  (unsigned char)(rbuf[13]) << 8 |
                  (unsigned char)(rbuf[12]);
    
    return r;
}

uint16_t nat_map_external_port(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, NAT_PMP_LIFETIME);
    NATPMPResponse* r = parse_nat_pmp_response(rbuf, mapping_protocol);
    return r->externalPort;
}

bool nat_pmp_disable_mapping(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol) {
    char* rbuf = send_nat_pmp(internal_port, requested_external_port, gateway, mapping_protocol, 0);
    NATPMPResponse* r = parse_nat_pmp_response(rbuf, mapping_protocol);
    return r->opcode == (uint8_t)mapping_protocol+128;
}
