#ifndef DALE_STRUCTS_H
#define DALE_STRUCTS_H

/**
 * A file containing definitions for structs, macros and functions
 * relating to DNS queries.
 *
 * @author  jwren0
 * @version 2023-08-11
 */

#include <arpa/inet.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/**
 * The maximum size of a domain name as defined
 * by RFC 1035 section 2.3.4.
 */
#define MAX_NAME 255

/**
 * The maximum size of a label as defined
 * by RFC 1035 section 2.3.4.
 */
#define MAX_LABEL 63

/**
 * The maximum size of a UDP message as defined
 * by RFC 1035 section 2.3.4.
 */
#define MAX_UDP 512

/**
 * A DNS header as described by
 * RFC 1035 section 4.1.1.
 */
typedef struct {
    uint16_t id;
    uint16_t meta;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} DNSHeader;

/**
 * A macro to retrieve 'ID' from a DNSHeader.
 */
#define DNSHeader_id(header) (header).id

/**
 * A macro to retrieve 'QR' from a DNSHeader.
 */
#define DNSHeader_qr(header) (header).meta & (1 << 15)

/**
 * A macro to retrieve 'Opcode' from a DNSHeader.
 */
#define DNSHeader_opcode(header) (header).meta & (15 << 14)

/**
 * A macro to retrieve 'AA' from a DNSHeader.
 */
#define DNSHeader_aa(header) (header).meta & (1 << 10)

/**
 * A macro to retrieve 'TC' from a DNSHeader.
 */
#define DNSHeader_tc(header) (header).meta & (1 << 9)

/**
 * A macro to retrieve 'RD' from a DNSHeader.
 */
#define DNSHeader_rd(header) (header).meta & (1 << 8)

/**
 * A macro to retrieve 'RA' from a DNSHeader.
 */
#define DNSHeader_ra(header) (header).meta & (1 << 7)

/**
 * A macro to retrieve 'Z' from a DNSHeader.
 */
#define DNSHeader_z(header) (header).meta & (7 << 6)

/**
 * A macro to retrieve 'RCODE' from a DNSHeader.
 */
#define DNSHeader_rcode(header) (header).meta & (1 << 3)

/**
 * A macro to retrieve 'QDCOUNT' from a DNSHeader.
 */
#define DNSHeader_qdcount(header) (header).qdcount

/**
 * A macro to retrieve 'ANCOUNT' from a DNSHeader.
 */
#define DNSHeader_ancount(header) (header).ancount

/**
 * A macro to retrieve 'NSCOUNT' from a DNSHeader.
 */
#define DNSHeader_nscount(header) (header).nscount

/**
 * A macro to retrieve 'ARCOUNT' from a DNSHeader.
 */
#define DNSHeader_arcount(header) (header).arcount

/**
 * A DNS question section as defined by
 * RFC 1035 section 4.1.2.
 */
typedef struct {
    uint8_t qname[MAX_NAME + 1];
    uint16_t qtype;
    uint16_t qclass;
} DNSQuestion;

/**
 * Updates a DNSQuestion struct based on a provided
 * pointer to the beginning of a question section.
 *
 * This function will fail if the QNAME was too long.
 *
 * @param question The DNSQuestion struct to update.
 * @param buf The buffer containing the question section.
 * @return zero on success, non-zero otherwise.
 */
int DNSQuestion_init(DNSQuestion *question, uint8_t *buf);

/**
 * A DNS query containing a header and question.
 */
typedef struct {
    DNSHeader header;
    DNSQuestion question;
} DNSQuery;

/**
 * Updates a DNSQuery struct based on a provided
 * buffer containing a DNS query.
 *
 * @param query The DNSQuery struct to update.
 * @param buf The buffer containing the DNS query.
 * @return zero on success, non-zero otherwise.
 */
int DNSQuery_init(DNSQuery *query, uint8_t *buf);

#endif // DALE_STRUCTS_H
