#include "response.hpp"

void   response::GetMethod(request &req)
{
    DIR *dir = opendir(req.getLocPath().c_str());
     if(dir)
     {
    //     if(req._loc.get_index().empty()) 
    //       {  
    //         if(req._loc.get_cgi() && !access(req.getLocPath().c_str(),X_OK))
    //         {

    //             std::cout << "cgu" << std::endl;
    //         }
    //          if(!access(req.getLocPath().c_str(),R_OK))
    //          {
    //             //send index;
    //             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
    //             req._res->set_get_con_type(req);
    //             req._res->setContentLenght(req);
    //             set_body(FinalString(req));
    //          }  
    //         else
         if(req._loc.get_auto_index())
         {
            //send autoindex
            std::cout << " boooooooooooool "<<req._res->_isDone<<std::endl;
            req._res->autoindex(req);
            req._res->bodyisDone(true);
            //std::cout << "auto index"<<get_autoindex() << std::endl;
         }
    }
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
        if(access(req.getLocPath().c_str(),R_OK) == 0)
        {
            //send file
            req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
            req._res->set_get_con_type(req);
            req._res->setContentLenght(req);
            //req._res->set_body(req._res->FinalString(req));
        }
        // else
        // {
        //     // send 404
        //     //req._res->SetStatusCode();
        //     set_body(req._res->Error_Body("HTTP/1.1 404 Not Found\r\n"));
        // }
    }
        
}


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
        // std::map<std::string, std::string> con_type = req.getHeaders();
        // std::string Content_type =  con_type["content-length"];
        // _Content_Lenght += "Content-Length: " + Content_type + "\r\n";
        std::ifstream file(req.getLocPath(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size is : " << file_size << std::endl;
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
    std::string res;
    if(!this->_isOpen)
    {
        this->file.close();
        this->file.open(req.getLocPath().c_str(), std::fstream::binary);
        std::cout << "file path >>>> " << req.getLocPath() << std::endl;
        if(!file.is_open())
        {
            //set 403
            std::cout << "file not open" << std::endl;
            exit(1);
        }
        else
            this->isfileopen(true);
    }
    if(this->_isOpen)
    {
        if(this->headerSent == false){
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        this->headerSent = true;  
        }
        
        if(this->headerSent == true && !this->_isDone){
            
            this->file.read(this->buffer, 1024);
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				this->isfileopen(true);
            }
            else
            {
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
     std::string res;
     if( (req._res->_isDone == false || req._loc.get_auto_index() == true)){
        std::cout << "req uri >>>>>>>>> " << req.getUri() << std::endl;
        if((req.getUri() == "/"|| req.getUri().empty())&& req._loc.get_auto_index() == true){
             res = req._res->get_autoindex();
                std::cout << "res >>>>>>>>> " << res << std::endl;
        }
    else
        res = req._res->FinalString(req);
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

void    response::autoindex(request &req)
{
    std::string index;
    std::string path = req.getLocPath();
    DIR *dir;
    struct dirent *entry;
    if((dir = opendir(path.c_str())) != NULL)
    {
        while((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
            
            index += "<li><a href=\"" + name + "\">" + name + "</a></li>";
        }
        closedir(dir);
    }
    else
    {
        //404
        exit(1);
    }
    this->_autoindex = "HTTP/1.1 200 OK\r\n";
    this->_autoindex += "Content-Type: text/html\r\n";
    this->_autoindex += "Content-Length: " + std::to_string(index.size() + strlen("<html><body><h1>Directory Listing</h1><ul>") + strlen("</ul></body></html>")) + "\r\n\r\n";
    this->_autoindex += "<html><body><h1>Directory Listing</h1><ul>" + index + "</ul></body></html>";
    this->bodyisDone(true);
}
std::string response::get_autoindex()
{
    return this->_autoindex;
}

void    response::setEnv(request &req)
{
    std::map<std::string, std::string>    headers = req.getHeaders();

    _env.push_back(std::string("REQUEST_METHOD=") + req.getMethod());
    _env.push_back("SERVER_PORT" + std::to_string(1050));
    _env.push_back(std::string("SCRIPT_NAME=") + set_cgi_executable(req));
    _env.push_back(std::string("SCRIPT_FILENAME=") + req.getLocPath());
    _env.push_back(std::string("PATH_INFO=") + req.getLocPath());
    _env.push_back(std::string("REDIRECT_STATUS=200"));
    _env.push_back(std::string("SERVER_HOST=") + headers["Host"]);
    _env.push_back(std::string("SERVER_PROTOCOL=HTTP/1.1"));
    _env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));
    if(req.getMethod() == "POST")
    {
        _env.push_back(std::string("CONTENT_TYPE=") + headers["content-type"]);
        _env.push_back(std::string("CONTENT_LENGTH=") + std::to_string(req.getContentLenght()));
    }
}

char** response::env_to_char()
{
    char **env = new char*[_env.size() + 1];
    int i = 0;

    for (std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        env[i] = new char[(*it).size() + 1];
        env[i] = strcpy(env[i], (*it).c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}
std::string getExtension(std::string uri) {
    size_t dotPos = uri.rfind('.');
    if (dotPos != std::string::npos && dotPos + 1 < uri.length()) {
        return uri.substr(dotPos + 1);
    } else {
        return "";
    }
}
std::string response::set_cgi_executable(request &req)
{
    std::string path = req.getLocPath();
    std::string res;
    std::string ext = getExtension(path);
    if(ext == ".php")
        res = "./php-cgi";
    else if(ext == ".py")
        res = "/usr/bin/python3";
    else
        res = "";

}
void    response::cgi_exec(request &req)
{
    int fd_in ;
    int fd_out;
    std::string res;
    FILE* f_in = tmpfile();
    FILE* f_out = tmpfile();
    fd_in = fileno(f_in);
    fd_out = fileno(f_out);

    char **env = env_to_char();
    if(req.getMethod() == "POST")
    {
         if(write(fd_in, req.getBody().c_str(), req.getBody().size()) == -1)
           {
               perror("write");
               exit(1);
           }
    }
    lseek(fd_in, 0, SEEK_SET);
    std::string path = set_cgi_executable(req);
    this->pid = fork();
    if(this->pid == -1)
    {
        perror("fork");
        exit(1);
    }    
     else if(this->pid == 0)
    {
        char *arg[3]= {(char *)path.c_str(),(char *)req.getLocPath().c_str(), NULL};
        if(!arg[0] || !arg[1])
        {
              // send505;
                exit(1);
        }
        dup2(fd_out, STDOUT_FILENO);
		dup2(fd_out, STDIN_FILENO);
        if(execve(path.c_str(), arg, env) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        waitpid(-1, NULL, 0);
        lseek(fd_out, 0, SEEK_SET);
        char buff[1024];
        while (1)
        {
            memset(buff, 0, 1024);
            if(read(fd_out, buff, 1023) == -1)
                break;
            res.append(buff);
         }
    }
        
    close(fd_out);
    close(fd_in);
    fclose(f_in);
    fclose(f_out);
    for (size_t i = 0; i < _env.size(); i++)
        delete [] env[i];
    delete [] env;
    if (this->pid == 0)
        exit(0);
    req._res->get_cgi_body(res, req);
}

void response::get_cgi_body(std::string &res,request &req)
{
    std::string key;
    std::string value;
    size_t i = 0;
    size_t j = 0;
    size_t position = 0;
    if(res.find("500\r\n") != std::npos || res.empty())
    {
        // send500
    }
    while((position = res.find("\r\n",i)) != std::string::npos)
    {
        if(position == i)
            break;
        key = res.substr(i, position - i);
        if((j = key.find(":")) != std::string::npos)
        {
            value = key.substr(0 + j);
            if(value == "Content-Type")
                req._res->setcontentTypeCgi(key.substr(j));
            else if(value == "Content-Length")
                req._res->setContentLenghtCgi(key.substr(j));
            else if(value == "Location")
                req._res->setLocationCgi(key.substr(j));
            else if(value == "Status")
                req._res->setStatuscodeCgi(key.substr(j));
            else if(value == "Set-Cookie")
                req._res->setCookieCgi(key.substr(j));
        }
        i = key.size() + 2;
        key.clear();
        value.clear();
    }
    if (pos == std::string::npos)
    {
        req._res->setBody(res);
       req._res->setcontentTypeCgi("text/html");
    }
    else
        req._res->setBody(res.substr(pos + 2));

    std::stringstream ss(req._res->getContentLenghtCgi());
    ss >> content_length;

    if (content_length == 0 && !req._res->getBody().empty())
        req._res->setContentLenghtCgi(std::to_string(req._res->getBody().size()));
    req._res->getBodyCgi(req);
}




std::string response::serveCgi(request &req)
{
    std::string res;
    if(!this->_isOpen)
    {
        this->file.close();
        this->file.open(req.getLocPath().c_str(), std::fstream::binary);
        std::cout << "file path >>>> " << req.getLocPath() << std::endl;
        if(!file.is_open())
        {
            //set 403
            std::cout << "file not open" << std::endl;
            exit(1);
        }
        else
            this->isfileopen(true);
    }
    if(this->_isOpen)
    {
        if(this->headerSent == false){
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        this->headerSent = true;  
        }
        
        if(this->headerSent == true && !this->_isDone){
            
            this->file.read(this->buffer, 1024);
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				this->isfileopen(true);
            }
            else
            {
                this->bodyisDone(true);
                this->file.close();
                this->isfileopen(false);
        
            }
        }
    }
    
    return res;
}

void response::setContentLenghtCgi(std::string &body)
{
    this->Content_Lenght_cgi = body;
}
void response::setLocationCgi(std::string &body)
{
    this->Location_cgi = body;
}
void response::setCookieCgi(std::string &body)
{
    this->Cookie_cgi = body;
}
void response::setStatuscodeCgi(std::string &body)
{
    this->statuscode_cgi = body;
}
std::string response::getContentLenghtCgi()
{
    return this->Content_Lenght_cgi;
}
std::string response::getLocationCgi()
{
    return this->Location_cgi;
}
std::string response::getCookieCgi()
{
    return this->Cookie_cgi;
}
std::string response::getStatusCodeCgi()
{
    return this->statuscode_cgi;
}
void response::setContentTypeCgi(std::string &body)
{
    this->Content_Type_cgi = body;
}
std::string response::getContentTypeCgi()
{
    return this->Content_Type_cgi;
}
void    response::setBodyCgi(std::string &body)
{
    this->body_cgi = body;
}
std::string response::getBodyCgi()
{
    return this->body_cgi;
}
void response::getBodyCgi(request &req)
{
    this->body_cgi =  req.getHttpV() + " " + this->getStatusCodeCgi()  + " " + this->getstatusDescription() + "\r\nContent-Length: " + this->getContentLenghtCgi() + "\r\n";
    if(this->Content_Type_cgi.size())
        this->body_cgi += this->getContentTypeCgi() + "\r\n";
    if(!this->Cookie_cgi.empty())
	    this->body_cgi +=  this->getCookieCgi() + "\r\n";
	if(!this->Location_cgi.empty()) 
	    this->body_cgi +=  + this->getLocationCgi() + "\r\n";
    this->body_cgi +=   "\r\n" + this->getBody();
}
