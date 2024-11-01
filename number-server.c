#include "http-server.h"
#include <string.h>

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";

char const HTTP_200_SUCCESS[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

int const ADD_PATH_NUM_INDEX = 11;

int num = 0;

void handle_shownums(int client_sock, char *path) {
    printf("SERVER LOG: Got request for path \"%s\"\n", path);
    
    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "The number is: %ld", num);

    write(client_sock, HTTP_200_SUCCESS, strlen(HTTP_200_SUCCESS));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_increment(int client_sock, char* path) {
    printf("SERVER LOG: Got request for path \"%s\"\n", path);
    

    ++num;
    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "The number is now: %ld", num);

    write(client_sock, HTTP_200_SUCCESS, strlen(HTTP_200_SUCCESS));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_add(int client_sock, char* path) {
    printf("SERVER LOG: Got request for path \"%s\"\n", path);

    int add_num = atoi(path + 11);

    num += add_num;
    
    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "The number is now: %ld", num);

    write(client_sock, HTTP_200_SUCCESS, strlen(HTTP_200_SUCCESS));
    write(client_sock, response_buff, strlen(response_buff));
}

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, response_buff, strlen(response_buff));
}

int is_numeric(char *str, int index) { 
    if (index > strlen(str) - 1) {
	return 0;
    }

    int i;
    for (i = index; str[i] != '\0'; ++i) {
	if (!(str[i] >= '0' && str[i] <= '9')) {
	    return 0;
	}
    }

    return 1;
}


void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strcmp(path, "/shownum") == 0) {
	handle_shownums(client_sock, path);	
    } else if (strcmp(path, "/increment") == 0) {
	handle_increment(client_sock, path);
    } else if (strstr(path, "/add?value=") != NULL && 
	is_numeric(path, ADD_PATH_NUM_INDEX)) {
	handle_add(client_sock, path);
    } else {
	handle_404(client_sock, path);
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
