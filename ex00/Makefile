NAME = btc

SRCS = main.cpp BitcoinExchange.cpp

OBJS =  $(SRCS:%.cpp=%.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

############################

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re