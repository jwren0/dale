#include "response.h"

void DNSQuestion_to(const DNSQuestion *question, char *buf) {
    char count = 0;
    uint16_t *qmeta = NULL;
    size_t store = 0;
    size_t qname_len = strlen((char *) question->qname);

    // Iterate over qname
    for (size_t i = 1; i <= qname_len; i++) {
        if (question->qname[i-1] == '.') {
            buf[store] = count;
            store = i;
            count = 0;
            continue;
        }

        buf[i] = question->qname[i-1];
        count++;
    }

    buf[store] = count;

    qmeta = (uint16_t *) (buf + store + count + 3);
    qmeta[0] = question->qtype;
    qmeta[1] = question->qclass;
}

void DNSResource_null_a(DNSResource *resource) {
    // Use a pointer
    resource->name = (uint8_t *) "\x0c\xc0";
    resource->type = TYPE_A;
    resource->class_ = CLASS_IN;
    resource->ttl = 60;
    resource->rdlength = 4;
    resource->rdata = (uint8_t *) "\x00\x00\x00\x00"; // 0.0.0.0
}

void DNSResource_null_aaaa(DNSResource *resource) {
    // Use a pointer
    resource->name = (uint8_t *) "\x0c\xc0";
    resource->type = TYPE_AAAA;
    resource->class_ = CLASS_IN;
    resource->ttl = 60;
    resource->rdlength = 2;
    resource->rdata = (uint8_t *) "::"; // ::
}

int DNSResource_to(const DNSResource *resource, char *buf) {
    uint16_t *rmeta = (uint16_t *) buf;

    if (
           resource->type != TYPE_A
        && resource->type != TYPE_AAAA
    ) {
        fprintf(
            stderr,
            "Error: Invalid resource type: %d\n",
            resource->type
        );
        return 1;
    }

    //rmeta[0] = htons((uint16_t) (3 << 14) | 12);
    rmeta[0] = htons(*(uint16_t *) resource->name);
    rmeta[1] = htons(resource->type);
    rmeta[2] = htons(resource->class_);
    *(uint32_t *) (rmeta + 3) = htonl(resource->ttl);
    rmeta[5] = htons(resource->rdlength);

    if (resource->type == TYPE_A) {
        buf[12] = resource->rdata[0];
        buf[13] = resource->rdata[1];
        buf[14] = resource->rdata[2];
        buf[15] = resource->rdata[3];

    } else if (resource->type == TYPE_AAAA) {
        buf[12] = resource->rdata[0];
        buf[13] = resource->rdata[1];
    }

    return 0;
}

int DNSResponse_from(DNSResponse *response, const DNSQuery *query) {
    response->header = query->header;
    response->question = query->question;

    // This is a response
    DNSHeader_set_qr(response->header);

    // Authoritative answer
    DNSHeader_set_aa(response->header);

    // 1 resource in answer
    response->header.ancount = 1;

    switch (query->question.qtype) {
        case TYPE_A:
            DNSResource_null_a(&(response->resource));
            break;

        // TODO: Fix this
        // case TYPE_AAAA:
        //     DNSResource_null_aaaa(&(response->resource));
        //     break;

        default:
            fprintf(
                stderr,
                "Error: Unexpected QTYPE: %d\n",
                query->question.qtype
            );
            return 1;
    }

    return 0;
}

int DNSResponse_to(
    const DNSResponse *response,
    char *buf,
    const size_t buf_len
) {
    int result;
    size_t min_required = 12;
    size_t qname_len = strlen((char *) response->question.qname);
    uint16_t *hptr = (uint16_t *) buf;

    // Header requires:
    //  12 bytes
    //
    // Question requires:
    //  len(qname) + 1 (for null) + 1 (for first count)
    //  + 4 bytes (for qtype + qclass)
    //
    // Resource requires:
    //  2 bytes (for pointer to name)
    //  10 bytes for type, class, ttl and rdlength
    //  4 bytes (for A) or 2 bytes (for AAAA)

    min_required += (qname_len + 2) + 4 + 12;

    if (response->question.qtype == TYPE_A) {
        min_required += 4;
    } else if (response->question.qtype == TYPE_AAAA) {
        min_required += 2;
    } else {
        fprintf(
            stderr,
            "Error: Unsupported QTYPE for DNSResponse_to: %d\n",
            response->question.qtype
        );
        return -1;
    }

    // Check minimum length
    if (buf_len < min_required) {
        fprintf(
            stderr,
            "Error: Buffer too small for storing response:\n"
            "Minimum: %zu\n"
            "Actual:  %zu\n",
            min_required,
            buf_len
        );
        return -1;
    }

    // Write header
    hptr[0] = htons(response->header.id);
    hptr[1] = htons(response->header._meta);
    hptr[2] = htons(response->header.qdcount);
    hptr[3] = htons(response->header.ancount);
    hptr[4] = htons(response->header.nscount);
    hptr[5] = htons(response->header.arcount);

    // Write the question to the buffer
    DNSQuestion_to(
        &(response->question),
        // Skip header
        buf + 12
    );

    // Write the resource record to the buffer
    result = DNSResource_to(
        &(response->resource),
        // Skip header and question
        buf + 12 + (qname_len + 1) + 5
    );

    if (result != 0) return -1;

    return min_required;
}
