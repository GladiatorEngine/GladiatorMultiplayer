//
//  NATHolePunching.swift
//  
//
//  Created by Pavel Kasila on 11/30/20.
//

import Foundation
import NATPMP

struct NATHolePunching {
    static func getGatewayIP() -> String {
        return "192.168.0.1"
    }
    
    static func punchHole(internalPort: UInt16, suggestedExternalPort: UInt16, proto: NATHolePunchingProtocols) -> UInt16 {
        nat_map_external_port(internalPort, suggestedExternalPort, getGatewayIP().cString(using: .ascii), .init(proto.rawValue))
    }
    
    static func closeHole(internalPort: UInt16, suggestedExternalPort: UInt16, proto: NATHolePunchingProtocols) -> Bool {
        nat_pmp_disable_mapping(internalPort, suggestedExternalPort, getGatewayIP().cString(using: .ascii), .init(proto.rawValue))
    }
}

enum NATHolePunchingProtocols: UInt32 {
    case UDP = 1
    case TCP = 2
}
