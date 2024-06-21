#include <iostream>

using namespace std;

class TCPNewReno
{
private:
    int cwnd;          // Congestion window size
    int ssthresh;      // Slow start threshold
    int flightSize;    // Number of packets in flight
    int packetsAcked;  // Number of packets acknowledged
    bool fastRecovery; // Flag to indicate if in fast recovery mode
    int dupAcks;       // Count of duplicate acknowledgments received

public:
    TCPNewReno()
    {
        cwnd = 1;      // Initial congestion window size (1 for slow start)
        ssthresh = 10; // Initial slow start threshold
        flightSize = 0;
        packetsAcked = 0;
        fastRecovery = false;
        dupAcks = 0;
    }

    void sendPacket()
    {
        if (flightSize < cwnd)
        {
            // Simulate sending packet
            flightSize++;
            cout << "Sending packet " << flightSize << ", cwnd = " << cwnd << endl;
        }
        else
        {
            cout << "Congestion window full, cannot send more packets." << endl;
        }
    }

    void receiveAck(bool isDuplicate)
    {
        packetsAcked++;
        flightSize--;

        if (isDuplicate)
        {
            dupAcks++;
        }
        else
        {
            dupAcks = 0; // Reset duplicate ACK counter
        }

        if (dupAcks == 3 && !fastRecovery)
        { // Fast retransmit
            cout << "Received 3 duplicate ACKs, performing fast retransmit." << endl;
            ssthresh = max(cwnd / 2, 2); // Multiplicative decrease
            cwnd = ssthresh + 3;         // Fast recovery threshold
            fastRecovery = true;
            dupAcks = 0;
        }
        else if (fastRecovery && packetsAcked >= cwnd)
        {
            cout << "Exiting fast recovery phase." << endl;
            fastRecovery = false;
            cwnd = ssthresh;
        }
        else
        {
            if (cwnd < ssthresh)
            {
                // Slow start phase
                cwnd++;
                cout << "In slow start phase. cwnd = " << cwnd << ", flightSize = " << flightSize << endl;
            }
            else
            {
                // Congestion avoidance phase
                cwnd += 1.0 / cwnd;
                cout << "In congestion avoidance phase. cwnd = " << cwnd << ", flightSize = " << flightSize << endl;
            }
        }
    }
};

int main()
{
    TCPNewReno tcp;

    // Simulate sending packets and receiving acknowledgments
    for (int i = 0; i < 20; i++)
    {
        tcp.sendPacket();
        if (i == 5 || i == 10 || i == 15)
        {
            // Simulate receiving duplicate ACKs
            tcp.receiveAck(true);
        }
        else
        {
            tcp.receiveAck(false);
        }
    }

    return 0;
}
