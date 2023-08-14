#include "dale.h"

void handle(Socks *socks) {
    ssize_t count;
    char query[MAX_UDP << 1] = {0};
    char resp[MAX_UDP << 1] = {0};

    // Receive query from client (downstream)
    count = get_query(
        socks,
        query, MAX_UDP + 1
    );

    // Check for failure
    if (count <= 0) return;

    // TODO: Check query
    // if (check_query(query, count) != 0) return;

    // Forward upstream
    count = forward_query(
        socks,
        query, count,
        resp, MAX_UDP + 1
    );

    // Check for failure
    if (count <= 0) return;

    // Forward response back to client (downstream)
    forward_response(
        socks,
        resp, count
    );
}

void start(Socks socks) {
    for (;;) {
        handle(&socks);
    }
}

int main(int argc, char *argv[]) {
    Socks socks;
    Args args = Args_default;
    pid_t pid;

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
    if (args.daemon == true) {
        pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        // Child process
        if (pid == 0) {
            setsid();
            start(socks);
        }
    } else {
        start(socks);
    }
}
