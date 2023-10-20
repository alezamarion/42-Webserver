#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
            continue; // Continue listening for other connections
        }

        // Receive data from the client
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1)
        {
            std::cerr << "Error receiving data from client: " << strerror(errno) << std::endl;
        } 
        else if (bytesRead == 0)
        {
            std::cerr << "Client disconnected." << std::endl;
        } 
        else 
        {
            //------server handles http client message------
            // Null-terminate the received data to treat it as a C string
            buffer[bytesRead] = '\0';

            // Parse the HTTP request
            char* token = strtok(buffer, "\r\n"); // Split lines by carriage return and newline

            // Print the request line
            std::cout << "Request Line: " << token << std::endl;

            // Parse and print the headers
            while ((token = strtok(NULL, "\r\n"))) {
                std::cout << "Header: " << token << std::endl;
            }
        }

        // Send a "Hello, Client!" message to the connected client
        // const char* message = "Hello, Client! :D";

        // Basic HTTP response
        const char* httpResponse = "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "\r\n"
                                    "Hello, Client! :D";
                                        
        if (send(clientSocket, httpResponse, strlen(httpResponse), 0) == -1)
        {
            std::cerr << "Error sending data to client: " << strerror(errno) << std::endl;
        }
 
        // Close the client socket
        close(clientSocket); 
    }

    // Close the server socket (this code will not be reached in this example)
    close(serverSocket);

    return 0;
}
