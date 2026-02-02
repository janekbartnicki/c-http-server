#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_request(char *request);
void throw_error(const char *error_message);

int main() {
    char request[] = "GET /index.html HTTP/1.1";
    parse_request(request);
    return 0;
}

void parse_request(char *request) {
    printf("%s", "Method: ");

    // GET request
    if (strncmp(request, "GET", 3) == 0) {
        printf("%s\n%s", "GET", "Path: ");

        char *path_start_ptr = strchr(request, ' ');
        if (path_start_ptr == NULL) {
            throw_error("INVALID REQUEST (no start space)");
            return;
        }

        path_start_ptr++; // Skip to "/"
        char *path_end_ptr = strchr(path_start_ptr, ' ');
        if (path_end_ptr == NULL) {
            throw_error("INVALID REQUEST (no end space)");
            return;
        }

        size_t path_length = path_end_ptr - path_start_ptr;
        char *path = (char*) malloc(path_length + 1);
        if (path == NULL) {
            throw_error("MEMORY ALLOC FAILED");
            return;
        };
        
        memcpy(path, path_start_ptr, path_length);
        path[path_length] = '\0';
        printf("%s", path);
        free(path);

    } else throw_error("INVALID METHOD");
}

void throw_error(const char *error_message) {
    printf("\n%s%s\n", "ERROR: ", error_message);
}