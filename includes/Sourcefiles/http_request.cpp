
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
        requestlines_to_vector();
        requestlines_to_hashtable();

    }

    // Parsing http request line by line, saving to vector
    void requestlines_to_vector()
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
            
            if(std::regex_search(line,pattern))
            {
                payload_flag = 1;
                std::cout << "Move the payload" << std::endl;
            }

            else // adding line to vector 
            {
            // std::cout << line << std::endl;
            requestLines.push_back(line);
            }
        }
        http_messgae_first_line = requestLines[0];
        requestLines.erase(requestLines.begin());
        

    }

   void requestlines_to_hashtable()
    {
        // WS = WhiteSpace, hf = Header Field
        std::regex ws_error_pattern(R"(([a-zA-Z]+\s+:))");
        std::regex hf_pattern(R"(([a-zA-Z]+:)(\s*[a-zA-Z]+)\s*)");
        
        for(std::string element : requestLines)
        {
            if (std::regex_search(element,ws_error_pattern))
            {
                // Send 400 (Bad Request), Stop adding to hash and send 
                std::cout << "Follow Protocol Sir" << std::endl;
            }
            else
            {
                std::smatch matches;
                std::regex_search(element,matches,hf_pattern);
                std::cout <<"Key: " << matches[0] << " Value: " << matches[1] << std::endl;

                          
            }
        }    
    }


    

  

    void print_request()
    {
        std::cout << "Request lines:" << std::endl;
        for (const std::string& line : requestLines)
        {
            // std::cout << line << std::endl;
        }
        if(payload_flag)
        {
            // std::cout << "THERE IS A MESSAGE" << std::endl;
        }
    }

    
};

int main()
{
    std::string http_request = "GET /index.html HTTP/1.1\r\nHost : example.com\r\nUserAgent: Mozilla/5.0\r\nContent-Length: 220\r\n\r\nThis is the body stuff";
    HTTP_REQUEST request(http_request);

    return 0;
}