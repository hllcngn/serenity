MACOSFLAGS = -Wno-empty-body -Wno-unused-value

serenity:
	gcc -oserenity src/*.c -Isrc -lncurses
re:	clean serenity
clean:
	-rm a.out serenity serenity.exe serenity.exe.stackdump
.PHONY: serenity
