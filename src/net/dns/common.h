#ifndef DALE_NET_DNS_COMMON_H
#define DALE_NET_DNS_COMMON_H

/**
 * A file containing definitions for structs, macros and functions
 * relating to DNS queries and responses.
 *
 * @author  jwren0
 * @version 2023-08-15
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
 * A DNS header as described by
 * RFC 1035 section 4.1.1.
 */
typedef struct {
    uint16_t id;
    uint16_t _meta;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} DNSHeader;

/**
 * A DNS question section as defined by
 * RFC 1035 section 4.1.2.
 */
typedef struct {
    uint8_t qname[MAX_NAME + 1];
    uint16_t qtype;
    uint16_t qclass;
} DNSQuestion;

#endif // DALE_NET_DNS_COMMON_H
