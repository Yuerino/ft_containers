NAME	= vector

CC		= c++
# https://stackoverflow.com/questions/12606713/enforcing-the-c98-standard-in-gcc
CFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic-errors

SRCS	= main.cpp

INC		= vector.hpp iterator.hpp type_traits.hpp utility.hpp algorithm.hpp

RM		= rm -f

.PHONY: all clean re assert debug

$(NAME): $(SRCS) $(INC)
		$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

all: $(NAME)

assert: CFLAGS = -Wall -Wextra -Werror -D ASSERT_TEST
assert: re

debug: CFLAGS = -Wall -Wextra -Werror -D ASSERT_TEST -D DEBUG
debug: re

clean:
		@$(RM) $(NAME)

re: clean all
