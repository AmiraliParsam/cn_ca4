#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int MAX_SEGMENTS = 100; // Max segments to receive
const int MSS = 1024;         // Maximum Segment Size (in bytes)

void simulate_receiver(SOCKET listenSocket)
{
    // Listen for incoming connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // Accept a client socket
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(listenSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // Receive loop
    char buffer[MSS];
    int segments_received = 0;

    while (segments_received < MAX_SEGMENTS)
    {
        int bytesReceived = recv(clientSocket, buffer, MSS, 0);
        if (bytesReceived > 0)
        {
            segments_received++;
            std::cout << "Received segment " << segments_received << " from "
                      << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
            // Simulate sending ACKs (echo ACK for the received segment number)
            int segment_num = segments_received - 1;
            std::cout << "Sending ACK for segment " << segment_num << std::endl;
            send(clientSocket, (char *)&segment_num, sizeof(segment_num), 0);
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed." << std::endl;
            break;
        }
        else
        {
            std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // Close the client socket
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();

    std::cout << "Receiver finished. Press Enter to exit..." << std::endl;
    std::cin.ignore(); // Wait for user to press Enter
}

int main()
{
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0)
    {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return -1;
    }

    // Create a TCP listening socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        WSACleanup();
        return -1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in listenAddr;
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(54000); // Example port number
    listenAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr *)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Can't bind socket! Quitting" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    // Start listening for incoming connection requests
    simulate_receiver(listenSocket);

    // Close the socket
    closesocket(listenSocket);

    // Cleanup Winsock
    WSACleanup();

    std::cout << "Receiver finished. Press Enter to exit..." << std::endl;
    std::cin.ignore(); // Wait for user to press Enter

    return 0;
}
