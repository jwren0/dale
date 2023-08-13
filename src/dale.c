#include "dale.h"

void handle(Socks *socks) {
    char buf[MAX_UDP << 1] = {0};
    char resp[MAX_UDP << 1] = {0};
    ssize_t count;
    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    DNSQuery query;

    count = recvfrom(
        socks->down.fd,
        buf, MAX_UDP + 1,
        0,
        &addr, &len
    );

    printf("Received query: %zu\n", count);

    // TODO: Maybe send something useful back?
    // Check failure
    if (count < 0) {
        perror("recvfrom");
        return;
    }
    // Check nothing was received
    if (count == 0) {
        fprintf(stderr, "Error: Received nothing, skipping...\n");
        return;
    }
    // Check malformed UDP
    if (count > MAX_UDP) {
        fprintf(stderr, "Error: Malformed UDP query received, skipping...\n");
        return;
    }
    // Check for truncation
    if (len > sizeof(addr)) {
        fprintf(stderr, "Error: Sender's address was truncated, skipping...\n");
        return;
    }
    // Check for malformed query
    if (DNSQuery_init(&query, buf) != 0) {
        fprintf(stderr, "Error: Malformed DNS query received, skipping...\n");
        return;
    }

    printf(
        "Successfully parsed query:\n"
        "ID:    %d\n"
        "QNAME: %s\n",
        query.header.id,
        query.question.qname
    );

    count = up_forward(
        &(socks->up),
        buf, count,
        resp, MAX_UDP
    );

    puts("Here is the response:\n");

    for (ssize_t i = 0; i < count; i++) {
        printf("%#x ", resp[i]);
    }

    puts("");

    down_forward(
        &(socks->down),
        resp, count,
        addr, len
    );
}

int main(int argc, char *argv[]) {
    Socks socks;
    Args args = Args_default;

    // Ignore malformed args
    if (argc < 1) {
        fprintf(stderr, "argc must be at least 1\n");
        return 1;
    }

    // Show help by default
    if (argc < 2) {
        Args_help(argv[0]);
        return 0;
    }

    Args_parse(&args, argc, argv);

    // Initialize sockets, bind and listen
    Socks_init(&socks, &args);

    // Start handling queries
    for (;;) {
        handle(&socks);
    }
}
