#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

char* parse_request(char *request);
void throw_error(const char *error_message);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int address_length = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);


    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("%s %i...", "Server running on port", PORT);

    while(1) {
        printf("\n%s\n", "Waiting for connection...");
        
        new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address_length);
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, 1024);
        char* path = parse_request(buffer);

        if (path == NULL) {
            throw_error("NULL PATH");
            char error_response[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
            write(new_socket, error_response, sizeof(error_response));
            close(new_socket);
        }

        size_t path_lenght = strlen(path);
        char response[1024];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\nContent-Length: %zu\r\n\r\n%s",
            path_lenght, path);
        write(new_socket, response, strlen(response));
        free(path);

        close(new_socket);
    }

    // char request[] = "GET /index.html HTTP/1.1";
    // parse_request(request);
    return 0;
}

char* parse_request(char *request) {
    printf("%s", "Method: ");

    // GET request
    if (strncmp(request, "GET", 3) == 0) {
        printf("%s\n%s", "GET", "Path: ");

        char *path_start_ptr = strchr(request, ' ');
        if (path_start_ptr == NULL) {
            throw_error("INVALID REQUEST (no start space)");
            return NULL;
        }

        path_start_ptr++; // Skip to "/"
        char *path_end_ptr = strchr(path_start_ptr, ' ');
        if (path_end_ptr == NULL) {
            throw_error("INVALID REQUEST (no end space)");
            return NULL;
        }

        size_t path_length = path_end_ptr - path_start_ptr;
        char *path = (char*) malloc(path_length + 1);
        if (path == NULL) {
            throw_error("MEMORY ALLOC FAILED");
            return NULL;
        };
        
        memcpy(path, path_start_ptr, path_length);
        path[path_length] = '\0';
        printf("%s", path);
        return path;

    } else {
        throw_error("INVALID METHOD");
        return NULL;
    }
}

void throw_error(const char *error_message) {
    printf("\n%s%s\n", "ERROR: ", error_message);
}