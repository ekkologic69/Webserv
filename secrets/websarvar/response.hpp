#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include "webserv.hpp"
//  class request;
#define BUFF_SIZE 30000
// #include "request/prequest.hpp"
class request;

class response
{
    public:
     char buffer[BUFF_SIZE];
      std::ifstream file;
      std::string _buffer;
     bool _isOpen;
     std::string _body;
    bool            _headerSent;
    std::string     _headers;
    std::string     _Content_Lenght;
    std::string     _Content_Type;
    std::string             _statuscode;
     int            file_size;
     bool           _isDone;
    // std::ifstream _file;
    

    public:
    response():_isOpen(false),_headerSent(false){};
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
    void    set_body(std::string body);
    std::string get_body();
    std::string Error_Body(std::string error);

    void setContentType(request &req);
    std::string getContentLenght();
    std::string getContentType();
    std::string getStatusCode();

    void isfileopen(bool isopen);
    void isheadersdone(bool headerSent);
     void bodyisDone(bool isDone);



void    GetMethod(request &req);
 void    SendResponse(int sck);
};
#endif