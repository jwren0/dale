#include "structs.h"

int DNSQuestion_init(DNSQuestion *question, uint8_t *buf) {
    int i, result = 1;
    uint8_t count = 0;
    uint16_t *qmeta = NULL;

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
            // TODO: Validate label length
            count = buf[i];
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

void DNSHeader_init(DNSHeader *header, uint8_t *buf) {
    uint16_t *hptr = (uint16_t *) buf;

    if (header == NULL || buf == NULL) return;

    // Interpret the first 12 bytes of buf as a header
    header->id = ntohs(hptr[0]);
    header->meta = ntohs(hptr[1]);

    // TODO: Validate that QDCOUNT is 1
    header->qdcount = ntohs(hptr[2]);
    header->ancount = ntohs(hptr[3]);
    header->nscount = ntohs(hptr[4]);
    header->arcount = ntohs(hptr[5]);
}

int DNSQuery_init(DNSQuery *query, uint8_t *buf) {
    if (query == NULL || buf == NULL) return 1;

    // The rest of buf should be a question section
    DNSHeader_init(&(query->header), buf);

    if (DNSQuestion_init(&(query->question), buf + 12) != 0) {
        fprintf(stderr, "ERROR: Malformed QNAME\n");
        return 1;
    }

    return 0;
}
