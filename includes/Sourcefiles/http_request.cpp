#include "../Headers/http_request.hpp"



HTTP_REQUEST::HTTP_REQUEST(const std::string& http_request_message)
{
        HTTP_REQUEST_MESSAGE = http_request_message;
        std::vector <std::string> spilt_http_request = StringToSingleLines(HTTP_REQUEST_MESSAGE);
        std::pair<std::vector<std::string>,std::vector<std::string>> http_request_sections = Grab_First_Line(spilt_http_request);
        std::vector<std::string> first_line = http_request_sections.first;
        std::vector<std::string> first_line_matches = Check_First_Line(first_line);
        std::string resource_path = first_line_matches[1];
        std::vector<std::string> headers_and_body = http_request_sections.second;
        std::pair<std::vector<std::string>,std::vector<std::string>> split_headers_and_body = Split_Headers_And_Body(headers_and_body);
        std::vector<std::string> headers = split_headers_and_body.first;
        std::vector<std::string> body = split_headers_and_body.second;
        std::unordered_map <std::string,std::string> header_fields = Check_Header_Fields( headers);
        bool is_body = ChecK_For_Body(header_fields);
        // bool has_access = User_Access(header_fields);
        std::string extension = Extract_Extension(resource_path);
        if (extension  == "" || extension == "js" || extension == "css")
        {   

           std::pair<std::string, std::string> fetched_file = Fetch_File(resource_path);
            std::string file_reponse = fetched_file.first;
            std::string file_size = fetched_file.second;
            std::string server_reponse = Build_Correct_HTTP_Reponse_File( file_reponse, file_size);
            SERVER_RESPONSE = server_reponse;
        
        }


        else
        {
            std::pair<std::vector<char>,std::string> fetched_image = read_image_to_binary(resource_path);
            std::vector<char> image_reponse = fetched_image.first;
            std::string image_size = fetched_image.second; 
            std::string image_base64_reponse = BinarytoBase64(image_reponse);
            std::string server_reponse = Build_Correct_HTTP_Reponse_Image(image_base64_reponse,image_size);
            SERVER_RESPONSE = server_reponse;
        }

}


