//
//  nat_pmp.h
//  
//
//  Created by Pavel Kasila on 11/29/20.
//

#ifndef nat_pmp_h
#define nat_pmp_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_LINE 1024
#define NAT_PMP_PORT 5351
#define NAT_PMP_LIFETIME 86400 // one day - 24 hours

enum MappingProtocol {
    UDP = 1,
    TCP = 2
};

typedef struct {
    uint8_t opcode;
    uint16_t resultCode;
    uint32_t epochTime;
    uint16_t internalPort;
    uint16_t externalPort;
    uint32_t lifetime;
} NATPMPResponse;

uint16_t nat_map_external_port(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol);

bool nat_pmp_disable_mapping(uint16_t internal_port, uint16_t requested_external_port, const char* gateway, enum MappingProtocol mapping_protocol);

#endif /* File_h */
