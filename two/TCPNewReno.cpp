#include <iostream>
#include <algorithm> // for min

using namespace std;

// Simulated network parameters
const int MAX_SEGMENTS = 1000; // Max segments to send
const int MSS = 1;             // Maximum Segment Size (in segments)
const int INITIAL_CWND = 1;    // Initial congestion window size
const int THRESHOLD = 16;      // Initial threshold

int cwnd = INITIAL_CWND;  // Congestion window
int ssthresh = THRESHOLD; // Slow start threshold

void simulate_network()
{
    int segments_sent = 0;
    int ack_count = 0; // Simulated count of ACKs received

    while (segments_sent < MAX_SEGMENTS)
    {
        // Slow start phase
        while (cwnd < ssthresh)
        {
            // Send packets up to the current cwnd
            for (int i = 0; i < cwnd; ++i)
            {
                if (segments_sent < MAX_SEGMENTS)
                {
                    cout << "Sending segment " << segments_sent << endl;
                    segments_sent++;
                }
            }
            // Simulate ACK reception
            ack_count++;
            cout << "ACK received, ack_count = " << ack_count << endl;
            // Exponential increase in cwnd
            cwnd *= 2;
        }

        // Congestion avoidance phase
        while (segments_sent < MAX_SEGMENTS)
        {
            // Send packets up to the current cwnd
            for (int i = 0; i < cwnd; ++i)
            {
                if (segments_sent < MAX_SEGMENTS)
                {
                    cout << "Sending segment " << segments_sent << endl;
                    segments_sent++;
                }
            }
            // Simulate ACK reception
            ack_count++;
            cout << "ACK received, ack_count = " << ack_count << endl;

            // Linear increase in cwnd
            cwnd++;

            // Simulate congestion (packet loss or duplicate ACKs)
            if (ack_count % 5 == 0)
            { // Simulate packet loss
                cout << "Duplicate ACKs detected, entering fast recovery phase..." << endl;
                ssthresh = max(cwnd / 2, 2); // Reduce threshold
                cwnd = ssthresh + 3;         // Fast recovery: increase cwnd by number of duplicate ACKs
                ack_count++;                 // Simulate ACK for fast recovery
                cout << "Exit fast recovery phase, cwnd = " << cwnd << ", ssthresh = " << ssthresh << endl;
                break; // Exit congestion avoidance phase
            }
        }
    }
}

int main()
{
    simulate_network();

    // Wait for user input to exit
    cout << "Simulation complete. Press Enter to exit..." << endl;
    cin.get(); // Wait for user to press Enter

    return 0;
}
