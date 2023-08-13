CXX	=	c++
CXXFLAGS = -I include -std=c++98#-fsanitize=address #-g

NAME = webserv

SRC = src/main.cpp src/Client.cpp src/Config.cpp src/Server.cpp \
	src/WebServer.cpp src/Request.cpp src/Response.cpp src/cgi.cpp
O_FILES = $(SRC:.cpp=.o)

all:$(NAME)

$(NAME): $(O_FILES)
	$(CXX) $(CXXFLAGS)  $(O_FILES) -o $(NAME) -g

clean:
	rm -rf src/*.o

fclean:clean
	rm -rf $(NAME)

push:
	sudo git add .
	sudo git commit -m "$(commit)"
	sudo git push 

re : clean fclean all
