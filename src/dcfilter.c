#include <stdlib.h>
#include <stdio.h>

#include <graphviz/cgraph.h>

#include <getopt.h>

static FILE *inFile;
static FILE *outFile;
static int Verbose = 0;
static char *cmd;


#define UNREACHABLE()                                                          \
  do {                                                                         \
    fprintf(stderr, "%s:%d: claimed unreachable code was reached", __FILE__,   \
            __LINE__);                                                         \
    abort();                                                                   \
  } while (0)


static inline FILE *openFile(
    const char *argv0,
    const char *name,
    const char *mode)
{
    FILE *fp = fopen(name, mode);
    if (fp == NULL) {
        const char *modestr = strcmp(mode, "r") == 0 ? "reading" : "writing";
        fprintf(stderr, "%s: could not open file %s for %s\n", argv0, name,
                modestr);
        perror(name);
        return NULL;
    }
    return fp;
}


static void init(int argc, char *argv[])
{
    int c;

    cmd = argv[0];
    opterr = 0;
    while ((c = getopt(argc, argv, ":vno:")) != -1)
    switch (c) {
    case 'o':
        if (outFile != NULL)
        fclose(outFile);
        outFile = openFile(argv[0], optarg, "w");
        break;
    case 'v':
        Verbose = 1;
        break;
    default:
        UNREACHABLE();
    }

    if (optind < argc) {
        inFile = openFile(argv[0], argv[optind], "r");
    } else {
        inFile = stdin;
    }
    if (!outFile) {
        outFile = stdout;
    }
}


int main(int argc, char *argv[])
{
    Agraph_t *g;
    Agnode_t *n;

    init(argc, argv);

    if ((g = agread(inFile, NULL)) == NULL) {
        return -1;
    }

    if (!agisdirected (g)) {
        if (Verbose)
            fprintf(stderr, "Graph \"%s\" is undirected\n", agnameof(g));
        return -1;
    }

    return 0;
}