#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include "webserv.hpp"
//  class request;
#include "request/prequest.hpp"
class response
{
    private:
     char *buff;
     int fd;
    // std::string     _path;
    int            headers;
    std::string     _headers;
    std::string     _Content_Lenght;
    std::string     _Content_Type;
    std::string             _statuscode;
     int            file_size;
     bool           isDone;
    // std::ifstream _file;
    

    public:
    response()
    {
        headers = 0;       
    };
    ~response(){};
    void    SetStatusCode(std::string _status)
    {
        _statuscode = _status;
    };
    void setContentLenght(request &req);
    void    set_get_con_type(request &req);
    std::string getExtensionFromURI(std::string uri);
    std::string FinalString(request &req);
    std::string get_Body();
    int get_file_size();
    void Send(int sck,request &req);
    void setContentType(request &req)
    {
        std::map<std::string, std::string> con_type = req.getHeaders();
        std::string Content_type =  con_type["content-type"];
        _Content_Type += "Content-type: " + Content_type + "\r\n";
    };
    int get_fd()
    {
        return this->fd;
    };
    std::string getContentLenght()
    {
        return _Content_Lenght;
    };
    std::string getContentType()
    {
        return _Content_Type;
    };
    std::string getStatusCode()
    {
        return this->_statuscode;
    };

    bool getIsDone()
    {
        return this->isDone;
    };


void    setGet_Body(request &req);
// std::string ChunkBody(std::string body)
// {
//     int fd = 
// }

void    GetMethod(location_obj &location,request &req);
 void    SendResponse(int sck);
};
#endif