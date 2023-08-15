#ifndef DALE_NET_DNS_RESPONSE_H
#define DALE_NET_DNS_RESPONSE_H

/**
 * A file containing definitions for structs,
 * macros, and functions relating to DNS responses.
 *
 * @author  jwren0
 * @version 2023-08-15
 */

#include "common.h"
#include "query.h"

/**
 * Macros to update 'QR' in a DNSHeader.
 */
#define DNSHeader_set_qr(header) (header)._meta |= (1 << 15)
#define DNSHeader_unset_qr(header) (header)._meta &= ~(1 << 15)

/**
 * A macro to update 'Opcode' in a DNSHeader.
 */
#define DNSHeader_set_opcode(header, value) (header)._meta = ((header)._meta & ~(15 << 11)) | (value << 11)

/**
 * Macros to update 'AA' in a DNSHeader.
 */
#define DNSHeader_set_aa(header) (header)._meta |= (1 << 10)
#define DNSHeader_unset_aa(header) (header)._meta &= ~(1 << 10)

/**
 * Macros to update 'TC' in a DNSHeader.
 */
#define DNSHeader_set_tc(header) (header)._meta |= (1 << 9)
#define DNSHeader_unset_tc(header) (header)._meta &= ~(1 << 9)

/**
 * Macros to update 'RD' in a DNSHeader.
 */
#define DNSHeader_set_rd(header) (header)._meta |= (1 << 8)
#define DNSHeader_unset_rd(header) (header)._meta &= ~(1 << 8)

/**
 * Macros to update 'RA' in a DNSHeader.
 */
#define DNSHeader_set_ra(header) (header)._meta |= (1 << 7)
#define DNSHeader_unset_ra(header) (header)._meta &= ~(1 << 7)

/**
 * A macro to forcefully set 'Z' in a DNSHeader.
 */
#define DNSHeader_set_z(header) (header)._meta &= ~(7 << 3)

/**
 * A macro to update 'RCODE' in a DNSHeader.
 */
#define DNSHeader_set_rcode(header, value) (header)._meta = ((header)._meta & ~15) | value

/**
 * A DNS Resource Record as described by
 * RFC 1035 section 4.1.3.
 */
typedef struct {
    uint8_t *name;
    uint16_t type;
    uint16_t class_;
    uint32_t ttl;
    uint16_t rdlength;
    uint8_t *rdata;
} DNSResource;

/**
 * Writes a DNSQuestion to a buffer.
 *
 * @param question The DNSQuestion struct to use.
 * @param buf The buffer to write to.
 */
void DNSQuestion_to(const DNSQuestion *question, char *buf);

/**
 * Stores a resource record for a null IPv4 address.
 *
 * @param resource The DNSResource struct to update.
 */
void DNSResource_null_a(DNSResource *resource);

/**
 * Stores a resource record for a null IPv6 address.
 *
 * @param resource The DNSResource struct to update.
 */
void DNSResource_null_aaaa(DNSResource *resource);

/**
 * Writes a DNSResource to a buffer.
 *
 * @param question The DNSResource struct to use.
 * @param buf The buffer to write to.
 * @return zero on success, non-zero otherwise
 */
int DNSResource_to(const DNSResource *resource, char *buf);

/**
 * A DNS response containing a header, question,
 * and resource record.
 *
 * This is used for generating custom responses.
 */
typedef struct {
    DNSHeader header;
    DNSQuestion question;
    DNSResource resource;
} DNSResponse;

/**
 * Updates a DNSResponse struct based on a query.
 *
 * @param response The DNSResponse struct to update.
 * @param query The query to use.
 * @return zero on success, non-zero otherwise.
 */
int DNSResponse_from(DNSResponse *response, const DNSQuery *query);

/**
 * Writes a DNSResponse to a buffer.
 *
 * @param response The response to write.
 * @param buf The buffer to write to.
 * @param buf_len The size of the buffer being written to.
 * @return The size of the response on success, negative value otherwise.
 */
int DNSResponse_to(
    const DNSResponse *response,
    char *buf,
    const size_t buf_len
);

#endif // DALE_NET_DNS_RESPONSE_H
