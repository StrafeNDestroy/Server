#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream> 
#include <regex>
#include <fstream> //file reads and writes NOT GOOD FOR IMAGES
#include "json.hpp"
 
 // image processing 


using json = nlohmann::json;

json Status_Codes()
{
    std::ifstream file("../../website/status_codes/status_codes.json");
    json STATUS_CODES = json::parse(file);
    return STATUS_CODES;
}
std::vector <std::string> StringToSingleLines(const std::string& http_request_message)
    {
        
        std::vector<std::string> split_http_request;
        std::istringstream iss(http_request_message);
        std::string line;
        while (std::getline(iss, line))
        {
           split_http_request.push_back(line);
        }
        return split_http_request;
    }
std::vector<std::vector<std::string>> Grab_First_Line(std::vector<std::string> split_http_request)
{
    std::vector<std::vector<std::string>> array;
    std::vector<std::string> first_line;
    first_line.push_back(split_http_request[0]);
    split_http_request.erase(split_http_request.begin());

    array = {first_line, split_http_request};

    return array;
}
std::vector<std::string> Check_First_Line(std::vector<std::vector<std::string>> array)
{
    
    std::vector<std::string> first_line_matches;
    
    std::vector <std::string> first_line = array[0];
    std::smatch matches;
    std::string methods = R"((connect|delete|get|head|options|post|put|trace))";
    std::string http_version = R"((HTTP/1.1|HTTP/2|HTTP/3))";
    std::string resource_page = R"((.+))";
    std::string pattern_str = methods+" " + resource_page + " "+ http_version;
    std::regex pattern(pattern_str,std::regex_constants::icase);

     if(std::regex_search(first_line[0],matches,pattern))
        {
            first_line_matches.push_back(matches[1]);// method
            first_line_matches.push_back(matches[2]); // resource
            first_line_matches.push_back(matches[3]); // http version
            return first_line_matches;

        }
        else
        {
            // http_response(404)
        }
        
}   
std::vector<std::vector<std::string>> Split_Headers_Body(const std::vector<std::string> headers_body)
{
    std::vector<std::vector<std::string>> array;  
    std::vector<std::string> headers; 
    std::vector<std::string> body;
    int index = 0;
    for (index; index < headers_body.size(); index++)
    {
        if (headers_body[index] == R"(/r)")
        {
            break;
        }
    }
    headers.insert(headers.end(), headers_body.begin(), headers_body.begin() + (index-1)); // /r not included
    headers_body.erase(headers_body.begin(), headers_body.begin() + index); // removal up to  /r
    

    array.push_back(headers);
    array.push_back(headers_body); 

    return array;
}
std::unordered_map <std::string,std::string> Check_Header_Fields(std::vector<std::vector<std::string>> array)
{
    std::vector<std::string> headers = array[0];
    std::unordered_map <std::string,std::string> header_fields;

    // WS = WhiteSpace, hf = Header Field
        std::regex ws_error_pattern(R"(([a-zA-Z]+\s+:))");
        std::regex hf_pattern(R"(([a-zA-Z-]+):(.+))");
        
        for(std::string element : headers)
        {
            if (std::regex_search(element,ws_error_pattern))
            {
                // Send 400 (Bad Request), Stop adding to hash and send 
               
            }
            else
            {
                // FIX ISSUES WITH MATCHING
                std::smatch matches;
                std::regex_search(element,matches,hf_pattern);
                header_fields.insert({matches[1],matches[2]});                        
            }
        } 
    return header_fields;
}
bool User_Access(std::unordered_map <std::string,std::string> header_fields)
{
    // some shit here
}
bool ChecK_For_Body(std::unordered_map <std::string,std::string> header_fields)
{
    bool is_body;
    if((header_fields.count("Content-Length") || header_fields.count("Transfer-Encoding")) >= 1 )
    {
        bool is_body = 1;
        return is_body;
    }

    return is_body = 0;

}
// This is where I am working from TODOOOOOOOOO
std::string Check_Extension(std::vector<std::string> first_line_matches) 
{
   std::string extension; 
    std::regex extension_pattern(R"((\.(\w+)$))");  // Matches the file extension preceded by a dot
    std::smatch match;

    if (std::regex_search(first_line_matches[1], match, extension_pattern)) 
    {
        extension = match[1]; // Extract the captured extension
        return extension;
    }
    return extension = "";
}

char* Image_Convert_to_bytes()
{

}

char* Get_Resource(std::vector<std::string> first_line, std::string extension)
{   
    
    std::string head_path = ".././website";
    std::string resource_path = first_line[1];
    if(extension == "")
    {
        // do nothing for now
    }

    else if(extension == "jpg"|"jpeg"|"png"|"gif"|"bmp"|"ico")
    {
        
        
        //fetch image
            // if not found return error
        // image to bytes conversion function
        //return image bytes
    }

    else if(extension == "js"|"css")
    {
        //fetch image
            // if not found return error
        //convert to bytes
        //return bytes
    }

    else
    {
        // return Invalid extension
    }
    


char* Build_Server_Response()
{
    // putting some words in here
}




    // Helper to convert js,css
    // Helper to Convert Images to bytes


    
}







