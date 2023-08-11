#include "structs.h"

int DNSQuestion_init(DNSQuestion *question, uint8_t *buf) {
    int i, result = 1;
    uint8_t count = 0;
    uint16_t *qmeta = NULL;

    // Fail if nulls provided
    if (question == NULL || buf == NULL) return 1;

    // Initialize QNAME
    memset(question->qname, 0, MAX_NAME + 1);

    // Read first count
    count = buf[0];
    buf++;

    // Iterate over the buffer
    for (i = 0; i < MAX_NAME; i++) {
        // If this byte is null, stop successfully
        if (buf[i] == 0) {
            result = 0;
            break;
        }

        // If count is 0, use this octet as new
        // count
        if (count == 0) {
            count = buf[i];

            // If count is greater than MAX_LABEL, fail
            if (count > MAX_LABEL) return 1;

            question->qname[i] = '.';
            continue;
        }

        // Otherwise, copy this octet and
        // decrement count
        question->qname[i] = buf[i];
        count--;
    }

    // Return early if result is non-zero
    if (result != 0) {
        return result;
    }

    // buf should now be pointing at the end of
    // QNAME, increment once, then store QTYPE and QCLASS
    buf++;
    qmeta = (uint16_t *) buf;

    question->qtype = ntohs(qmeta[0]);
    question->qclass = ntohs(qmeta[1]);

    return 0;
}

int DNSHeader_init(DNSHeader *header, uint8_t *buf) {
    uint16_t *hptr = (uint16_t *) buf;

    if (header == NULL || buf == NULL) return 1;

    // Interpret the first 12 bytes of buf as a header
    header->id = ntohs(hptr[0]);
    header->_meta = ntohs(hptr[1]);

    // TODO: Validate that QDCOUNT is 1
    header->qdcount = ntohs(hptr[2]);
    header->ancount = ntohs(hptr[3]);
    header->nscount = ntohs(hptr[4]);
    header->arcount = ntohs(hptr[5]);

    return 0;
}

int DNSQuery_init(DNSQuery *query, uint8_t *buf) {
    if (query == NULL || buf == NULL) return 1;

    if (DNSHeader_init(&(query->header), buf) != 0) {
        fprintf(stderr, "ERROR: Malformed header\n");
        return 1;
    }

    if (DNSQuestion_init(&(query->question), buf + 12) != 0) {
        fprintf(stderr, "ERROR: Malformed question section\n");
        return 1;
    }

    return 0;
}
