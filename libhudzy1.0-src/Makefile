CC:=g++
libs:=-lgdi32
flags:=-Wall
version:=1.0
outdir:=lib

all:
	@echo You'll have to use target specific commands depending on your toolchain. Check out the inside of the Makefile.

lib64:
	$(CC) $(flags) -m64 -c hudzy_internal.cpp -o libhudzy$(version)x64.o
	ar rvs -o $(outdir)/libhudzy1.0x64.a libhudzy1.0x64.o
	del /Q libhudzy1.0x64.o
lib32:
	$(CC) $(flags) -m32 -c hudzy_internal.cpp -o libhudzy$(version)x32.o
	ar rvs -o $(outdir)/libhudzy$(version)x32.a libhudzy$(version)x32.o
	del /Q libhudzy$(version)x32.o
clean:
	del /Q $(outdir)\libhudzy$(version)x32.a
	del /Q $(outdir)\libhudzy$(version)x64.a
