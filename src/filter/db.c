#include "db.h"

PGconn *db_connect(void) {
    const char *keywords[] = {
        "hostaddr",
        "port",
        "user",
        "password",
        "dbname",
        NULL,
    };

    const char *values[] = {
        "127.0.0.1",
        "5432",
        "dale",
        "password",
        "daledb",
        NULL,
    };

    PGconn *conn = PQconnectdbParams(
        keywords, values, 0
    );

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(
            stderr,
            "Error: Failed connecting to the database:\n%s\n",
            PQerrorMessage(conn)
        );
        exit(1);
    }

    return conn;
}
