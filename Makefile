CPP=g++
CFLAGS=
CPPFLAGS= -std=c++0x

FUNFILES=main.o ui.o jsoncpp.o game.o curl.o gameinfo.o
OBJFILES=main_game.o   client_server.o game.o gameinfo.o box.o lua_shell.o
SRVFILES=main_server.o client_server.o server.o

INCLUDES=-Ilua-5.3.1/src/

LIBS=-lncurses -Llua-5.3.1/src/ -llua -L. -lcurl

all: weeblyfun
	

clean: 
	rm *.o

funserver: $(SRVFILES)
	$(CPP) $(CPPFLAGS) -o funserver $(SRVFILES) $(LIBS)

multistuff: multi.o
	$(CPP) $(CPPFLAGS) -o multistuff multi.o

weeblyfun: $(FUNFILES)
	$(CPP) $(CPPFLAGS) -o weeblyfun $(FUNFILES) $(LIBS)

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDES)
