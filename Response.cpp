#include "response.hpp"

// void   response::GetMethod(location_obj &location,request &req)
// {
//     DIR *dir = opendir(req.getLocPath().c_str());
//      if(dir)
//      {
//         if(location.get_index().empty()) 
            
//             if(location.get_cgi() && !access(req.getLocPath().c_str(),X_OK))
//             {

//                 //  send cgi
//             }
//             else if(!access(req.getLocPath().c_str(),R_OK))
//              {
//                 //send index;
//                 req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//                 req._res->set_get_con_type(req);
//                 req._res->setContentLenght(req);
//              }   
//          else if(location.get_auto_index())
//          {
//             //send autoindex
//             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//          }
//         else
//         {
//             closedir(dir);
//             //send 403;
//             req._res->SetStatusCode("HTTP/1.1 403 Forbidden\r\n");
//         }
//      }
//     if(access(req.getLocPath().c_str(),F_OK) == 0)
//     {
//         if(!access(req.getLocPath().c_str(),R_OK))
//         {
//             //send file
//             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//             req._res->set_get_con_type(req);
//             req._res->setContentLenght(req);
//         }
//         else
//         {
//             // send 404
//             req._res->SetStatusCode("HTTP/1.1 404 Not Found\r\n");
//         }
//     }
        
// }

// void    SendResponse(int sck,request &req)
// {
//   std::map<std::string, std::string> con_type = req.getHeaders();
//   std::string Content_type =  con_type['Contnt-type'];
// }

std::string response::getExtensionFromURI(std::string uri) {
    size_t dotPos = uri.rfind('.');
    if (dotPos != std::string::npos && dotPos + 1 < uri.length()) {
        return uri.substr(dotPos + 1);
    } else {
        return "";
    }
}

void    response::set_get_con_type(request &req)
{
    std::string extension = getExtensionFromURI(req.getUri());
    if (extension == "html" || extension == "htm") {
        _Content_Type = "Content-Type: text/html\r\n";
    } else if (extension == "css") {
        _Content_Type = "Content-type: text/css\r\n";
    } else if (extension == "js") {
        _Content_Type = "Content-type: application/javascript\r\n";
    } else if (extension == "jpg" || extension == "jpeg") {
        _Content_Type = "Content-type: image/jpeg\r\n";
    } else if (extension == "png") {
        _Content_Type = "Content-type: image/png\r\n";
    } else if (extension == "gif") {
        _Content_Type = "Content-type: image/gif\r\n";
    } else if (extension == "txt") {
        _Content_Type = "Content-type: text/plain\r\n";
    } else if (extension == "xml") {
        _Content_Type = "Content-type: text/xml\r\n";
    } else if (extension == "mp3") {
        _Content_Type = "Content-type: audio/mpeg\r\n";
    } else if (extension == "mp4") {
        _Content_Type = "Content-type: video/mp4\r\n";
    } else if (extension == "avi") {
        _Content_Type = "Content-type: video/x-msvideo\r\n";
    } else if (extension == "mpeg") {
        _Content_Type = "Content-type: video/mpeg\r\n";
    } else if (extension == "pdf") {
        _Content_Type = "Content-type: application/pdf\r\n";
    } else if (extension == "zip") {
        _Content_Type = "Content-type: application/zip\r\n";
    } else if (extension == "gz") {
        _Content_Type = "Content-type: application/x-gzip\r\n";
    } else if (extension == "tar") {
        _Content_Type = "Content-type: application/x-tar\r\n";
    } else if (extension == "ico") {
        _Content_Type = "Content-type: image/x-icon\r\n";
    } else {
        _Content_Type = "Content-type: application/octet-stream\r\n";
    }
}

    void response::setContentLenght(request &req)
    {
        // std::map<std::string, std::string> con_type = req.getHeaders();
        // std::string Content_type =  con_type["content-length"];
        // _Content_Lenght += "Content-Length: " + Content_type + "\r\n";
        std::ifstream file(req.getLocPath(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size >>>>>>>>>>>>>>>>>" << file_size << std::endl;
        file.close();
        this->_Content_Lenght += "Content-Length: " + std::to_string(file_size) + "\r\n";
    };
int response::get_file_size()
{
    return this->file_size;
}
// void   response::setGet_Body(request &req)
// {
//     int fd;
//     fd = open(req.getLocPath().c_str(),O_RDONLY);
//     if(fd == -1)
//     {
//         perror("open");
//         exit(1);
//     }
//     this->buff = new char[1025]();
//     int ret;
//     ret = read(fd,buff,1024);
//     // if(ret > 0)
//         // this->_buffer += buff;
    
//     close(fd);
// }



std::string response::FinalString(request &req)
{
    
    this->isDone = false;
    
    this->fd = open(req.getLocPath().c_str(),O_RDONLY);
    // if(fd == -1)
    // {
    //     perror("open");
    //     exit(1);
    // }
    this->buff = new char[2049]();
    int ret;
    ret = read(this->fd,this->buff,2048);
    //  if(ret == 0)
    //     this->isDone = true;
    
    close(this->fd);
    std::string res;
    // if(this->headers == 0){
    res = req._res->getStatusCode();
    res += req._res->getContentType();
    res += req._res->getContentLenght();
    res += "\r\n\r\n";
    // this->headers = 1;
    // }
    // if(this->headers == 1)
        res += std::string(this->buff);
    
    
    return res;
}

void response::Send(int sck,request &req)
{
    // if(req._res->getIsDone() == false)
    // {
    std::cout << "the final string is : " << req._res->FinalString(req) << std::endl;    
     send(sck, req._res->FinalString(req).c_str(), strlen(req._res->buff), 0);
    // std::cout << "sizeeeeee >>>>>>>>>>>>>>>>>>" <<  test << std::endl;
    req._res->headers = 1;
    delete [] this->buff;
    // }
}















// void    SendResponse(int sck,request &req)
// {
//     std::string res = req._res->getStatusCode();;
//     res += req._res->getContentType();
//     res += req._res->get_Content_Lenght();
//     res += "\r\n";
//     send(sck,res.c_str(),res.size(),0);
// }
// {
   
   

//     send(sck,res.c_str(),res.size(),0);

// }

//      std::string htmlContent =  "<!DOCTYPE html>\r\n"
//                                 "<html>\r\n"
//                                 "<head>\r\n"
//                                   "<title>Welcome to My Website</title>\r\n"
//                                 "</head>\r\n"
//                                 "<body>\r\n"
//                                   "<h1>Welcome to My Website</h1>\r\n"
//                                   "<p>Thank you for visiting our site!</p>\r\n"
//                                 "</body>\r\n"
//                                 "</html>\r\n";
//  std::string res = "HTTP/1.1 200 OK\r\n"
//                       "Content-Type: text/html\r\n"
//                       "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
//             std::cout << res << std::endl;
//             //(void)sck;