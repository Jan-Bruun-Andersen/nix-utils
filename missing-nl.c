/**********************************************************************************\
| DESCRIPTION
|   A quick and dirty tool to quickly check for missing NL's in (text) files.
|
|   Give it a list of files, and it will write out the names of the files that
|   are missing the final NL.
|
| KNOWN BUGS
| - Do not expect a sensible result if given non-text files.
| - It does not deal well with directories.
| - It does not deal well with 0 bytes files (which technically IS missing a NL).
|
| EXIT STATUS
|   0  If all files checks out OK.
|   1  If one or more files is missing a NL, or if there is some other problem
|      with a file, like the file is missing, is a directory, has zero length,
|      etc.
|
| AUTHOR
|   Jan Bruun Andersen, 2023-03-04
\**********************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void usage (char *prog_name)
{
    fprintf(stderr, "Usage: %s file-path[...]\n", prog_name);
}

int main (int argc, char *argv[])
{
    extern int  errno;
    int         errors = 0;

    switch (argc) {
        case 1: 
            usage(argv[0]);
            exit(1);
    }

    for (char **path = ++argv; *path; ++path ) {
        int    fd = open(*path, O_RDONLY);
        char   buf[1];

        if (fd                      < 0
        ||  lseek(fd, -1, SEEK_END) < 0
        ||  read(fd, buf, 1)        < 0
        ) {
            perror(*path);
            errors++;
            (void) close(fd);
            continue;
        }

        if (buf[0] != 10) {
            printf("%s\n", *path);
            errors++;
        }

        (void) close(fd);
    }

    return (errors > 0);
}

// vim: set shiftwidth=4 autoindent nohlsearch number:
