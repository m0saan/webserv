SRC = main.cpp src/response.cpp
CMP = clang++ -std=c++98
FLAGS = -Wall -Wextra -Werror  -g3
NAME = a.out
all : $(NAME) 

$(NAME):
	$(CMP) $(FLAGS) $(SRC) -o a.out

clean:
	rm a.out
re: clean all