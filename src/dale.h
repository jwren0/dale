#ifndef DALE_DALE_H
#define DALE_DALE_H

/**
 * A file containing core functionality.
 *
 * @author  jwren0
 * @version 2023-08-13
 */

#include <stdio.h>
#include "args.h"
#include "dns/query.h"
#include "server.h"

#define VERSION "v0.0.1"

/**
 * Handles a single DNS query from downstream.
 *
 * @param socks The socket information to use.
 */
void handle(Socks *socks);

/**
 * Simply the main function.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The exit status of the program
 */
int main(int argc, char *argv[]);

#endif // DALE_DALE_H
