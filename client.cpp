#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>

int main() 
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server: " << strerror(errno) << std::endl;
        return 1;
    }

    // // Send data to the server
    // const char* message = "Hello, Server!";
    // if (send(clientSocket, message, strlen(message), 0) == -1) {
    //     std::cerr << "Error sending data to server: " << strerror(errno) << std::endl;
    //     return 1;
    // }

    // HTTP GET request
    const char* httpRequest =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n" // Adjust the host and port as needed
        "Connection: close\r\n"    // Close the connection after receiving the response
        "\r\n";                    // Empty line indicating the end of headers

    if (send(clientSocket, httpRequest, strlen(httpRequest), 0) == -1) {
        std::cerr << "Error sending HTTP request to server: " << strerror(errno) << std::endl;
        return 1;
    }

    // Receive data from the server
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data from server: " << strerror(errno) << std::endl;
        return 1;
    }

    //--------parse HTTP response------------

    std::istringstream responseStream(buffer);

    // Read and print the first line (HTTP status line)
    std::string statusLine;
    if (std::getline(responseStream, statusLine)) {
        std::cout << "HTTP Status Line: " << statusLine << std::endl;
    }

    // Parse and print the headers
    std::string headerLine;
    while (std::getline(responseStream, headerLine) && !headerLine.empty()) {
        std::cout << "Header: " << headerLine << std::endl;
    }

    // Print the response body (if any)
    std::string responseBody;
    while (std::getline(responseStream, responseBody)) {
        // Handle the response body as needed
        std::cout << "Response: " << responseBody << std::endl;
    }

    std::cout << "Received from server: " << buffer << std::endl;

    close(clientSocket);

    return 0;
}
