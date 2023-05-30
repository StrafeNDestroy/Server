
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream> 
#include <regex>


char* METHOD,STATUS_CODE,OUTWARD_HEADER;

class HTTP_REQUEST
{

// Class attributes 
private: 
    std::vector<std::string> requestLines;
    std::string http_message;
    std::string http_messgae_first_line; 
    std::unordered_map <std::string,std::string> http_message_hashtable;
    int payload_flag = 0;

// Constructor
public: 
    HTTP_REQUEST(const std::string& http_request)
    {
        http_message = http_request;
        get_request_lines();
        print_request();

    }

    // Parsing http request line by line, saving to vector
    void get_request_lines()
    {
        std::istringstream iss(http_message);
        std::string line;
        std::regex pattern("Content-Length",std::regex_constants::icase);
        while (std::getline(iss, line))
        {
            if (line == "\r") // preventing blank line to be added to vector 
            {

                continue;
            }
            if(std::regex_search(line,pattern)) // if there is a message payload
            {
                payload_flag = 1;
                requestLines.push_back(line);
            }

            else // adding line to vector 
            {
            std::cout << line << std::endl;
            requestLines.push_back(line);
            }
        }    
    }

   void requestlines_to_hashtable()
    {
        std::regex patter();
        for (std::string element, requestLines)
        {

        }
    }


    

  

    void print_request()
    {
        std::cout << "Request lines:" << std::endl;
        for (const std::string& line : requestLines)
        {
            std::cout << line << std::endl;
        }
        if(payload_flag)
        {
            std::cout << "THERE IS A MESSAGE" << std::endl;
        }
    }

    
};

int main()
{
    std::string http_request = "GET /index.html HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Length: 220\r\n\r\nThis is the body stuff";
    HTTP_REQUEST request(http_request);

    return 0;
}