NAME		=serenity

SRCDIR		=./src/
SRCFILES	=\
		 start.c\
		 game.c\
		 assets.c\
		 inter.c\
		 actions.c\
		 player.c\
		 map.c\
		 menus.c\
		 display.c\
		 anim.c\
		 list.c\
		 tools.c\

OBJDIR		=./obj/
OBJ		=$(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

INCDIR		=./\
		./src/
INC		=$(addprefix -I, $(INCDIR))


MACOSFLAGS = -Wno-empty-body -Wno-unused-value


all:		obj serenity
obj:
		mkdir -p obj
serenity:	$(OBJ)
		gcc $^ -o $(NAME) -lncurses
$(OBJDIR)%.o:	$(SRCDIR)%.c
		gcc -c $^	-o $@	$(INC)

clean:
		rm -rf obj
fclean:		clean
		rm -f serenity serenity.exe serenity.exe.stackdump
re:		fclean all
