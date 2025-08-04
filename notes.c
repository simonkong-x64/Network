#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/type.h>
#include <netdb.h>

// network byte order = big endian
// host byte order = system order (usually little endian)

// ai = address info
// in = internet

getaddrinfo() = gets address info
htons() = converts host byte order to network byte order for shorts
ntohs() = network byte order to host byte order for shorts
htonl() = host to network for longs
ntohl() = net to host for longs

// pton stands for presentation to network
// returns -1 if error. returns 0 if bad address. Error check by seeing if return val > 0
inet_pton(AF_INET, "IP ADDRESS", &(sa.sin_addr)) = converts ip address in dot notation to struct ina or struct in6a

// 
inet_ntop()
// addrinfo is a linked list
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6. 
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
};

// ------------------------------------------------------------------------------------

struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address. Contains both ip addrs and Port Number
}; 

// ------------------------------------------------------------------------------------
// (IPv4 only--see struct sockaddr_in6 for IPv6)

// Parallel structure of sockaddr. Can be casted to sockaddr struct
struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number. must be in Network Byte Order
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr. for padding. always 0000000. done by using memset
};

// ------------------------------------------------------------------------------------
// (IPv4 only--see struct in6_addr for IPv6)

// Internet address (a structure for historical reasons)
struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};

// ------------------------------------------------------------------------------------

// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)

struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};

// ------------------------------------------------------------------------------------

// inet pton examble
struct sockaddr_in sa;   // IPv4
struct sockaddr_in6 sa6; // IPv6

inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));

// ------------------------------------------------------------------------------------

// inet_ntop example

// IPv4:

char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
struct sockaddr_in sa;      // pretend this is loaded with something

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPv4 address is: %s\n", ip4);


// IPv6:

char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
struct sockaddr_in6 sa6;    // pretend this is loaded with something

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

printf("The address is: %s\n", ip6);

// ------------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);


// ------------------------------------------------------------------------------------

int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
}

// servinfo now points to a linked list of 1 or more
// struct addrinfos

// ... do everything until you don't need servinfo anymore ....

freeaddrinfo(servinfo); // free the linked-list

// ------------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol); 



// ------------------------------------------------------------------------------------
// example

struct addrinfo hints, *res;
int sockfd;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

getaddrinfo(NULL, "3490", &hints, &res);

// make a socket:

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// bind it to the port we passed in to getaddrinfo():

bind(sockfd, res->ai_addr, res->ai_addrlen);

// ------------------------------------------------------------------------------------

int yes=1;
//char yes='1'; // Solaris people use this

// lose the pesky "Address already in use" error message
setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

// ------------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/socket.h>

// sockfd = socket() return value
// serv_addr = struct sockaddr
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 

// ------------------------------------------------------------------------------------

struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};

struct addrinfo hints, *res;
int sockfd;

// ------------------------------------------------------------------------------------


// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("www.example.com", "3490", &hints, &res);

// make a socket:

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// connect!

connect(sockfd, res->ai_addr, res->ai_addrlen);

// ------------------------------------------------------------------------------------

// sockfd = socket() to listen to
// backlog = queue of socketfd (max is 20 usually)
int listen(int sockfd, int backlog); 
// returns -1 on error

// ------------------------------------------------------------------------------------


#include <sys/types.h>
#include <sys/socket.h>

// sockfd = socket() that is listen()ing
// addr = usually a pointer to struct sockaddr_storage
// addrlen = size of sockaddr_storage so, accept() wont put more bytes than the size of the data storage
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 
// returns a socketfd
// returns -1 on err

// ------------------------------------------------------------------------------------
// example
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue holds

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(NULL, MYPORT, &hints, &res);

    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype,
                                                 res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                                                       &addr_size);

    // ready to communicate on socket descriptor new_fd!

}
// ------------------------------------------------------------------------------------
// send() and receive() are more for socket_streams than socket_dgram

int send(int sockfd, const void *msg, int len, int flags);
// returns the number of bytes that were sent
// returns -1 if err

int recv(int sockfd, void *buf, int len, int flags);
// returns the number of bytes that were received
// returns -1 if err
// returns 0 if connections was closed on opposite end

// ------------------------------------------------------------------------------------

// sendto() and receiveto() are for socket_Dgrams
int sendto(int sockfd, const void *msg, int len, unsigned int flags,
           const struct sockaddr *to, socklen_t tolen); 
// returns number of bytes sent
// returns -1 if err

int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
             struct sockaddr *from, int *fromlen); 
// returns number of bytes received
// return -1 if err

// ------------------------------------------------------------------------------------
int shutdown(int sockfd, int how); 

// how 	Effect
// 0 	Further receives are disallowed
// 1 	Further sends are disallowed
// 2 	Further sends and receives are disallowed (like close())

// does not close fd. only closes connection(s)
// close() will close send,receive, and socketfd

// ------------------------------------------------------------------------------------
#include <sys/socket.h>

// tells who is at the other end of a connected stream
int getpeername(int sockfd, struct sockaddr *addr, int *addrlen); 

#include <unistd.h>
// name of the computer the program is running on
int gethostname(char *hostname, size_t size); 


