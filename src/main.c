/**
 * A file containing the entry point for dale.
 *
 * @author  jwren0
 * @version 2023-08-11
 */

#include <stdio.h>

#include "structs.h"

int main(void) {
    DNSQuery query;
    // TODO: Receive this from a client
    uint8_t buf[MAX_UDP << 1] = {
        // BEGIN Header

        // ID
        0x09, 0x06,

        // QR, Opcode, AA, TC, RD
        0x1,
        // RA, Z, RCODE
        0x0,

        // QDCOUNT
        0x0, 0x1,

        // ANCOUNT
        0x0, 0x0,

        // NSCOUNT
        0x0, 0x0,

        // ARCOUNT
        0x0, 0x0,

        // END Header


        // BEGIN Question

        // QNAME
        0x6, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65,
        0x3, 0x63, 0x6f, 0x6d,
        0x0,

        // QTYPE
        0x0, 0x1,

        // QCLASS
        0x0, 0x1

        // END Question
    };

    if (DNSQuery_init(&query, buf) != 0) {
        fprintf(stderr, "ERROR: DNSQuery_init failed\n");
        return 1;
    }

    printf(
        "ID:    %d\n"
        "QNAME: %s\n",
        query.header.id,
        query.question.qname
    );
}
