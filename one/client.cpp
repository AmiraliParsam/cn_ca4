#include <iostream>
#include <winsock2.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

void sendData(SOCKET clientSocket)
{
    const char *synMsg = "SYN from client";
    const char *ackMsg = "ACK from client";

    // Send SYN
    if (send(clientSocket, synMsg, strlen(synMsg), 0) == SOCKET_ERROR)
    {
        std::cerr << "Send SYN failed with error: " << WSAGetLastError() << std::endl;
        return;
    }
    std::cout << "Sent SYN to server" << std::endl;

    char buffer[BUFFER_SIZE] = {0};
    int valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (valread > 0)
    {
        std::cout << "Received SYN-ACK from server: " << buffer << std::endl;

        // Send ACK
        if (send(clientSocket, ackMsg, strlen(ackMsg), 0) == SOCKET_ERROR)
        {
            std::cerr << "Send ACK failed with error: " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "Sent ACK to server" << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive SYN-ACK from server" << std::endl;
        return;
    }

    // Data transfer
    const char *dataMsg = "Data from client";
    while (true)
    {
        // Send data to server
        int bytesSent = send(clientSocket, dataMsg, strlen(dataMsg), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            std::cerr << "Send data failed with error: " << WSAGetLastError() << std::endl;
            break;
        }

        // Log the sent packet
        std::cout << "Sending data packet" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2)); // 2 seconds interval
    }
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
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Prepare server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    serverAddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Perform SYN, SYN-ACK, ACK handshake
    sendData(clientSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
