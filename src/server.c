#include "server.h"

void Sock_init(
    Sock *sock,
    const char *host,
    const unsigned int port
) {
    sock->fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Check for errors
    if (sock->fd < 0) {
        perror("socket");
        exit(1);
    }

    sock->info.sin_family = AF_INET;
    sock->info.sin_port = htons(port);
    sock->info.sin_addr.s_addr = inet_addr(host);

    // Check for errors
    if (sock->info.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "Invalid hostname: %s\n", host);
        exit(1);
    }
}

void Socks_init(Socks *socks, Args *args) {
    int result;

    Sock_init(&(socks->down), args->lhost, args->lport);
    Sock_init(&(socks->up), args->rhost, args->rport);

    // Bind listener
    result = bind(
        socks->down.fd,
        (struct sockaddr *) &(socks->down.info),
        sizeof(socks->down.info)
    );

    // Check for errors
    if (result < 0) {
        perror("bind");
        exit(1);
    }

    printf(
        "Started listening on %s:%d\n",
        args->lhost,
        args->lport
    );
}

ssize_t up_forward(
    Sock *up,
    const char *query,
    const size_t query_len,
    char *resp,
    const size_t resp_len
) {
    // TODO: Actually validate count
    ssize_t count;

    puts("Forwarding query to upstream");
    count = sendto(
        up->fd,
        query, query_len,
        0,
        (struct sockaddr *) &(up->info),
        sizeof(up->info)
    );
    printf("Forwarded query to upstream: %zd\n", count);

    // TODO: Validate this response maybe
    puts("Waiting for response from upstream...");
    count = recvfrom(
        up->fd,
        resp, resp_len,
        0,
        NULL, NULL
    );
    printf("Received response from upstream: %zd\n", count);

    return count;
}

int down_forward(
    Sock *sock,
    const char *resp,
    const size_t resp_len,
    struct sockaddr down,
    const socklen_t down_len
) {
    ssize_t count;

    // TODO: Validate count
    puts("Forwarding response back downstream...");
    count = sendto(
        sock->fd,
        resp, resp_len,
        0,
        &down, down_len
    );
    printf("Forwarded response downstream: %zd\n", count);

    return 0;
}
