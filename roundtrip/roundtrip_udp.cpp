
#include <sys/types.h>
#include <sys/socket.h>
#include <ctime>

const int g_port = 3123;

struct Message 
{
    int64_t request;
    int64_t response;
} __attribute__((__packed__));

static_assert(sizeof(Message) == 16, "Message size should be 16 bytes");

int64_t now()
{
    struct timeval tv = {0, 0};
    gettimeofday(&tv, NULL);

    return tv.tv_sec * int64_t(100000) + tv.usec;
}

void runServer()
{
    
}