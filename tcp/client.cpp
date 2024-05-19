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
const int CHUNK_SIZE = 128;

int main() {
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;

    int err;
    if ((err = getaddrinfo("localhost", PORT, &hints, &res)) != 0) {
        cerr << gai_strerror(err) << endl;
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    if ((connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1) {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        freeaddrinfo(res);
        return 1;
    }

    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, res->ai_addr, ipstr, res->ai_addrlen);
    cout << "The client is connected on address: " << ipstr << ":" << PORT << endl;

    freeaddrinfo(res);

    char buffer[CHUNK_SIZE];
    int n;

    if ((n = recv(sockfd, buffer, CHUNK_SIZE - 1, 0)) == -1) {
        std::cerr << "recv: " << strerror(errno) << std::endl;
    }

    close(sockfd);

    buffer[n] = '\0'; // terminate c-string

    cout << "Recieved the message: " << buffer << endl;

    return 0;
}
