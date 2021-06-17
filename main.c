#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "compare.h"
#include "generate.h"

/*
 * returns non-zero(true) if string(argument) is correct hesh, otherwise zero(false) is returned
 */
int is_md5_hash(char *argument)
{
    if (strlen(argument) != 32) {
        return 0;
    }
    for (size_t i = 0; i < 32; ++i) {
        if (!isxdigit(argument[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{          
    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "ERROR:Wrong number of arguments!\n");
        return 1;
    }

    uint8_t opt = 0;
    if (argc == 4) {
        if (strcmp(argv[1], "-t")) {
            fprintf(stderr, "ERROR:Unknown option!\n");
            return 1;
        } else {
            opt = 1;
        }
    }

    if (!is_md5_hash(argv[argc - 1])) {
        fprintf(stderr, "ERROR:Incorrect hesh!\n");
        return 1;
    }

    FILE *dictionary = fopen(argv[opt + 1], "r");
    if (!dictionary) {
        fprintf(stderr, "ERROR:Couldn't open file!\n");
        return 1;
    }

    int found = find_password(dictionary, argv[argc - 1], opt);
    if (found > 0) {
        fclose(dictionary);
        return 1;
    }
    if (found < 0) {
        printf("password not found\n");

    }
    fclose(dictionary);
    return 0;
}

