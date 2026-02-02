#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "http_parser.h"
#include "utils.h"

#define PORT 8080

// char* parse_request(char *request);

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
        printf("\n\n%s\n", "Waiting for connection...");
        
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

    return 0;
}