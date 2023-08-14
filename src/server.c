#include "server.h"
#include "arpa/inet.h"

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

ssize_t get_query(
    Socks *socks,
    char *query,
    const size_t query_len
) {
    ssize_t count;
    socklen_t client_len = sizeof(socks->client);

    // Receive a query from downstream
    count = recvfrom(
        socks->down.fd,
        query, query_len,
        0,
        &(socks->client), &client_len
    );

    check_recv(count);
    if (count <= 0) return count;

    // Check for truncation
    if (client_len > sizeof(socks->client)) {
        fprintf(
            stderr,
            "Error: Client's address was truncated.\n"
        );
        return -1;
    }

    return count;
}

ssize_t forward_query(
    Socks *socks,
    const char *query,
    const size_t query_len,
    char *resp,
    const size_t resp_len
) {
    // For validating response address
    const size_t MAX_IP = 16;
    char expect_ip[MAX_IP+1];
    char check_ip[MAX_IP+1];
    char *tmp = NULL;

    struct sockaddr_in check_addr;
    socklen_t check_len = sizeof(check_addr);
    ssize_t count;

    // Initialize buffers
    memset(expect_ip, 0, MAX_IP+1);
    memset(check_ip, 0, MAX_IP+1);

    // Save expected IP
    tmp = inet_ntoa(socks->up.info.sin_addr);
    strncpy(expect_ip, tmp, MAX_IP);

    // Forward query upstream
    count = sendto(
        socks->up.fd,
        query, query_len,
        0,
        (struct sockaddr *) &(socks->up.info),
        sizeof(socks->up.info)
    );

    check_send(count);
    if (count <= 0) return count;

    printf(
        "Forwarded query upstream: %s\n",
        expect_ip
    );

    // Receive a response from upstream
    count = recvfrom(
        socks->up.fd,
        resp, resp_len,
        0,
        (struct sockaddr *) &check_addr, &check_len
    );

    check_recv(count);

    // Validate response address
    // Save response IP
    tmp = inet_ntoa(check_addr.sin_addr);
    strncpy(check_ip, tmp, MAX_IP);

    printf(
        "Received response from upstream: %s\n",
        check_ip
    );

    if (strncmp(expect_ip, check_ip, MAX_IP+1) != 0) {
        fprintf(
            stderr,
            "Expected response IP from upstream resolver is mismatched:\n"
            "Expected: %s\n"
            "Actual:   %s\n",
            expect_ip, check_ip
        );

        return -1;
    }

    return count;
}

void forward_response(
    Socks *socks,
    const char *resp,
    const size_t resp_len
) {
    ssize_t count;

    // Send the response downstream
    count = sendto(
        socks->down.fd,
        resp, resp_len,
        0,
        &(socks->client), sizeof(socks->client)
    );

    check_send(count);

    if (count > 0) {
        printf("Forwarded response downstream\n");
    }
}
