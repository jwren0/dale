#ifndef DALE_FILTER_DB_H
#define DALE_FILTER_DB_H

/**
 * A file containing functionality
 * for interacting with the database.
 *
 * @author  jwren0
 * @version 2023-08-15
 */

#include <stdlib.h>
#include <libpq-fe.h>

PGconn *db_connect(void);

#endif // DALE_FILTER_DB_H