json Status_Codes()
{
    std::ifstream file("../../website/status_codes/status_codes.json");
    json STATUS_CODES = json::parse(file);
    return STATUS_CODES;
}
std::vector <std::string> HTTP_REQUEST::StringToSingleLines(const std::string& http_request_message)
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
std::pair<std::vector<std::string>,std::vector<std::string>> HTTP_REQUEST::Grab_First_Line(std::vector<std::string> split_http_request)
{
    std::vector<std::string> first_line;
    std::vector<std::string> header_and_body =  split_http_request;


    first_line.push_back(header_and_body[0]);
    header_and_body.erase(header_and_body.begin());

    return {first_line, header_and_body};
}
std::vector<std::string> HTTP_REQUEST::Check_First_Line(std::vector<std::string> http_request_sections)
{
    
    std::vector<std::string> first_line_matches;
    
    std::string first_line = http_request_sections[0];
    std::smatch matches;
    std::string methods = R"((connect|delete|get|head|options|post|put|trace))";
    std::string http_version = R"((HTTP/1.1|HTTP/2|HTTP/3))";
    std::string resource_page = R"((.+))";
    std::string pattern_str = methods+" " + resource_page + " "+ http_version;
    std::regex pattern(pattern_str,std::regex_constants::icase);

     if(std::regex_search(first_line,matches,pattern))
        {
            first_line_matches.push_back(matches[1]);// method
            first_line_matches.push_back(matches[2]); // resource
            first_line_matches.push_back(matches[3]); // http version
            return first_line_matches;

        }
        else
        {
          exit(-1);
        }
        
}   
std::pair<std::vector<std::string>,std::vector<std::string>> HTTP_REQUEST::Split_Headers_And_Body( std::vector<std::string> headers_and_body)
{
    
    std::vector<std::string> headers; 
    std::vector<std::string> body;
    int index = 0;
    for (index; index < headers_and_body.size(); index++)
    {
        if (headers_and_body[index] == R"(/r)")
        {
            break;
        }
    }
    headers.insert(headers.end(), headers_and_body.begin(), headers_and_body.begin() + (index-1)); // /r not included
    headers_and_body.erase(headers_and_body.begin(), headers_and_body.begin() + index); // removal up to  /r

    return {headers,body};
}
std::unordered_map <std::string,std::string> HTTP_REQUEST::Check_Header_Fields(std::vector<std::string> headers)
{
    
    std::unordered_map <std::string,std::string> header_fields;
    // WS = WhiteSpace, hf = Header Field
        std::regex ws_error_pattern(R"(([a-zA-Z]+\s+:))");
        std::regex hf_pattern(R"(([a-zA-Z-]+):(.+))");
        
        for(std::string element : headers)
        {
            if (std::regex_search(element,ws_error_pattern))
            {
                // Build_Error_HTTP_Reponse()
               
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
bool HTTP_REQUEST::User_Access(std::unordered_map <std::string,std::string> header_fields)
{
    /*
    if(access)
    {
        true
    }

    else
    {
        Build_Error_HTTP_Reponse()
        Send Log()
    }
    
    */
   return 0;

}
bool HTTP_REQUEST::ChecK_For_Body(std::unordered_map <std::string,std::string> header_fields)
{
    bool is_body;
    if((header_fields.count("Content-Length") || header_fields.count("Transfer-Encoding")) >= 1 )
    {
        bool is_body = 1;
        return is_body;
    }

    return is_body = 0;

}
std::string HTTP_REQUEST::Extract_Extension(std::string resource_path) 
{
    std::string extension; 
    std::regex extension_pattern(R"((\.(\w+)$))");  // Matches the file extension preceded by a dot
    std::smatch match;

    if (std::regex_search(resource_path, match, extension_pattern)) 
    {
        extension = match[1]; // Extract the captured extension
        if(extension == "jpeg"||"ico"||"png")
        {
            return extension;
        }
        else
        {
            // Build_Error_HTTP_Reponse()
        }
    }



    return extension = "";
}
 std::pair<std::vector<char>,std::string> HTTP_REQUEST::read_image_to_binary(std::string resource_path) 
 {
    // Open the file in binary mode
    std::string head_path = "../../website";
    resource_path = head_path + resource_path;
    std::ifstream file(resource_path, std::ios::binary);

    // Get the size of the file
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) 
    {
        std::stringstream ss;
        ss << buffer.size();
        std::string temp = ss.str();
        const char* image_size = temp.c_str();
        return {buffer,image_size};
    } else 
    {
        throw std::runtime_error("Failed to read file " + resource_path);
    }

 }
std::string HTTP_REQUEST::BinarytoBase64(const std::vector<char>& image_data)
{
    return cppcodec::base64_rfc4648::encode(image_data.data(),image_data.size());

}
std::pair<std::string, std::string> HTTP_REQUEST::Fetch_File(std::string resource_path)
{
    std::string head_path = "../../website";
    if (resource_path == "/")
    {
        resource_path = head_path + "/pages/index.html";

    }

    std::fstream file(resource_path);
    std::string line;
    std::string response_size;
    std::string file_response;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            file_response += line + '\n';
        }
    }
    else
    {
        return {"", ""};
    }

    response_size = std::to_string(file_response.size());

    return {file_response, response_size};
}
std::string HTTP_REQUEST::Build_Correct_HTTP_Reponse_File(std::string file_reponse,std::string file_size)
{
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Content-Type: text/html\r\n";
    responseStream << "Content-Length: " << file_size << "\r\n";
    responseStream << "\r\n";
    responseStream << file_reponse;

    std::string server_reponse = responseStream.str();
    return server_reponse;
}
std::string HTTP_REQUEST::Build_Correct_HTTP_Reponse_Image(std::string image_base64_reponse,std::string image_size)
{
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Content-Type: image/vnd.microsoft.icon\r\n"; // or "Content-Type: image/vnd.microsoft.icon\r\n"
    responseStream << "Content-Transfer-Encoding: base64\r\n"; 
    responseStream << "Content-Length: " << image_size << "\r\n";
    responseStream << "\r\n";
    responseStream << image_base64_reponse;
    std::string server_response = responseStream.str();
    return server_response;
}








































   


