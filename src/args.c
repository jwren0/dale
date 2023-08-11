#include "args.h"

#define VERSION "v0.0.1"

void Args_help(char *program) {
    printf(
        "%s, %s\n"
        "\nUsage:\n"
        "  %s [options...]\n"
        "\nOptions:\n"
        "  -? --help          Display this help menu\n"
        "  -b --background    Run in the background                   (Default)\n"
        "  -f --foreground    Run in the foreground\n"
        "  -h --lhost <ip>    The address to listen on                (Default: 127.0.0.1)\n"
        "  -p --lport <port>  The port to listen on                   (Default: 53)\n"
        "  -H --rhost <ip>    The upstream host to forward queries to\n"
        "  -P --rport <port>  The port the upstream host listens on   (Default: 53)\n",
        program, VERSION, program
    );
}

int Args_store_port(unsigned int *port, char *input) {
    char *endptr = NULL;
    long result;

    result = strtol(input, &endptr, 10);

    // Check for failure
    if (endptr == input
        || result <= 0
        || endptr < input + strlen(input)
        || result >= 1 << 16
    ) {
        fprintf(stderr, "Invalid port: '%s'\n", input);
        return 1;
    }

    // Store port on success
    *port = result;

    return 0;
}

void Args_parse(Args *args, int argc, char *argv[]) {
    char *arg = NULL;

    // Start at 1 to skip the program name
    for (int i = 1; i < argc; i++) {
        // Current argument
        arg = argv[i];

        // -?/--help
        if (
               !strncmp("-?", arg, strlen("-?") + 1)
            || !strncmp("--help", arg, strlen("--help") + 1)
        ) {
            Args_help(argv[0]);
            exit(0);
        }
        // -b/--background
        else if (
               !strncmp("-b", arg, strlen("-b") + 1)
            || !strncmp("--background", arg, strlen("--background") + 1)
        ) {
            args->daemon = true;
        }
        // -f/--foreground
        else if (
               !strncmp("-f", arg, strlen("-f") + 1)
            || !strncmp("--foreground", arg, strlen("--foreground") + 1)
        ) {
            args->daemon = false;
        }
        // -h/--lhost
        else if (
               !strncmp("-h", arg, strlen("-h") + 1)
            || !strncmp("--lhost", arg, strlen("--lhost") + 1)
        ) {
            // Ensure argc is large enough
            if (i + 1 >= argc) {
                fprintf(stderr, "'%s' is missing an argument\n", arg);
                Args_help(argv[0]);
                exit(1);
            }

            args->lhost = argv[++i];
        }
        // -p/--lport
        else if (
               !strncmp("-p", arg, strlen("-p") + 1)
            || !strncmp("--lport", arg, strlen("--lport") + 1)
        ) {
            // ensure argc is large enough
            if (i + 1 >= argc) {
                fprintf(stderr, "'%s' is missing an argument\n", arg);
                Args_help(argv[0]);
                exit(1);
            }

            // check for conversion failures
            if (Args_store_port(&(args->lport), argv[++i]) != 0) {
                Args_help(argv[0]);
                exit(1);
            }
        }
        // -H/--rhost
        else if (
               !strncmp("-H", arg, strlen("-H") + 1)
            || !strncmp("--rhost", arg, strlen("--rhost") + 1)
        ) {
            // Ensure argc is large enough
            if (i + 1 >= argc) {
                fprintf(stderr, "'%s' is missing an argument\n", arg);
                Args_help(argv[0]);
                exit(1);
            }

            args->rhost = argv[++i];
        }
        // -P/--rport
        else if (
               !strncmp("-P", arg, strlen("-P") + 1)
            || !strncmp("--rport", arg, strlen("--rport") + 1)
        ) {
            // ensure argc is large enough
            if (i + 1 >= argc) {
                fprintf(stderr, "'%s' is missing an argument\n", arg);
                Args_help(argv[0]);
                exit(1);
            }

            // check for conversion failures
            if (Args_store_port(&(args->rport), argv[++i]) != 0) {
                Args_help(argv[0]);
                exit(1);
            }
        }
        // Anything else
        else {
            fprintf(stderr, "Invalid argument: '%s'\n", arg);
            Args_help(argv[0]);
            exit(1);
        }
    }
}
