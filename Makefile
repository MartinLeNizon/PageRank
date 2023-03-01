exe: source.c
	@gcc source.c -O3 -Wall -Werror -o exe

clean:
	@rm -f exe