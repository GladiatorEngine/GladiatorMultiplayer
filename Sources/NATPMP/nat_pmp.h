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
#include <string.h>
#include <sys/socket.h>

enum MappingProtocol {
    TCP = 0x00,
    UDP = 0x01
};

int16_t nat_pmp_external_port(int16_t internal_port, int16_t requested_external_port, const struct sockaddr gateway, enum MappingProtocol mapping_protocol);

#endif /* File_h */
