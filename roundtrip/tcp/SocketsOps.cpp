
#include "SocketsOps.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <cassert>

using SA = struct sockaddr;

const SA* sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const SA*>(implicit_cast<const void*>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<SA*> (implicit_cast<void*>(addr));
}

void setNonBlockAndCloseOnExec(int sockfd)
{
    int flags = fcntl(sockfd, FGETFL, 0);
    flags |= O_NONBLOCK;
    int ret = fcntl(sockfd, F_SETFL, flags);

    flags = fcntl(sockfd, FGETFL, 0);
    flags |= FD_CLOEXEC;
    ret = fcntl(sockfd, F_SETFL, flags);

    // 这个地方不能直接 flags |= O_NONBLOCK; flags |= FD_CLOEXEC
    // 然后 一次调用 fcntl(sockfd, F_SETFL, flags)设置，这样是只设置了FD_CLOEXEC 和 O_NONBLOCK 
    // 较大的那个（这个结论是实验获得，不代表所有都是这样）
}

int sockets::createNonblockingOrDie()
{
    #if VALGRIND
        int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        assert(sockfd >=0, "sockets::createNonblockingOrDie");
        setNonBlockAndCloseOnExec(sockfd);
    #else
        int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
        assert(sockfd >=0, "sockets::createNonblockingOrDie");
    #endif

    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr )
{
    int ret = bind(sockfd, sockaddr_cast(&addr), sizeof addr);
    assert(ret ==0 , "sockets::bindOrDie");
}

void sockets::listenOrDie(int sockfd)
{
    // 注意第二个参数设置的是established的最大队列长度
    // 当然内核 sys.net.core.somaxconn 也对该值得上限做了限制
    int ret = listen(sockfd, SOMAXCONN);
    assert(ret == 0, "sockets::listenOrDie");
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof *addr;

#if VALGRIND 
    int connfd = accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0)
    {
        int savedErrno == errno;
        std::cerr << "Socket::accept\n";
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE: // per-process limit of open file desctiptor
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                assert(false, "unexpected")
        }
    }
}