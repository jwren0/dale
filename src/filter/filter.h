#ifndef DALE_FILTER_FILTER_H
#define DALE_FILTER_FILTER_H

/**
 * A file containing functionality for
 * filtering DNS queries.
 *
 * @author  jwren0
 * @version 2023-08-15
 */

#include <stdio.h>
#include <string.h>
#include "../net/dns/query.h"

/**
 * Test function for checking if queries can
 * be filtered.
 *
 * @param query The query to check.
 * @return zero if the query should be filtered,
 *         a negative value if an error occured,
 *         a positive value otherwise.
 */
int filter_query(const DNSQuery *query);

#endif // DALE_FILTER_FILTER_H
