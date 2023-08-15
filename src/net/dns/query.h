#ifndef DALE_NET_DNS_QUERY_H
#define DALE_NET_DNS_QUERY_H

/**
 * A file containing definitions for structs,
 * macros, and functions relating to DNS queries.
 *
 * @author  jwren0
 * @version 2023-08-15
 */

#include "common.h"

/**
 * Possible QTYPE values as defined
 * by RFC 1035 section 3.2.3.
 *
 * These extend the already defined TYPE values.
 */
#define QTYPE_AXFR  252
#define QTYPE_MAILB 253
#define QTYPE_MAILA 254
#define QTYPE_ALL   255

/**
 * Possible QCLASS values as defined
 * by RFC 1035 section 3.2.5.
 *
 * These extend the already defined CLASS values.
 */
#define QCLASS_ANY 255

/**
 * Updates a DNSHeader struct based on a provided
 * pointer to the beginning of a header.
 *
 * @param header The DNSHeader struct to update.
 * @param buf The buffer containing the header.
 * @return zero on success, non-zero otherwise.
 */
int DNSHeader_from(DNSHeader *header, const char *buf);

/**
 * A macro to retrieve 'QR' from a DNSHeader.
 */
#define DNSHeader_qr(header) ((header)._meta >> 15) & 1

/**
 * A macro to retrieve 'Opcode' from a DNSHeader.
 */
#define DNSHeader_opcode(header) ((header)._meta >> 11) & 15

/**
 * A macro to retrieve 'AA' from a DNSHeader.
 */
#define DNSHeader_aa(header) ((header)._meta >> 10) & 1

/**
 * A macro to retrieve 'TC' from a DNSHeader.
 */
#define DNSHeader_tc(header) ((header)._meta >> 9) & 1

/**
 * A macro to retrieve 'RD' from a DNSHeader.
 */
#define DNSHeader_rd(header) ((header)._meta >> 8) & 1

/**
 * A macro to retrieve 'RA' from a DNSHeader.
 */
#define DNSHeader_ra(header) ((header)._meta >> 7) & 1

/**
 * A macro to retrieve 'Z' from a DNSHeader.
 */
#define DNSHeader_z(header) ((header)._meta >> 4) & 7

/**
 * A macro to retrieve 'RCODE' from a DNSHeader.
 */
#define DNSHeader_rcode(header) (header)._meta & 15

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
int DNSQuestion_from(DNSQuestion *question, const char *buf);

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
int DNSQuery_from(DNSQuery *query, const char *buf);

#endif // DALE_NET_DNS_QUERY_H
