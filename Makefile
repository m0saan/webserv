# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbani <mbani@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/18 18:15:22 by mbani             #+#    #+#              #
#    Updated: 2021/12/18 18:19:06 by mbani            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.cpp src/parser.cpp src/request.cpp src/request_response.cpp src/response.cpp src/server.cpp src/sockets.cpp
NAME = webserv
FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(SRCS)
	@clang++ $(FLAGS) $(SRCS) -o $(NAME)

run:
	@ ./$(NAME) config_files/multiple-servers.config

clean :
	@rm -rf $(NAME)

fclean : clean

re : fclean all 
