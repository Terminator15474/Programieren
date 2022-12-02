all: compile run

compile:
	@gcc ascii_art.c lib\libz.a -o ascii_art.exe

run:
	@ascii_art.exe testimage.png

debug:
	@gcc ascii_art.c lib\libz.a -o ascii_art.exe -g
	@ascii_art.exe testimage.png