#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "compare.h"
#include "generate.h"
#include "md5.h"

/*
 * returns 0 when hashes are same
 */
int compare_hash(MD5_CTX *ctx, char *stream, unsigned char *result, char *current_hash, char *goal_hash)
{
    MD5_Init(ctx);
    MD5_Update(ctx, stream, strlen(stream));
    MD5_Final(result, ctx);


    /*** | found this piece of code here : https://stackoverflow.com/questions/7627723/how-to-create-a-md5-hash-of-a-string-in-c,
     *** ˇ  but i am still not sure why and how exactly it works. ***/
    for (int i = 0; i < 16; ++i) {
        snprintf(&(current_hash[i*2]), 16*2, "%02x", (unsigned int)(result[i]));
    }

    size_t m = 0;
    for (m = 0; m < 32; ++m) {        
        if (goal_hash[m] != current_hash[m]) {
            break;
        }
    }

    if (m != 32) {
        return 1;
    }
    return 0;

}

int find_password(FILE *dict, char *goal_hash, uint8_t opt)
{       
    size_t array_size = 10;
    size_t string_size = 0;

    char *stream = malloc(array_size);
    if (!stream) {
        fprintf(stderr, "ERROR:Malloc failed!\n");
        return 1;
    }

    MD5_CTX ctx;
    unsigned char result[16];
    char current_hash[33];

    char c = '\0';
    while (c != EOF) {
        c = (char) fgetc(dict);

        if (c == '\n') {
            stream[string_size] = '\0';
            if (opt) {                
                if (!(gencmp_hash("", stream, goal_hash))) {
                    string_size = 0;
                    continue;
            }
                free(stream);
                return 0;
            } else {
                if (compare_hash(&ctx, stream, result, current_hash, goal_hash)) {
                    string_size = 0;
                    continue;
                }
                printf("password found\n%s\n", stream);
                free(stream);
                return 0;
            }
        }
        stream[string_size] = c;
        ++string_size;

        if (string_size >= (array_size - 1)) {
            array_size *= 2;
            char *tmp = realloc(stream, array_size);
            if (!tmp) {
                fprintf(stderr, "ERROR:Realloc failed!\n");
                free(stream);
                return 1;
            }
            stream = tmp;
        }
    }

    free(stream);
    return -1;
}
