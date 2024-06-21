#include <iostream>
#include <vector>

using namespace std;

// Simulation parameters
const int INITIAL_CWND = 1;  // Initial congestion window size
const int THRESHOLD = 16;    // Initial threshold
const int MAX_PACKETS = 100; // Maximum packets to simulate

// TCP state variables
int cwnd = INITIAL_CWND;  // Congestion window
int ssthresh = THRESHOLD; // Slow start threshold

// Simulated packet class
class Packet
{
public:
    bool acked; // Whether the packet has been acknowledged
    Packet() : acked(false) {}
};

// Function to simulate packet loss (for simplicity, just set packet as not acked)
void simulate_packet_loss(Packet &packet)
{
    packet.acked = false;
}

// Function to simulate receiving an ACK
void receive_ack(Packet &packet)
{
    packet.acked = true;
}

// TCP New Reno congestion control algorithm
void tcp_new_reno()
{
    vector<Packet> packets(MAX_PACKETS); // Simulated packets

    int next_packet_index = 0; // Index of the next packet to send

    while (next_packet_index < MAX_PACKETS)
    {
        // Send packets based on current cwnd
        for (int i = 0; i < cwnd && next_packet_index < MAX_PACKETS; ++i)
        {
            // Simulate sending packet
            Packet &packet = packets[next_packet_index];
            simulate_packet_loss(packet); // Simulate packet loss for demonstration

            // Move to the next packet
            ++next_packet_index;
        }

        // Simulate receiving ACKs
        for (int i = 0; i < cwnd && i < MAX_PACKETS; ++i)
        {
            Packet &packet = packets[i];
            if (!packet.acked)
            {
                receive_ack(packet);

                // Handle fast retransmit and fast recovery
                if (packet.acked && i >= ssthresh)
                {
                    // Fast recovery phase
                    cwnd++;
                }
                else
                {
                    // Congestion avoidance phase
                    if (cwnd < ssthresh)
                    {
                        // Slow start phase
                        cwnd *= 2;
                    }
                    else
                    {
                        // Congestion avoidance phase
                        cwnd++;
                    }
                }
            }
        }
    }
}

int main()
{
    // Run TCP New Reno simulation
    tcp_new_reno();

    cout << "Simulation completed." << endl;
    return 0;
}
