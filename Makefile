NAME	= codexion
CC		= cc
RM		= rm -rf
CFLAGS	= -Wall -Wextra -Werror -pthread -g
INCDIR	= includes
SRCDIR	= srcs
SCHDIR	= scheduler
UTILS	= utils
HEADERS	= includes/codexion.h
OBJDIR	= .obj

SRCS	=	$(SRCDIR)/main.c \
			$(SRCDIR)/parsing.c \
			$(SRCDIR)/init.c \
			$(SRCDIR)/coder.c \
			$(SRCDIR)/monitor.c \
			$(SRCDIR)/dongle.c \
			$(SCHDIR)/heap.c \
			$(UTILS)/utils.c

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
