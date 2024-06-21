**Computer Networks Project 4**

The purpose of this assignment is to deepen the understanding of TCP protocols, error handling, and network communication using the C++ programming language. we should implement different aspects of the TCP protocol experimentally and analyze the behavior of this protocol under different network conditions.

**Part Two Explanation:**

The TCP New Reno algorithm enhances the congestion control mechanism in TCP (Transmission Control Protocol) to manage network congestion effectively. It operates in four distinct phases:

**Slow Start Phase:**

- TCP starts by sending segments at an increasing rate until it detects network congestion.
- 
- Initially, it sends segments one at a time and doubles the number of segments sent in each round-trip time until it reaches a congestion threshold (cwnd threshold).
- 
**Congestion Avoidance Phase:**
  
- After reaching the congestion threshold (cwnd threshold), TCP enters the congestion avoidance phase.
- 
- In this phase, TCP increases the congestion window size (cwnd) more conservatively, typically by incrementing it linearly for each round-trip time.
- 
**Fast Retransmit Phase:**
  
- When TCP detects packet loss (indicated by duplicate acknowledgments), it assumes congestion has occurred.
- 
- TCP quickly retransmits the missing segment without waiting for a timeout, which helps to reduce the delay caused by timeouts.
- 
**Fast Recovery Phase:**
  
- After fast retransmit, TCP enters fast recovery.
- 
- During this phase, TCP reduces its congestion window size to half of what it was before the packet loss occurred.
- 
- It then re-enters congestion avoidance to gradually increase the congestion window size.
