#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(const char *error_message) {
    fprintf(stderr, "[ERROR]: %s\n", error_message);
}