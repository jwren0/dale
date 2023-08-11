#ifndef DALE_ARGS_H
#define DALE_ARGS_H

/**
 * A file containing structs and functions for handling
 * command line arguments passed to dale.
 *
 * @author  jwren0
 * @version 2023-08-11
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * A struct storing information about the
 * arguments passed to dale.
 */
typedef struct {
    /**
     * Whether to run as a daemon (in the background)
     * or in the foreground.
     */
    bool daemon;

    /**
     * The address to listen on.
     */
    char *lhost;

    /**
     * The port to bind to.
     */
    unsigned int lport;

    /**
     * The upstream host of the resolver to forward
     * queries to (if not filtered).
     */
    char *rhost;

    /**
     * The port the upstream resolver listens on.
     */
    unsigned int rport;
} Args;

/**
 * Initializes a Args struct.
 *
 * @param args The Args struct to initialize.
 */
#define Args_default \
{ \
    .daemon = false, \
    .lhost = "127.0.0.1", \
    .lport = 53, \
    .rhost = NULL, \
    .rport = 53, \
}

/**
 * Displays the help menu for dale.
 *
 * @param program The program name.
 */
void Args_help(char *program);

/**
 * Converts a char[] input into a port number if valid.
 *
 * @param port Where to store the converted value.
 * @param input The buffer to read a port from.
 * @return zero on success, non-zero otherwise
 */
int Args_store_port(unsigned int *port, char *input);

/**
 * Parses command line arguments into an Args struct.
 *
 * @param args The Args struct to parse into.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return zero on success, non-zero otherwise.
 */
void Args_parse(Args *args, int argc, char *argv[]);

#endif // DALE_ARGS_H
