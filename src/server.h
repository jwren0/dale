#ifndef DALE_SERVER_H
#define DALE_SERVER_H

/**
 * A file containing functionality
 * used for downstream and upstream communication.
 *
 * @author  jwren0
 * @version 2023-08-13
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "args.h"

/**
 * The maximum size of a UDP message as defined
 * by RFC 1035 section 2.3.4.
 */
#define MAX_UDP 512

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
    Sock down;
    Sock up;
} Socks;

/**
 * Initializes a Socks struct.
 *
 * @param socks The Socks struct to initialize.
 * @param args The arguments to use for initialization.
 */
void Socks_init(Socks *socks, Args *args);

/**
 * Forward a query upstream and get a response back.
 *
 * @param sock The upstream server's Sock struct.
 * @param query The query to forward.
 * @param query_len The size of the query.
 * @param resp Where to store the response.
 * @param resp_len The maximum size of the response.
 * @return zero on success, non-zero otherwise.
 */
ssize_t up_forward(
    Sock *sock,
    const char *query,
    const size_t query_len,
    char *resp,
    const size_t resp_len
);

/**
 * Forward a response downstream.
 *
 * @param sock The Sock struct for downstream traffic.
 * @param resp The response to forward.
 * @param resp_len The length of the response.
 * @param down The downstream client's sockaddr struct.
 * @param down_len The length of the client's sockaddr struct.
 */
int down_forward(
    Sock *sock,
    const char *resp,
    const size_t resp_len,
    struct sockaddr down,
    const socklen_t down_len
);

#endif // DALE_SERVER_H
