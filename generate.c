#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "generate.h"
#include "compare.h"
#include "md5.h"

/*
 * returns 1 when char is supposed to be changed
 */
int check_char(char c)
{
    c = (char) tolower(c);
    char array[] = "abeilost";

    for (size_t i = 0; i < strlen(array); ++i) {
        if (c == array[i]) {
            return 1;
        }
    }
    return 0;
}

char interchange_char(char c)
{
    c = (char) tolower(c);
    switch (c) {
    case 'a' :
       return '@';
    case '@' :
        return '4';
    case 'b' :
        return '8';
    case 'e' :
        return '3';
    case 'i' :
        return '!';
    case 'l' :
        return '1';
    case 'o' :
        return '0';
    case 's' :
        return '$';
    case '$' :
        return '5';
    case 't' :
        return '7';
    }
    return c;
}


/*
 * recursively generating combinations
 */
int gencmp_hash(char *parent_string, char *rec_string, char *goal_hash)
{
    if (!(*rec_string)) {
        MD5_CTX ctx;
        unsigned char result[16];
        char current_hash[33];


        size_t size = strlen(parent_string) - 1;
        if (!(compare_hash(&ctx, parent_string, result, current_hash, goal_hash))) {
            printf("password found\n%s\n", parent_string);
            return 1;
        }

        char flag = parent_string[size];
        parent_string[size] = interchange_char(parent_string[size]);
        while (flag != parent_string[size]) {
            if (!(compare_hash(&ctx, parent_string, result, current_hash, goal_hash))) {
                printf("password found\n%s\n", parent_string);
                return 1;
            }
            flag = parent_string[size];
            parent_string[size] = interchange_char(parent_string[size]);
        }
        return 0;
    }

    size_t i;
    char cflag; //char changing stopper
    size_t parent_size = strlen(parent_string);

    for (i = 0; i < strlen(rec_string); ++i) {
        if (check_char(rec_string[i])) {

            char slice[parent_size + i + 2];

            slice[0] = '\0';
            strcat(slice, parent_string);
            for (size_t n = 0; n < i; ++n) {
                slice[parent_size + n] = rec_string[n];
            }
            slice[parent_size + i + 1] = '\0';

            slice[parent_size + i] = rec_string[i];
            if (gencmp_hash(slice, &rec_string[i + 1], goal_hash)) {
                return 1;
            }


            cflag = slice[parent_size + i];
            slice[parent_size + i] = interchange_char(rec_string[i]);

            while (slice[parent_size + i] != cflag) {
                if (gencmp_hash(slice, &rec_string[i + 1], goal_hash)) {
                    return 1;
                }
                cflag = slice[parent_size + i];
                slice[parent_size + i] = interchange_char(slice[parent_size + i]);
            }
            return 0;
        }
    }

    char slice[parent_size + i + 1];
    slice[0] = '\0';
    strcat(slice, parent_string);
    for (size_t n = 0; n < (i); ++n) {
        slice[parent_size + n] = rec_string[n];
    }
    slice[parent_size + i] = '\0';
    if (gencmp_hash(slice, "", goal_hash)) {
        return 1;
    }

    return 0;
}
