#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <graphviz/cgraph.h>

#include <getopt.h>

#include "bit_array/bit_array.h"


static FILE *inFile;
static FILE *outFile;
static int doWrite = 1;
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


static char *useString = "Usage: %s [-nv?] [-o outfile] <file>\n\
  -o <file> - put output in <file>\n\
  -n        - do not output graph\n\
  -v        - verbose\n\
  -?        - print usage\n";


static void usage(int v)
{
    fprintf(stderr, useString, cmd);
    exit(v);
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
    case 'n':
        doWrite = 0;
        break;
    case 'v':
        Verbose = 1;
        break;
    case '?':
        if (optopt == '?')
            usage(0);
        else {
            fprintf(stderr, "%s: option -%c unrecognized\n", cmd, optopt);
            usage(-1);
        }
        break;
    case ':':
        fprintf(stderr, "%s: missing argument for option -%c\n",
            cmd, optopt);
        usage(-1);
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


struct DependenciesTable
{
    BIT_ARRAY *deps;
    Agnode_t *node;
};
typedef struct DependenciesTable DependenciesTable;


int main(int argc, char *argv[])
{
    Agraph_t *g;
    Agnode_t *n;
    Agedge_t *e;

    init(argc, argv);

    if ((g = agread(inFile, NULL)) == NULL) {
        return -1;
    }

    if (!agisdirected (g)) {
        if (Verbose)
            fprintf(stderr, "Graph \"%s\" is undirected\n", agnameof(g));
        return -1;
    }

    int nodes_count = agnnodes(g);

    if (Verbose)
        fprintf(stderr, "Found %d nodes\n", nodes_count);

    DependenciesTable *table = calloc(nodes_count, sizeof(DependenciesTable));

    int idx = 0;
    // fill nodes in table
    for (n = agfstnode(g); n; n = agnxtnode(g, n), ++idx)
    {
        assert(idx < nodes_count);
        table[idx].node = n;
        table[idx].deps = NULL;
    }

    assert(idx == nodes_count);
    BIT_ARRAY *modified = bit_array_create(nodes_count);

    // fill dependencies if any exists for node
    for (int i = 0; i < nodes_count; ++i)
    {
        assert(NULL == table[i].deps);

        for (e = agfstin(g, table[i].node); e; e = agnxtin(g, e))
        {
            if (NULL == table[i].deps)
            {
                table[i].deps = bit_array_create(nodes_count);
            }

            if (NULL != table[i].deps)
            {
                for (int j = 0; j < nodes_count; ++j)
                {
                    if (table[j].node == e->node)
                    {
                        bit_array_set(table[i].deps, j);
                        bit_array_set(modified, i);

                        if (table[j].deps)
                        {
                            bit_array_or(table[i].deps, table[i].deps, table[j].deps);
                        }
                    }
                }
            }
        }
    }

    BIT_ARRAY *zero = bit_array_create(nodes_count);

    // Add transitive dependencies
    while(!bit_array_eq(modified, zero))
    {
        for (int i = 0; i < nodes_count; ++i)
        {
            if (bit_array_get(modified, i))
            {
                for (int j = 0; j < nodes_count; ++j)
                {
                    if (i != j && table[j].deps && table[j].deps)
                    {
                        if (bit_array_get(table[j].deps, i))
                        {
                            BIT_ARRAY *or_res = bit_array_create(nodes_count);
                            bit_array_or(or_res, table[i].deps, table[j].deps);

                            if (!bit_array_eq(or_res, table[j].deps))
                            {
                                bit_array_set(modified, j);
                            }

                            bit_array_free(table[j].deps);
                            table[j].deps = or_res;
                        }
                    }
                }

                bit_array_unset(modified, i);
            }
        }
    }

    // If node does not depend on itself, than it does not enters any cycle.
    for (int i = 0; i < nodes_count; ++i)
    {
        if (NULL == table[i].deps || !bit_array_get(table[i].deps, i))
        {
            agdelnode(g, table[i].node);
        }
    }

    if (doWrite) {
        agwrite(g, outFile);
        fflush(outFile);
    }

    for (int i = 0; i < nodes_count; ++i)
    {
        if (NULL != table[i].deps)
        {
            bit_array_free(table[i].deps);
        }
    }

    bit_array_free(modified);
    free(table);
    return 0;
}
