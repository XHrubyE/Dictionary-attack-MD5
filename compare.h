#include <stdint.h>
#include <stdio.h>

#include "md5.h"

int compare_hash(MD5_CTX *ctx, char *stream, unsigned char *result, char *current_hash, char *goal_hash);
int find_password(FILE *dict, char *goal_hash, uint8_t opt);

