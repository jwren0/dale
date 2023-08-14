#include "filter.h"

int filter_query(const char *buf) {
    DNSQuery query;

    if (DNSQuery_init(&query, buf) != 0) {
        fprintf(stderr, "Failed parsing query\n");
        return -1;
    }

    // Check for filter
    if (!strncmp(
        "google.com",
        (char *) query.question.qname,
        strlen("google.com") + 1
    )) {
        printf("Filtering domain: %s\n", query.question.qname);
        return 0;
    }

    return 1;
}