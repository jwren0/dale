#ifndef DALE_NET_SERVER_H
#define DALE_NET_SERVER_H

/**
 * A file containing functionality
 * used for downstream and upstream communication.
 *
 * @author  jwren0
 * @version 2023-08-14
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "../args.h"

/**
 * The maximum size of a UDP message as defined
 * by RFC 1035 section 2.3.4.
 */
#define MAX_UDP 512

/**
 * Checks the count returned from recvfrom.
 */
#define check_recv(count) { \
    if (count < 0) { \
        perror("recvfrom"); \
    } else if (count == 0) { \
        fprintf(stderr, "Error: Received nothing, skipping...\n"); \
    } \
}

/**
 * Checks the count returned from sendto.
 */
#define check_send(cout) { \
    if (count < 0) { \
        perror("sendto"); \
    } else if (count == 0) { \
        fprintf(stderr, "Error: Sent nothing\n"); \
    } \
}

/**
 * A struct holding information about
 * a socket used for communication.
 */
typedef struct {
    /**
     * The file descriptor of the socket.
     */
    int fd;

    /**
     * Information about this socket.
     */
    struct sockaddr_in info;
} Sock;

/**
 * Initializes a Sock struct.
 *
 * @param sock The Sock struct to initialize.
 * @param host The host this socket should use.
 * @param port The port this socket should use.
 */
void Sock_init(
    Sock *sock,
    const char *host,
    const unsigned int port
);

/**
 * A struct containing the sockets
 * used for communication.
 */
typedef struct {
    /**
     * Information about the socket listening for
     * DNS queries from a client (downstream).
     */
    Sock down;

    /**
     * Information about the socket forwarding
     * queries to an upstream resolver and
     * receiving responses.
     */
    Sock up;

    /**
     * Information about the client a DNS query
     * is being handled for.
     */
    struct sockaddr client;
} Socks;

/**
 * Initializes a Socks struct.
 *
 * @param socks The Socks struct to initialize.
 * @param args The arguments to use for initialization.
 */
void Socks_init(Socks *socks, Args *args);

/**
 * Receives a query from a cient.
 *
 * Updates the Socks struct with the client information.
 *
 * @param socks The Socks struct to use.
 * @param query The buffer to store the query in.
 * @param query_len The maximum query size expected.
 * @return The number of bytes received. <= 0 on failure.
 */
ssize_t get_query(
    Socks *socks,
    char *query,
    const size_t query_len
);

/**
 * Forwards a query to the upstream resolver.
 *
 * @param socks The Socks struct to use.
 * @param query The query to forward.
 * @param query_len The size of the query.
 * @param resp The buffer to store the response in.
 * @param resp_len The maximum response size expected.
 * @return The number of bytes written to resp. <= 0 on failure.
 */
ssize_t forward_query(
    Socks *socks,
    const char *query,
    const size_t query_len,
    char *resp,
    const size_t resp_len
);

/**
 * Forwards a response back to the client.
 *
 * @param socks The Socks struct to use.
 * @param resp The response to forward.
 * @param resp_len The size of the response
 */
void forward_response(
    Socks *socks,
    const char *resp,
    const size_t resp_len
);

#endif // DALE_NET_SERVER_H
