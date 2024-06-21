#include <iostream>
#include <vector>

using namespace std;

// Parameters
const int MAX_PACKETS = 100; // Total number of packets to simulate
const int MSS = 10;          // Maximum Segment Size
const int CWND_INITIAL = 10; // Initial congestion window size
const int THRESHOLD = 20;    // Initial threshold for fast retransmit

// TCP State variables
int cwnd = CWND_INITIAL;  // Congestion window size
int ssthresh = THRESHOLD; // Slow start threshold
int acked_packets = 0;    // Packets acknowledged by receiver

// Simulated network conditions
vector<bool> ack_received(MAX_PACKETS, false);

// Function to simulate sending packets and receiving acknowledgments
void simulate_transfer()
{
    int packets_sent = 0;

    while (acked_packets < MAX_PACKETS)
    {
        // Send packets up to the current congestion window size
        for (int i = 0; i < cwnd && packets_sent < MAX_PACKETS; ++i)
        {
            cout << "Packet " << packets_sent << " sent.\n";
            packets_sent++;
        }

        // Simulate ACK reception
        for (int i = 0; i < packets_sent; ++i)
        {
            if (!ack_received[i])
            {
                // Simulate ACK reception with probability 0.9
                if (rand() % 10 < 9)
                {
                    ack_received[i] = true;
                    acked_packets++;
                    cout << "ACK received for packet " << i << ".\n";
                }
            }
        }

        // Congestion control phases
        if (acked_packets >= ssthresh)
        {
            // Congestion avoidance phase
            cwnd += MSS * MSS / cwnd;
        }
        else
        {
            // Slow start phase
            cwnd *= 2;
        }
    }
}

int main()
{
    cout << "Simulation starting with initial cwnd = " << cwnd << ", ssthresh = " << ssthresh << ".\n";
    simulate_transfer();
    cout << "Simulation finished.\n";
    return 0;
}
