SRCS	= srcs/main.c srcs/table.c srcs/data.c
OBJS	= $(SRCS:.c=.o)
NAME	= thread_pool_join
INCL	= includes
RPRT	= queue_report

.c.o :
	cc -c $< -o $(<:.c=.o) -I $(INCL)

$(NAME) : $(OBJS)
	cc -o $(NAME) $(OBJS)

all: $(NAME)

test : $(NAME)
	rm $(RPRT)
	time ./$(NAME) >> $(RPRT)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
