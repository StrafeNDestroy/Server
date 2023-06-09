#ifndef http_protocol
#define http_protocol



#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream> 
#include <regex>
#include <fstream>
#include "json.hpp"
#include <cppcodec/base64_rfc4648.hpp>



using json = nlohmann::json;


class HTTP_REQUEST
{

// Class attributes 
private: 
std::string HTTP_REQUEST_MESSAGE;


json Status_Codes();

// Inital Http request Checks
std::vector <std::string> StringToSingleLines(const std::string& http_request_message);
std::pair<std::vector<std::string>,std::vector<std::string>> Grab_First_Line(std::vector<std::string> split_http_request);
std::vector<std::string> Check_First_Line(std::vector<std::string> http_request_sections);
std::pair<std::vector<std::string>,std::vector<std::string>> Split_Headers_And_Body(const std::vector<std::string> headers_body);
std::unordered_map <std::string,std::string> Check_Header_Fields(std::vector<std::string> headers);
bool User_Access(std::unordered_map <std::string,std::string> header_fields);
bool ChecK_For_Body(std::unordered_map <std::string,std::string> header_fields);
std::string Extract_Extension(std::string resource_path);


// HTML FILE SECTION
std::pair<std::string, std::string> Fetch_File(std::string resource_path);

// PICTURE SECTIONS
std::pair<std::vector<char>,std::string> read_image_to_binary(std::string resource_path);
// Convert image binary to 64base
std::string BinarytoBase64(const std::vector<char>& image_data);

// Build Reponse 
std::string Build_Correct_HTTP_Reponse_Image(std::string image_base64_reponse,std::string image_size);
std::string Build_Correct_HTTP_Reponse_File(std::string file_reponse,std::string file_size);




// Constructor
public: 
    // PUBLIC ATTRIBUTE
    std::string SERVER_RESPONSE;
    HTTP_REQUEST(const std::string& http_request_message);
};



#endif