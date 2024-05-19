#include <iostream>
#include <cstring>
#include <cerrno>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

const char *PORT = "8080";
const int BACKLOG = 5;

int main() {
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;

    int err;
    if ((err = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        cerr << gai_strerror(err) << endl;
        return 1;
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    if ((bind(socket_fd, res->ai_addr, res->ai_addrlen)) == -1) {
        std::cerr << "bind: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    if ((listen(socket_fd, BACKLOG)) == -1) {
        std::cerr << "listen: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, res->ai_addr, ipstr, res->ai_addrlen);
    cout << "The server is listening on address: " << ipstr << ":" << PORT << endl;

    freeaddrinfo(res);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    int client_fd;

    if ((client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size)) == -1) {
        std::cerr << "accept: " << strerror(errno) << std::endl;
        return 1;
    }

    // only expecting one connection - stop listening on server socket
    close(socket_fd);

    auto msg = "Hello, Stranger!";
    int len = strlen(msg);
    int n;

    if ((n = send(client_fd, msg, len, 0)) == -1) {
        std::cerr << "send: " << strerror(errno) << std::endl;
    }

    close(client_fd);

    return 0;
}
