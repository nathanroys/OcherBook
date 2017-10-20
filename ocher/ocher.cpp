/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/settings/Options.h"
#include "ocher/ux/Controller.h"

#include <exception>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void usage(const char *msg)
{
    printf(
            "OcherBook  Copyright (C) 2015 Chuck Coffing  <clc@alum.mit.edu>\n"
            "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
            "\n");
    if (msg) {
        printf("%s\n\n", msg);
    }
    printf(
            // 12345678901234567890123456789012345678901234567890123456789012345678901234567890
            "Usage:  ocher [OPTIONS]... [FILE]...\n"
            "\n"
            "-h,--help              This help.\n"
            "-v,--verbose           Increase logging verbosity.\n"
            "-q,--quiet             Quiet; decrease logging verbosity.\n"
            "   --key <key>=<value> Set the global <key> to <value>\n"
            "\n"
            "Multiple files and/or directories may be specified, and will override any\n"
            "platform specific search paths.  Directories will be searched recursively.\n"
            "\n"
            );
    exit(msg ? 0 : 1);
}

#define OPT_KEY 256

int main(int argc, char **argv)
{
    Options *opt = new Options;

    struct option long_options[] =
    {
        { "help",         no_argument,       0, 'h' },
        { "quiet",        no_argument,       0, 'q' },
        { "verbose",      no_argument,       0, 'v' },
        { "key",          required_argument, 0, OPT_KEY },
        { 0, 0, 0, 0 }
    };

    while (1) {
        // getopt_long stores the option index here.
        int option_index = 0;

        int ch = getopt_long(argc, argv, "d:bfhtvq", long_options, &option_index);
        if (ch == -1)
            break;
        switch (ch) {
        case 0:
            break;
        case 'v':
            opt->verbose++;
            break;
        case 'q':
            opt->verbose--;
            break;
        case 'h':
            usage(0);
            break;
        case OPT_KEY:
        {
            char *split = strchr(optarg, '=');
            if (split) {
                *split = 0;
                opt->keys[optarg] = split + 1;
            } else {
                opt->keys[optarg] = "";
            }
            break;
        }
        default:
            usage("Unknown argument");
            break;
        }
    }

    if (optind < argc) {
        opt->files = (const char **)&argv[optind];
    }

    try {
        Controller c(opt);

        c.run();
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    }

    return 0;
}
