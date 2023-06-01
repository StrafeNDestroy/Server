#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <cerrno>
#include <vector>
#include <unordered_map>
#include <sstream> 
#include <regex>
#include <fstream>
#include <arpa/inet.h>

class HTTP_REQUEST
{
private: 

    std::vector<std::string> HTTP_REQUEST_ELEMENTS;
    std::string HTTP_REQUEST_MESSAGE;
    std::string HTTP_MESSAGE_FIRST_LINE; 
    std::unordered_map <std::string,std::string> HTTP_HEADER_CATEGORIES;
    int BODY_FLAG = 0;
    std::string HTTP_BODY;
    int RESPONSE_CODE = 0;
    int RESPONSE_SENT = 0;
    std::string  METHOD;
    std::string WEBPAGE_PATH = "index.html";
    std::string HTTP_VERSION;
    std::string PATH_HEAD = "../../website/Index/";
    void StringToLinesConversion()
    {
        std::istringstream iss(HTTP_REQUEST_MESSAGE);
        std::string line;
        std::regex pattern("Content-Length",std::regex_constants::icase);
        while (std::getline(iss, line))
        {
            if (line == "\r") // preventing blank line to be added to vector 
            {

                continue;
            }
            
            if(std::regex_search(line,pattern))
            {
                BODY_FLAG = 1;
                std::cout << "Move the payload" << std::endl;
                
            }
            
            HTTP_REQUEST_ELEMENTS.push_back(line);
        }


        HTTP_MESSAGE_FIRST_LINE = HTTP_REQUEST_ELEMENTS[0];
        HTTP_REQUEST_ELEMENTS.erase(HTTP_REQUEST_ELEMENTS.begin());

        // Grabbing message body from vector
        if(BODY_FLAG) 
        {
            HTTP_BODY = HTTP_REQUEST_ELEMENTS.back();
            HTTP_REQUEST_ELEMENTS.erase(HTTP_REQUEST_ELEMENTS.end());
        } 
    }

    void Categorize_HeaderFields()
    {
        // WS = WhiteSpace, hf = Header Field
        std::regex ws_error_pattern(R"(([a-zA-Z]+\s+:))");
        std::regex hf_pattern(R"(([a-zA-Z-]+):(.+))");
        
        for(std::string element : HTTP_REQUEST_ELEMENTS)
        {
            if (std::regex_search(element,ws_error_pattern))
            {
                // Send 400 (Bad Request), Stop adding to hash and send 
                std::cout << "Follow Protocol Sir" << std::endl;
            }
            else
            {
                // FIX ISSUES WITH MATCHING
                std::smatch matches;
                std::regex_search(element,matches,hf_pattern);
                HTTP_HEADER_CATEGORIES.insert({matches[1],matches[2]});                        
            }
        }    
    }    


    std::string FirstLine_Parse()
    {
        std::smatch matches;
        std::string resource_data;
        std::string methods = R"((connect|delete|get|head|options|post|put|trace))";
        std::string website_pages = R"((/))";
        std::string http_version = R"((HTTP/1.1|HTTP/2|HTTP/3))";
        std::string pattern_str = methods+" " + website_pages + " "+ http_version;
        std::regex pattern(pattern_str,std::regex_constants::icase);
        
        if(std::regex_search(HTTP_MESSAGE_FIRST_LINE,matches,pattern))
        {
            METHOD =  matches[1];
            HTTP_VERSION = matches[3] ;
            resource_data = Fetch_Resource(WEBPAGE_PATH);

        }
        else
        {
            std::cout << "Invalid Request" << std::endl;
        }
        return resource_data;
    }

    std::string Fetch_Resource(std::string resource_path)
    {
        std::string appended_line;
        std::string line;

        std::ifstream file;
        std::vector<std::string> vector_data;
        file.open(PATH_HEAD + resource_path);

        while(std::getline(file,line))
        {
            appended_line = line +"\r\n";
            vector_data.push_back(appended_line);
        }
        file.close();

        std::string result;
        for (std::string str : vector_data)
        {
            result += str;
        }
        return result;

    }

    void Build_Http_Response(std::string resource_data)
    {
        
        
        std::string server_response_first_line = HTTP_VERSION + " " + "200" + " " + "OK" + "\r\n";
        std::string content_type = "Content-Type: text/html\r\n";
        int size = sizeof(resource_data);
        std::string sizedata = std::to_string(size);
        std::string server_response_header_fields = "Content-Length: " + sizedata +"\r\n"+content_type;
        std::string space = "\r\n";  
        SERVER_RESPONSE = server_response_first_line+server_response_header_fields+space+resource_data;
    }

public: 
    // PUBLIC ATTRIBUTE
    std::string SERVER_RESPONSE;
    HTTP_REQUEST(const std::string& http_request_message)
    {
        HTTP_REQUEST_MESSAGE = http_request_message;
        StringToLinesConversion();
        Categorize_HeaderFields();
        FirstLine_Parse();
        std::string resource_data = Fetch_Resource(WEBPAGE_PATH);
        Build_Http_Response(resource_data);
    }

};

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
    std::cout << "Server Socket Created!" << std::endl;
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

family_address.sin_addr.s_addr = htonl(0b01111111000000000000000000000001); // htonl: convert a 32-bit unsigned integer from the host byte order to the network byte order
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
            return -1;
    }
    //Allocate to Heap attempt to prevent buffer overflow
    char requestbuffer[30000];
    int valread = read( data_socket_fd , requestbuffer, 30000-1);
    std::string requestbuffer_str(requestbuffer);
    std::cout << "################################ Reading Client HTTP Request ################################ \n" << std::endl;
    // This section will contain HTTP_REQUEST Class to parse and sanatize request according the RFC7230
    const std::string buffer(requestbuffer);
    HTTP_REQUEST Http_Request(buffer);
    std::string server_response = Http_Request.SERVER_RESPONSE;
    int response_size = sizeof(server_response);
    std::cout << server_response << std::endl;
    write(data_socket_fd,requestbuffer,sizeof(server_response));
    
}

    return 0;
}

