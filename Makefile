serenity:
	gcc -oserenity src/*.c -Isrc -lncurses
clean:
	rm serenity.exe serenity.exe.stackdump
.PHONY: serenity
