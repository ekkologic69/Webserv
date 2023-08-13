#include "response.hpp"

void   response::GetMethod(request &req)
{
    // DIR *dir = opendir(req.getLocPath().c_str());
    //  if(dir)
    //  {
    //     if(req._loc.get_index().empty()) 
    //       {  
    //         if(req._loc.get_cgi() && !access(req.getLocPath().c_str(),X_OK))
    //         {

    //             std::cout << "cgu" << std::endl;
    //         }
    //         else if(!access(req.getLocPath().c_str(),R_OK))
    //          {
    //             //send index;
    //             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
    //             req._res->set_get_con_type(req);
    //             req._res->setContentLenght(req);
    //             set_body(FinalString(req));
    //          }   
    //      else if(req._loc.get_auto_index())
    //      {
    //         //send autoindex
    //         req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
    //      }
    //     else
    //     {
    //         closedir(dir);
    //         //send 403;
    //         //req._res->SetStatusCode("HTTP/1.1 403 Forbidden\r\n");
    //         set_body("403 Forbidden");
    //     }
    //  }
    //  }
    if(access(req.getLocPath().c_str(),F_OK) == 0)
    {
        if(!access(req.getLocPath().c_str(),R_OK))
        {
            //send file
            req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
            req._res->set_get_con_type(req);
            req._res->setContentLenght(req);
            set_body(req._res->FinalString(req));
        }
        // else
        // {
        //     // send 404
        //     //req._res->SetStatusCode();
        //     set_body(req._res->Error_Body("HTTP/1.1 404 Not Found\r\n"));
        // }
    }
        
}

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
        _Content_Type = "Content-Type: text/css\r\n";
    } else if (extension == "js") {
        _Content_Type = "Content-Type: application/javascript\r\n";
    } else if (extension == "jpg" || extension == "jpeg") {
        _Content_Type = "Content-Type: image/jpeg\r\n";
    } else if (extension == "png") {
        _Content_Type = "Content-Type: image/jpeg\r\n";
    } else if (extension == "gif") {
        _Content_Type = "Content-Type: image/gif\r\n";
    } else if (extension == "txt") {
        _Content_Type = "Content-Type: text/plain\r\n";
    } else if (extension == "xml") {
        _Content_Type = "Content-Type: text/xml\r\n";
    } else if (extension == "mp3") {
        _Content_Type = "Content-Type: audio/mpeg\r\n";
    } else if (extension == "mp4") {
        _Content_Type = "Content-Type: video/mp4\r\n";
    } else if (extension == "avi") {
        _Content_Type = "Content-Type: video/x-msvideo\r\n";
    } else if (extension == "mpeg") {
        _Content_Type = "Content-Type: video/mpeg\r\n";
    } else if (extension == "pdf") {
        _Content_Type = "Content-Type: application/pdf\r\n";
    } else if (extension == "zip") {
        _Content_Type = "Content-Type: application/zip\r\n";
    } else if (extension == "gz") {
        _Content_Type = "Content-Type: application/x-gzip\r\n";
    } else if (extension == "tar") {
        _Content_Type = "Content-Type: application/x-tar\r\n";
    } else if (extension == "ico") {
        _Content_Type = "Content-Type: image/x-icon\r\n";
    } else {
        _Content_Type = "Content-Type: application/octet-stream\r\n";
    }
}

    void response::setContentLenght(request &req)
    {
        std::ifstream file(req.getLocPath(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        file.close();
        this->_Content_Lenght += "Content-Length: " + std::to_string(file_size) + "\r\n";
    };
int response::get_file_size()
{
    return this->file_size;
}


std::string response::FinalString(request &req)
{
    std::string res;
    if(!this->_isOpen)
    {
        this->file.close();
        this->file.open(req.getLocPath().c_str(), std::fstream::binary);
        std::cout << "file path >>" << req.getLocPath() << std::endl;
        if(!file.is_open())
        {
            //res = Error_Body("HTTP/1.1 404 Not Found\r\n");
            std::cout << "file not open" << std::endl;
            return res;
        }
        else
            this->isfileopen(true);
    }
    if(this->_isOpen)
    {
        if(this->_headerSent == false){
        std::cout << "header" << std::endl;
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
       std::cout << res << std::endl;
        this->_headerSent = true;   
        }
        
        if(this->_headerSent == true){
            std::cout << "header true " << std::endl;
            this->file.read(this->buffer, sizeof(this->buffer));
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             {  std::cout << "bytes read " << bytesRead << std::endl;
                std::string chunk = std::string(this->buffer,bytesRead);
                //std::cout << "chunk"<< chunk << std::endl;
                res.append(chunk);
				this->isfileopen(true);
                
            }
            else
            {std::cout << "body done " << std::endl;
                this->bodyisDone(true);
                this->file.close();
                this->isfileopen(false);
            }
        }
    }
    
    return res;
}

void response::Send(int sck,request &req)
 {
        std::cout << "body is done " << req._res->_isDone << std::endl;
     if(req._res->_isDone == false)
     {
        std::string res = req._res->get_body();
        send(sck,res.c_str(), res.size(), 0);
     }
}


void    response::set_body(std::string body)
{
    this->_body = body;
}

std::string response::get_body()
{
    return this->_body;
}

std::string response::Error_Body(std::string error)
{
    std::string body = "<html>\r\n"
                        "<head><title>" + error + "</title></head>\r\n"
                        "<body bgcolor=\"white\">\r\n"
                        "<center><h1>" + error + "</h1></center>\r\n"
                        "<hr><center>Web Server</center>\r\n"
                        "</body>\r\n"
                        "</html>\r\n";
    return body;
}
void response::setContentType(request &req)
{
        std::map<std::string, std::string> con_type = req.getHeaders();
        std::string Content_type =  con_type["content-type"];
        _Content_Type += "Content-type: " + Content_type + "\r\n";
}

std::string response::getContentLenght()
{
       return _Content_Lenght;
}
 std::string response::getContentType()
{
    return _Content_Type;
}
std::string response::getStatusCode()
{
       return this->_statuscode;
}

void response::isfileopen(bool isopen)
{
    this->_isOpen = isopen;
}

void response::isheadersdone(bool headerSent)
{
    this->_headerSent = headerSent;
}

void response::bodyisDone(bool isDone)
{
   this->_isDone = isDone;
}
