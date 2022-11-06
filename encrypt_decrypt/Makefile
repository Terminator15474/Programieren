all: setup start
start:
	encrypt.exe testfile.testfile
	encrypt.exe testfile.testfile.encrypted 

setup:
	rm testfile.*
	cp testfile_backup\testfile.testfile .
	gcc encrypt.c -o encrypt.exe
