//
//  nat_pmp.c
//  
//
//  Created by Pavel Kasila on 11/29/20.
//

#include "nat_pmp.h"

int16_t nat_pmp_external_port(int16_t internal_port, int16_t requested_external_port, const struct sockaddr gateway, enum MappingProtocol mapping_protocol) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    char* buf = "123";
    sendto(sfd, buf, strlen(buf), 0, &gateway, sizeof(gateway));
    
    // TODO: implement receiving
    
    return 0;
}
