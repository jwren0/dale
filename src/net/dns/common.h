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
 * Possible TYPE values as defined
 * by RFC 1035 section 3.2.2.
 */
#define TYPE_A      1
#define TYPE_NS     2
#define TYPE_CNAME  5
#define TYPE_SOA    6
#define TYPE_WKS    11
#define TYPE_PTR    12
#define TYPE_HINFO  13
#define TYPE_MINFO  14
#define TYPE_MX     15
#define TYPE_TXT    16

/**
 * Possible TYPE values as defined
 * by RFC 3596 section 2.1.
 */
#define TYPE_AAAA 28

/**
 * Possible CLASS values as defined
 * by RFC 1035 section 3.2.4.
 */
#define CLASS_IN 1
#define CLASS_CH 3
#define CLASS_HS 4

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
 * Possible values for QR as defined
 * by RFC 1035 section 4.1.1.
 */
#define QR_QUERY    0
#define QR_RESPONSE 1

/**
 * Possible values for Opcode as defined
 * by RFC 1035 section 4.1.1.
 */
#define OPCODE_QUERY  0
// Obsoleted by RFC 3425
// #define OPCODE_IQUERY 1
#define OPCODE_STATUS 2

/**
 * Possible values for AA as defined
 * by RFC 1035 section 4.1.1.
 */
#define AA_FALSE 0
#define AA_TRUE  1

/**
 * Possible values for TC as defined
 * by RFC 1035 section 4.1.1.
 */
#define TC_FALSE 0
#define TC_TRUE  1

/**
 * Possible values for RD as defined
 * by RFC 1035 section 4.1.1.
 */
#define RD_FALSE 0
#define RD_TRUE  1

/**
 * Possible values for RA as defined
 * by RFC 1035 section 4.1.1.
 */
#define RA_FALSE 0
#define RA_TRUE  1

/**
 * Possible values for RCODE as defined
 * by RFC 1035 section 4.1.1.
 */
// No error
#define RCODE_ENONE   0
// Format error
#define RCODE_EFORMAT 1
// Server failure
#define RCODE_ESERV   2
// Name error
#define RCODE_ENAME   3
// Not implemented
#define RCODE_EIMPL   4
// Refused
#define RCODE_EREFUSE 5

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
