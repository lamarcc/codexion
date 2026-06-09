NAME	= codexion
CC		= cc
RM		= rm -rf
CFLAGS	= -Wall -Wextra -Werror -pthread -g3
INCDIR	= includes
SRCDIR	= srcs
HEADERS	= includes/codexion.h
OBJDIR	= .obj

SRCS	=	$(SRCDIR)/main.c \
			$(SRCDIR)/running.c \
			$(SRCDIR)/parser/parsing.c \
			$(SRCDIR)/parser/init.c \
			$(SRCDIR)/parser/utils.c \
			$(SRCDIR)/routine/thread_routine.c \
			$(SRCDIR)/routine/dongle.c \
			$(SRCDIR)/routine/queue.c \
			$(SRCDIR)/log.c \
			$(SRCDIR)/time.c \
			$(SRCDIR)/utils.c \

OBJS	= $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
