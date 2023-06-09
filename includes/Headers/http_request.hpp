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
std::pair<std::string, std::string> Fetch_File(std::string resource_path,std::string extension);

// PICTURE SECTIONS
std::pair<std::vector<char>,int> read_image_to_binary(std::string resource_path);
// Convert image binary to 64base
std::string BinarytoBase64(const std::vector<char>& image_data);

// Build Reponse 
std::pair<std::string, int>  Build_FirstLine_Headers_HTTP_Image_Reponse(std::vector<char> fetched_image_binary,std::string extension);
std::pair<std::string, int>  Build_Correct_HTTP_Reponse_File(std::string file_reponse,std::string file_size,std::string extension);




// Constructor
public: 
    // PUBLIC ATTRIBUTE
    // Helper to tell if its media or a File
    std::string EXTENSION;

    // FILE RESPONSES
    std::string FILE_HTTP_REPONSE;
    int FILE_HTTP_REPONSE_SIZE;

    //IMAGE RESPONSES
    std::string IMAGE_FIRSTLINE_HEADERS_REPONSE;
    int IMAGE_FIRSTLINE_HEADERS_REPONSE_SIZE;
    std::vector<char> IMAGE_BINARY_RESPONSE;
    int IMAGE_BINARY_RESPONSE_SIZE;
    HTTP_REQUEST(const std::string& http_request_message);
};



#endif