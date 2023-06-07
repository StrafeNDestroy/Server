
#include "../Headers/http_request.hpp"



HTTP_REQUEST::HTTP_REQUEST(const std::string& http_request_message)
{
        HTTP_REQUEST_MESSAGE = http_request_message;
        // Status_Codes();

        StringToLinesConversion();
        Categorize_HeaderFields();
        FirstLine_Parse();
        std::string resource_data = Fetch_Resource(WEBPAGE_PATH);
        Build_Http_Response(resource_data);
}

// json HTTP_REQUEST::Status_Codes()
// {
//     std::ifstream file("../../website/status_codes/status_codes.json");
//     STATUS_CODES = json::parse(file);
// }


void HTTP_REQUEST::StringToLinesConversion()
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

    void HTTP_REQUEST::Categorize_HeaderFields()
    {
        // WS = WhiteSpace, hf = Header Field
        std::regex ws_error_pattern(R"(([a-zA-Z]+\s+:))");
        std::regex hf_pattern(R"(([a-zA-Z-]+):(.+))");
        
        for(std::string element : HTTP_REQUEST_ELEMENTS)
        {
            if (std::regex_search(element,ws_error_pattern))
            {
                // Send 400 (Bad Request), Stop adding to hash and send 
                RESPONSE_STATUS_CODE = "400";
                STATUS_CODES[RESPONSE_STATUS_CODE]["message"];
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


    std::string HTTP_REQUEST::FirstLine_Parse()
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

    std::string HTTP_REQUEST::Fetch_Resource(std::string resource_path)
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

    void HTTP_REQUEST::Build_Http_Response(std::string resource_data)
    {
        
        
        std::string server_response_first_line = HTTP_VERSION + " " + "200" + " " + "OK" + "\r\n";
        std::string content_type = "Content-Type: text/html\r\n";
        int size = sizeof(resource_data);
        std::string sizedata = std::to_string(size);
        std::string server_response_header_fields = "Content-Length: " + sizedata +"\r\n"+content_type;
        std::string space = "\r\n";  
        SERVER_RESPONSE = server_response_first_line+server_response_header_fields+space+resource_data;
    }

    



   


