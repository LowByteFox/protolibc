/* $NetBSD: getopt.c,v 1.29 2014/06/05 22:00:22 christos Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	BADCH '?'
#define	BADARG ':'

static char EMSG[] = "";

char *optarg;
int opterr = 1;
int optind = 1;
int optopt;
int optreset;

int getopt(int argc, char *const *argv, const char *optstring)
{
    static char *place = EMSG;
    char *oli;

    if (optreset || *place == 0) {
        optreset = 0;
        place = argv[optind];
        if (optind >= argc || *place++ != '-') {
            place = EMSG;
            return -1;
        }

        optopt = *place++;
        if (optopt == '-' && *place == 0) {
            optind++;
            place = EMSG;
            return -1;
        }

        if (optopt == 0) {
            place = EMSG;
            if (strchr(optstring, '-') == NULL)
                return -1;
            optopt = '-';
        }
    } else
        optopt = *place++;

    if (optopt == ':' || (oli = strchr(optstring, optopt)) == NULL) {
        if (*place == 0)
            optind++;
        if (opterr && *optstring != ':')
            fprintf(stderr, "%s: illegal option -- %c\n", getprogname(),
                optopt);
        return BADCH;
    }

    if (oli[1] != ':') {
        optarg = NULL;
        if (*place == 0)
            optind++;
    } else {
        if (*place)
            optarg = place;
        else if (oli[2] == ':')
            optarg = NULL;
        else {
            place = EMSG;
            if (*optstring == ':')
                return BADARG;
            if (opterr)
                fprintf(stderr, "%s: option requires an argument -- %c\n",
                    getprogname(), optopt);
            return BADCH;
        }
        place = EMSG;
        optind++;
    }

    return optopt;
}
