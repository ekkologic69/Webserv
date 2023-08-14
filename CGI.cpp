#include "response.hpp"

std::string get_query_string(std::string uri) {
    size_t questionMarkPos = uri.find('?');
    if (questionMarkPos != std::string::npos && questionMarkPos + 1 < uri.length()) {
        return uri.substr(questionMarkPos + 1);
    } else {
        return "";
    }
}
void    response::setEnv(request &req)
{
    std::map<std::string, std::string>    headers = req.getHeaders();

    this->_env.push_back(std::string("REQUEST_METHOD=") + req.getMethod());
    this->_env.push_back("SERVER_PORT=" + std::to_string(1050));
    this->_env.push_back(std::string("HTTP_PORT=") + std::to_string(1050));
    this->_env.push_back(std::string("SCRIPT_NAME=") + req.getLocPath());
    this->_env.push_back(std::string("SCRIPT_FILENAME=") + req.getLocPath());
    this->_env.push_back(std::string("PATH_INFO=") + req.getLocPath());
    this->_env.push_back(std::string("QUERY_STRING=") + get_query_string(req.getUri()));
    this->_env.push_back(std::string("REDIRECT_STATUS=200"));
    this->_env.push_back(std::string("SERVER_HOST=") + "127.0.0.1");
    this->_env.push_back(std::string("HTTP_HOST=") + "127.0.0.1");
    this->_env.push_back(std::string("SERVER_PROTOCOL=HTTP/1.1"));
    this->_env.push_back(std::string("USER_AGENT=") + headers["user-agent"]);
    this->_env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));

    if(req.getMethod() == "POST")
    {
        this->_env.push_back(std::string("CONTENT_TYPE=") + headers["content-type"]);
        this->_env.push_back(std::string("CONTENT_LENGTH=") + std::to_string(req.getContentLenght()));
    }
    
}

char** response::env_to_char()
{
    char **env = new char*[this->_env.size() + 1];
    int i = 0;

    for (std::vector<std::string>::iterator it = this->_env.begin(); it != this->_env.end(); it++)
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
    std::cout << "ext :: " << ext << std::endl;
    if(ext == "php"){
        res = "./php-cgi";
        return res;
    }
    else if(ext == "py"){
        res = "./python-cgi";
        return res;
    }
    else
        res = "";
    return res;

}
    std::string generateRandomFileName(const std::string& extension) {
    const char* characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int length = 10; // Change the length as needed
    std::string randomName;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < length; ++i) {
        randomName += characters[rand() % (sizeof(characters) - 1)];
    }

    randomName += extension;
    return randomName;
}

std::string  response::cgi_exec(request &req)
{
    int fd_in = 0;
    int fd_out;
    std::string res;
    std::string cgi =  generateRandomFileName(".txt");
    std::cout << "cgi :: " << cgi << std::endl;
    // if (req.getMethod() == "POST")
    //     fd_in = open("/tmp/post.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    fd_out = open(cgi.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    std::cout << "fd_out :: " << fd_out << std::endl;
    setEnv(req);
    char **env = env_to_char();
    // if(req.getMethod() == "POST")
    // {
        
    // }
    // lseek(fd_in, 0, SEEK_SET);
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
		 if(dup2(fd_in, 0) == -1)
        {
            perror("dup2_in");
            exit(1);
        }
        if(dup2(fd_out, 1) == -1)
        {
            perror("dup2_out");
            exit(1);
        }
        close(fd_out);
        if (fd_in != 0)
            close(fd_in);
        execve(path.c_str(), arg, env);
            perror("execve");
            exit(1);
    }
    else
    {
        
        std::string line;
        waitpid(this->pid, NULL, 0);

        std::ifstream input_file;
        std::cout << "cgi :: " << cgi << std::endl;
        input_file.open(cgi.c_str());
        res = "HTTP/1.1 200 OK\r\n";
        std::string ext = getExtension(req.getLocPath());
        if (ext == "py")
        {
            res += "Content-Type: text/html\r\n";
            res += "\r\n";
        }

        while(getline(input_file,line))
        {
            std::cout << "line :: " << line << std::endl;
            res += line;
            res += "\n";
        }
        std::cout << "res :: " << res << std::endl;
        input_file.close();
        if (fd_in != 0)
            close(fd_in);
        std::ofstream output_file;
        output_file.open(cgi.c_str());
        output_file << res;
        output_file.close();
    }
    close(fd_out);
    for (size_t i = 0; i < _env.size(); i++)
        delete [] env[i];
    delete [] env;
    if (this->pid == 0)
        exit(0);
    
    return cgi;
    
}

void response::get_cgi_body(std::string &res,request &req)
{
    std::string key;
    std::string value;
    size_t i = 0;
    size_t j = 0;
    size_t position = 0;
    if(res.find("500\r\n") != std::string::npos || res.empty())
    {
        std::cout << "500" << std::endl;
    }
    while((position = res.find("\r\n",i)) != std::string::npos)
    {
        std::cout << "position :: " << position << std::endl;
        if(position == i)
            break;
        key = res.substr(i, position - i);
        if((j = key.find(":")) != std::string::npos)
        {
            value = key.substr(0 + j);
            if(value == "Content-Type")
                req._res->setContentTypeCgi(key.substr(j));
            else if(value == "Content-Length")
                req._res->setContentLenghtCgi(key.substr(j));
            else if(value == "Location")
                req._res->setLocationCgi(key.substr(j));
            else if(value == "Status")
                req._res->setStatusCodeCgi(key.substr(j));
            else if(value == "Set-Cookie")
                req._res->setCookieCgi(key.substr(j));
        }
        i = key.size() + 2;
        key.clear();
        value.clear();
    }
    if (position == std::string::npos)
    {
        req._res->setBodyCgi(res);
       req._res->setContentTypeCgi("text/html");
    }
    else
        req._res->setBodyCgi(res.substr(position + 2));
    size_t content_length = 0;
    std::stringstream ss(req._res->getContentLenghtCgi());
    ss >> content_length;

    if (content_length == 0 && !req._res->getBody().empty())
        req._res->setContentLenghtCgi(std::to_string(req._res->getBody().size()));
}

void response::setContentLenghtCgi(std::string body)
{
    this->Content_Lenght_cgi = body;
}
void response::setLocationCgi(std::string body)
{
    this->Location_cgi = body;
}
void response::setCookieCgi(std::string body)
{
    this->Cookie_cgi = body;
}
void response::setStatusCodeCgi(std::string body)
{
    this->statuscode_cgi = body;
}
std::string response::getContentLenghtCgi()
{
    return this->Content_Lenght_cgi;
}
std::string response::getLocationCgi(){ 
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
void response::setContentTypeCgi(std::string body)
{
    this->Content_Type_cgi = body;
}
std::string response::getContentTypeCgi()
{
    return this->Content_Type_cgi;
}
void    response::setBodyCgi(std::string body)
{
    this->body_cgi = body;
}
std::string response::getBody()
{
    return this->body_cgi;
}


std::string response::serveCgi(request &req)
{
    
   std::string res;
    if(!this->_isOpen)
    {
       std::string path = req._res->cgi_exec(req);
        std::cout << "path :: " << path << std::endl;
        this->file.open(path.c_str(), std::ios::in);
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