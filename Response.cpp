#include "webserv.hpp"

void    Getmethod(request &req)
{
    if(!opendir(req.getUri().c_str()))
    {
        std::cout << "File" << std::endl;
        return;
    }
}