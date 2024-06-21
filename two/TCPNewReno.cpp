#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
vector<bool> ack_received(MAX_PACKETS, false); // Acknowledgment status of each packet
vector<int> packets_in_flight;                 // Packets currently in flight
int next_seq_num = 0;                          // Next sequence number to send

// Function to simulate sending packets and receiving acknowledgments
void send_packet(int seq_num)
{
    cout << "Sending packet " << seq_num << ".\n";
    packets_in_flight.push_back(seq_num);
    next_seq_num++;
}

void receive_ack(int ack_num)
{
    cout << "Received ACK for packet " << ack_num << ".\n";
    ack_received[ack_num] = true;
    acked_packets++;

    // Remove acknowledged packet from packets_in_flight
    auto it = find(packets_in_flight.begin(), packets_in_flight.end(), ack_num);
    if (it != packets_in_flight.end())
    {
        packets_in_flight.erase(it);
    }
}

// Function to simulate network conditions (ACK reception with probability)
void simulate_network()
{
    srand(time(nullptr)); // Seed for random number generation

    // Simulate packet transmission and ACK reception until all packets are acknowledged
    while (acked_packets < MAX_PACKETS)
    {
        // Send packets up to the current congestion window size
        for (int i = 0; i < cwnd && next_seq_num < MAX_PACKETS; ++i)
        {
            send_packet(next_seq_num);
        }

        // Simulate ACK reception
        for (int i = 0; i < packets_in_flight.size(); ++i)
        {
            int packet_num = packets_in_flight[i];
            if (!ack_received[packet_num])
            {
                // Simulate ACK reception with probability 0.9
                if (rand() % 10 < 9)
                {
                    receive_ack(packet_num);
                }
            }
        }

        // Check for fast retransmit condition (3 duplicate ACKs)
        int duplicate_count = 0;
        int duplicate_packet = -1;
        for (int i = 0; i < packets_in_flight.size(); ++i)
        {
            int packet_num = packets_in_flight[i];
            if (ack_received[packet_num])
            {
                duplicate_count++;
                if (duplicate_count >= 3)
                {
                    duplicate_packet = packet_num;
                    break;
                }
            }
        }

        if (duplicate_packet != -1)
        {
            // Perform fast retransmit
            cout << "Fast retransmit triggered for packet " << duplicate_packet << ".\n";
            ssthresh = max(cwnd / 2, MSS);    // Set new ssthresh
            cwnd = ssthresh + 3 * MSS;        // Perform fast retransmit and recovery
            acked_packets = duplicate_packet; // Adjust acked_packets to simulate retransmission
            packets_in_flight.clear();        // Clear packets in flight
            for (int i = duplicate_packet; i < next_seq_num; ++i)
            {
                if (!ack_received[i])
                {
                    send_packet(i);
                }
            }
        }
        else
        {
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
}

int main()
{
    cout << "Simulation starting with initial cwnd = " << cwnd << ", ssthresh = " << ssthresh << ".\n";
    simulate_network();
    cout << "Simulation finished.\n";
    return 0;
}
