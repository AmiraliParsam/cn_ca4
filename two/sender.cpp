#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <random>
#include <chrono>
#include <thread> 
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

const int MAX_SEGMENTS = 100;           // Max segments to send
const int MSS = 1024;                   // Maximum Segment Size (in bytes)
const int INITIAL_CWND = 1;             // Initial congestion window size
const int THRESHOLD = 16;               // Initial threshold
const int PACKET_LOSS_PROBABILITY = 20; // Probability (%) of packet loss

int cwnd = INITIAL_CWND;  // Congestion window
int ssthresh = THRESHOLD; // Slow start threshold

void simulate_sender(SOCKET sock, sockaddr_in receiverAddr)
{
    int segments_sent = 0;
    int ack_count = 0;     // Simulated count of ACKs received
    int dup_ack_count = 0; // Count of duplicate ACKs received

    // Simulation loop
    while (segments_sent < MAX_SEGMENTS)
    {
        // Slow start phase
        while (cwnd < ssthresh && segments_sent < MAX_SEGMENTS)
        {
            // Send up to the current cwnd
            for (int i = 0; i < cwnd; ++i)
            {
                if (segments_sent < MAX_SEGMENTS)
                {
                    std::cout << "Sending segment " << segments_sent << std::endl;
                    char buffer[MSS]; // Placeholder for segment data
                    // Simulate packet loss
                    if (rand() % 100 >= PACKET_LOSS_PROBABILITY)
                    {
                        send(sock, buffer, MSS, 0);
                        segments_sent++;
                    }
                }
            }
            // Simulate ACK reception (for simplicity, assume immediate ACKs in this example)
            ack_count += cwnd;
            std::cout << "ACK received, ack_count = " << ack_count << std::endl;

            // Exponential increase in cwnd
            cwnd *= 2;
        }

        // Congestion avoidance phase
        while (cwnd >= ssthresh && segments_sent < MAX_SEGMENTS)
        {
            // Send up to the current cwnd
            for (int i = 0; i < cwnd; ++i)
            {
                if (segments_sent < MAX_SEGMENTS)
                {
                    std::cout << "Sending segment " << segments_sent << std::endl;
                    char buffer[MSS]; // Placeholder for segment data
                    // Simulate packet loss
                    if (rand() % 100 >= PACKET_LOSS_PROBABILITY)
                    {
                        send(sock, buffer, MSS, 0);
                        segments_sent++;
                    }
                }
            }
            // Simulate ACK reception (for simplicity, assume immediate ACKs in this example)
            ack_count += cwnd;
            std::cout << "ACK received, ack_count = " << ack_count << std::endl;

            // Linear increase in cwnd
            cwnd++;
        }

        // Fast retransmit and fast recovery
        if (dup_ack_count >= 3)
        { // Assume fast retransmit threshold of 3 duplicate ACKs
            std::cout << "Fast retransmit triggered. Resending segment " << ack_count << std::endl;
            char buffer[MSS]; // Placeholder for segment data
            // Simulate packet loss
            if (rand() % 100 >= PACKET_LOSS_PROBABILITY)
            {
                send(sock, buffer, MSS, 0);
                segments_sent++;
            }
            // Reset duplicate ACK count and adjust cwnd and ssthresh
            dup_ack_count = 0;
            ssthresh = cwnd / 2; // Set ssthresh to half of cwnd
            cwnd = ssthresh + 3; // Fast recovery: increase cwnd by 3 segments
        }

        // Simulate receiving ACKs and potentially duplicate ACKs
        ack_count += cwnd;
        if (rand() % 100 < 10)
        { // Simulate 10% chance of duplicate ACK
            dup_ack_count++;
            std::cout << "Duplicate ACK received, count = " << dup_ack_count << std::endl;
        }
        else
        {
            dup_ack_count = 0;
        }
    }

    // After finishing the simulation, wait before closing
    std::this_thread::sleep_for(std::chrono::seconds(1));
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

    // Create a TCP socket for sending
    SOCKET sendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sendSocket == INVALID_SOCKET)
    {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        WSACleanup();
        return -1;
    }

    // Connect to the receiver
    sockaddr_in receiverAddr;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(54000);                    // Example port number
    inet_pton(AF_INET, "127.0.0.1", &receiverAddr.sin_addr); // Example IP address

    int connectResult = connect(sendSocket, (sockaddr *)&receiverAddr, sizeof(receiverAddr));
    if (connectResult == SOCKET_ERROR)
    {
        std::cerr << "Can't connect to server! Error: " << WSAGetLastError() << std::endl;
        closesocket(sendSocket);
        WSACleanup();
        return -1;
    }

    // Simulate sender
    simulate_sender(sendSocket, receiverAddr);

    // Close the socket
    closesocket(sendSocket);

    // Cleanup Winsock
    WSACleanup();

    std::cout << "Sender finished. Press Enter to exit..." << std::endl;
    std::cin.ignore(); // Wait for user to press Enter

    return 0;
}
