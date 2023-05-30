#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <cerrno>
#include <vector>



int main()
{

// ##################################### Create Server_Socket #####################################

int server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
if (server_socket_fd < 0)
{
    std::cout << "Cannot create socket, Errno: \n" << errno << std::endl;
    exit(-1);
}
else 
{
    std::cout << "Server Socket Created! Server fd "<< server_socket_fd << "\n" << std::endl;
}

// Creating socket options to close socket when the program exits or crashes 
linger lin;
lin.l_onoff = 1;
lin.l_linger = 1;
size_t structSize = sizeof(struct linger);

//  int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
int sock_options = setsockopt(server_socket_fd,SOL_SOCKET,SO_LINGER,&lin,structSize);
if(sock_options < 0)
{
    std::cout << "setsock failed, Errno: \n" << errno << std::endl;
    exit(-1);
}

// ##################################### Bind(Naming) Socket to IP and PORT #####################################

// Creating Socket Familty struct
struct sockaddr_in family_address;
const int PORT = 8080;
memset((char*)&family_address,0,sizeof(family_address)); // Initializing memory section for family address to 0, converted familyaddress to char which is one byte sections
family_address.sin_addr.s_addr = htonl(INADDR_ANY); // htonl: convert a 32-bit unsigned integer from the host byte order to the network byte order
family_address.sin_family = AF_INET;
family_address.sin_port = htons(PORT); // htons: convert a 16-bit unsigned integer from the host byte order to the network byte order
socklen_t address_length = sizeof(family_address);
if (bind(server_socket_fd,(struct sockaddr*) &family_address,address_length) < 0)
{
   std::cout << "Could not bind socket Errno Code: " << errno << std::endl;
   exit(-1);
}

// ##################################### Listen for Traffic with Server_Socket #####################################

if(listen(server_socket_fd,3) < 0)
{
    std::cout << "Cannot listen to socket, Errno: " << errno << "\n"<< std::endl;
        exit(-1);
}


while(1)
{

    // ##################################### Accept #####################################
    std::cout << "################################ Waiting for Client Connection ################################ \n" << std::endl;
    int data_socket_fd = accept(server_socket_fd,(struct sockaddr*) &family_address,(socklen_t *)&address_length);
    std::cout << data_socket_fd << "\n" <<std::endl;
    if (data_socket_fd < 0)
    {
        std::cout << " Client connection socket not created, Errno: " << errno << "\n"<< std::endl;
            return -1;
    }
    //Allocate to Heap attempt to prevent buffer overflow
    char requestbuffer[30000];
    int valread = read( data_socket_fd , requestbuffer, 30000);
    std::string requestbuffer_str(requestbuffer);
    std::cout << "################################ Reading Client HTTP Request ################################ \n" << std::endl;
    // This section will contain HTTP_REQUEST Class to parse and sanatize request according the RFC7230
    
    std::cout << requestbuffer_str << std::endl;
}

    return 0;
}