#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cerrno>
#include <vector>
#include "../Headers/http_request.hpp"
#include <arpa/inet.h>

void server_response(HTTP_REQUEST server_response,int data_socket_fd)
{
    

    if(server_response.EXTENSION == ""|| server_response.EXTENSION == ".js" || server_response.EXTENSION == ".css" )
    {
        char server_response_file_buffer[30000];
        std::strcpy(server_response_file_buffer,server_response.FILE_HTTP_REPONSE.c_str());
        if(write(data_socket_fd,server_response_file_buffer,server_response.FILE_HTTP_REPONSE_SIZE)<0)
        {
            std::cout << "Socket write failed!" << std::endl;
            exit(-1);
        }
        std::cout << "################################ Server Response ################################ \n" << std::endl;
        std::cout << server_response_file_buffer << std::endl;
        close(data_socket_fd);
    }


    else
    {
        char image_text_reponse[30000];
        char image_binary_reponse[100000];
        std::strcpy(image_text_reponse,server_response.IMAGE_FIRSTLINE_HEADERS_REPONSE.c_str());
        std::copy(server_response.IMAGE_BINARY_RESPONSE.begin(),server_response.IMAGE_BINARY_RESPONSE.end(),image_binary_reponse);


        if(write(data_socket_fd,image_text_reponse,server_response.IMAGE_FIRSTLINE_HEADERS_REPONSE_SIZE)<0)
        {
            std::cout << "Socket write failed!" << std::endl;
            exit(-1);
        }

        if(write(data_socket_fd,image_binary_reponse,server_response.IMAGE_BINARY_RESPONSE_SIZE)<0)
        {
            std::cout << "Socket write failed!" << std::endl;
            exit(-1);
        }
        std::cout << "################################ Server Response ################################ \n" << std::endl;
        std::cout << image_text_reponse << std::endl;
        std::cout << image_binary_reponse << std::endl;

        close(data_socket_fd);

    }

}



int main()
{

    // ##################################### Create Server_Socket #####################################

    int server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if (server_socket_fd < 0)
    {
        std::cout << "Cannot create socket, Errno: \n" << errno << std::endl;
        exit(-1);
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
    const int PORT = 9005;
    char ip[] = "172.20.194.194";
    memset((char*)&family_address,0,sizeof(family_address)); // Initializing memory section for family address to 0, converted familyaddress to char which is one byte sections

    inet_pton(AF_INET,ip,&family_address.sin_addr.s_addr); // htonl: convert a 32-bit unsigned integer from the host byte order to the network byte order
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
        if (data_socket_fd < 0)
        {
            std::cout << " Client connection socket not created, Errno: " << errno << "\n"<< std::endl;
                exit(-1);
        }

        std::cout << "################################ Reading Client HTTP Request ################################ \n" << std::endl;

        char client_request_buffer[30000];
        

        if(read( data_socket_fd , client_request_buffer, 30000-1) < 0)
        {
            std::cout << "Socket read failed!" << std::endl;
            exit(-1);
        }
        std::cout << client_request_buffer << std::endl;


        // Sending request as string to HTTP_REQUEST Class for parsing
        const std::string http_request(client_request_buffer);
        HTTP_REQUEST Http_Request(http_request);
        server_response(Http_Request,data_socket_fd);
    }

    return 0;
}