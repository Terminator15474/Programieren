all: cleanall compile run

cleanall:
	@rm -f *.o *.exe

compile:
	@gcc -o ascii_art.exe ascii_art.c lib\libz.a

run:
	@ascii_art.exe testimage.png

debug:
	@gcc ascii_art.c lib\libz.a -o ascii_art.exe -g
	@ascii_art.exe testimage.png