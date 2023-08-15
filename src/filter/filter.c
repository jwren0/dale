#include "filter.h"

int filter_query(const DNSQuery *query) {
    // Check for filter
    if (!strncmp(
        "google.com",
        (char *) query->question.qname,
        strlen("google.com") + 1
    )) {
        printf("Filtering domain: %s\n", query->question.qname);
        return 0;
    }

    return 1;
}
