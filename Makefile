CPP=g++
CFLAGS=
CPPFLAGS= -std=c++0x

FUNFILES=main.o ui.o jsoncpp.o game.o curl.o gameinfo.o js_shell.o duktape.o
OBJFILES=main_game.o   client_server.o game.o gameinfo.o box.o lua_shell.o
SRVFILES=main_server.o client_server.o server.o

INCLUDES=-Ilua-5.3.1/src/ -Icurl-include/

LIBS=-lncurses -Llua-5.3.1/src/ -llua -L. -lcurl
all: weeblyfun
	

clean: 
	rm *.o

weeblyfun: $(FUNFILES)
	$(CPP) $(CPPFLAGS) -o weeblyfun $(FUNFILES) $(LIBS)

duktape.o: duktape.c duktape.h
	gcc -std=c99 -c duktape.c

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDES)

