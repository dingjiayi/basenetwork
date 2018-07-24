
#include <sys/types.h>
#include <sys/socket.h>

const int g_port = 3123;

struct Message 
{
    int64_t request;
    int64_t response;
} __attribute__((__packed__));

static_assert(sizeof(Message) == 16, "Message size should be 16 bytes");

int