NAME        = codexion
CC          = cc
RM          = rm -rf
CFLAGS      = -Wall -Wextra -Werror -pthread -g3
INCDIR      = includes
SRCDIR      = srcs
HEADERS     = includes/codexion.h
OBJDIR      = .obj

SRCS        = $(SRCDIR)/main.c \
              $(SRCDIR)/coder.c \
              $(SRCDIR)/monitor.c \
              $(SRCDIR)/dongle.c \
              $(SRCDIR)/scheduler.c \
              $(SRCDIR)/init/parsing.c \
              $(SRCDIR)/init/init.c \
              $(SRCDIR)/init/utils.c \
              $(SRCDIR)/utils/time.c \
              $(SRCDIR)/utils/queue.c \
              $(SRCDIR)/utils/log.c \
              $(SRCDIR)/utils/check.c \

OBJS        = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS        = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
