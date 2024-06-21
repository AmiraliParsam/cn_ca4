#include <iostream>
#include <vector>

using namespace std;

// Simulation parameters
const int MAX_PACKETS_IN_FLIGHT = 100;
const int INITIAL_CWND = 1;
const int INITIAL_SSTHRESH = 16;

// State variables
int cwnd = INITIAL_CWND;
int ssthresh = INITIAL_SSTHRESH;
int packets_in_flight = 0;
int duplicate_ack_count = 0;
bool in_slow_start = true;

// Function to simulate sending a packet
void send_packet()
{
    // Simulate sending a packet
    cout << "Sending packet. cwnd = " << cwnd << endl;
}

// Function to simulate receiving an acknowledgment
void receive_acknowledgment()
{
    // Simulate receiving an acknowledgment
    cout << "Received acknowledgment." << endl;
}

// Main simulation loop
void simulate_tcp()
{
    int total_packets_to_send = 1000; // Total packets to send in simulation

    for (int packet_sent = 0; packet_sent < total_packets_to_send; ++packet_sent)
    {
        // Send packets up to the current cwnd limit
        for (int i = 0; i < min(cwnd - packets_in_flight, MAX_PACKETS_IN_FLIGHT - packets_in_flight); ++i)
        {
            send_packet();
            packets_in_flight++;
        }

        // Simulate receiving acknowledgments
        for (int ack = 0; ack < packets_in_flight; ++ack)
        {
            receive_acknowledgment();
            packets_in_flight--;

            // Handle acknowledgment based on TCP New Reno algorithm
            if (in_slow_start)
            {
                // In Slow Start phase
                if (cwnd < ssthresh)
                {
                    cwnd++; // Exponential increase in cwnd
                }
                else
                {
                    in_slow_start = false;
                }
            }
            else
            {
                // In Congestion Avoidance phase
                cwnd += 1 / cwnd; // Slow linear increase in cwnd
            }
        }

        // Check for packet loss using duplicate acknowledgments
        if (duplicate_ack_count == 3)
        {
            // Fast Retransmit/Fast Recovery
            ssthresh = max(cwnd / 2, 2); // Set ssthresh
            cwnd = ssthresh + 3;         // Reduce cwnd and adjust
            duplicate_ack_count = 0;     // Reset duplicate acknowledgment count
        }
    }
}

int main()
{
    simulate_tcp();
    return 0;
}
