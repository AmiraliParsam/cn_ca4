#include <iostream>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

void handleClient(SOCKET clientSocket)
{
    char buffer[BUFFER_SIZE] = {0};
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    getpeername(clientSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // SYN
    int valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (valread > 0)
    {
        std::cout << "Received SYN from " << ntohs(clientAddr.sin_port) << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive SYN from " << ntohs(clientAddr.sin_port) << std::endl;
        closesocket(clientSocket);
        return;
    }

    // SYN-ACK
    const char *synAckMsg = "SYN-ACK from server";
    if (send(clientSocket, synAckMsg, strlen(synAckMsg), 0) == SOCKET_ERROR)
    {
        std::cerr << "Failed to send SYN-ACK to " << ntohs(clientAddr.sin_port) << std::endl;
        closesocket(clientSocket);
        return;
    }
    std::cout << "Sent SYN-ACK to " << ntohs(clientAddr.sin_port) << std::endl;

    // ACK
    valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (valread > 0)
    {
        std::cout << "Received ACK from " << ntohs(clientAddr.sin_port) << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive ACK from " << ntohs(clientAddr.sin_port) << std::endl;
        closesocket(clientSocket);
        return;
    }

    // Data transfer
    while ((valread = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        std::cout << "Received from " << ntohs(clientAddr.sin_port) << ": " << buffer << std::endl;

        // Send ACK
        const char *ackMsg = "ACK from server";
        send(clientSocket, ackMsg, strlen(ackMsg), 0);

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);
    }

    std::cout << "Client " << ntohs(clientAddr.sin_port) << " disconnected" << std::endl;
    closesocket(clientSocket);
}

int main()
{
    WSADATA wsaData;
    int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartupResult != 0)
    {
        std::cerr << "WSAStartup failed with error: " << wsaStartupResult << std::endl;
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Prepare server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true)
    {
        // Accept incoming connection
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Handle client in a separate thread
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Detach thread to allow it to run independently
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
