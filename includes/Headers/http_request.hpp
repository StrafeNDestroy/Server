#ifndef http_request
#define http_request



#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream> 
#include <regex>
#include <fstream>
class HTTP_REQUEST
{

// Class attributes 
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
    
    
    void StringToLinesConversion();
    void Categorize_HeaderFields();
    std::string FirstLine_Parse();
    std::string Fetch_Resource(std::string resource_path);
    void Build_Http_Response(std::string resource_data);
   
// Constructor
public: 
    // PUBLIC ATTRIBUTE
    std::string SERVER_RESPONSE;
    HTTP_REQUEST(const std::string& http_request_message);
};



#endif