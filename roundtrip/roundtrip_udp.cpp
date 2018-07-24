
#include <sys/types.h>
#include <sys/socket.h>

const size_t frameLen = 2*sizeof(int64_t);
int createNonblockingUDP()
{
    int sockfd = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0)
    {
        std::cerr << "create nonblock udp socket failed\n"; 
    }

    return sockfd;
}

void serverRead