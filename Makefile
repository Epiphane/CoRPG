CPP=g++
CFLAGS=
CPPFLAGS= -std=c++0x

OBJFILES=main_game.o game.o gameinfo.o box.o lua_shell.o
SRVFILES=main_server.o server_lock.o

INCLUDES=-Ilua-5.3.1/src/

LIBS=-lncurses -Llua-5.3.1/src/ -llua

all: funserver weeblyfun
	

clean: 
	rm *.o

funserver: $(SRVFILES)
	$(CPP) $(CPPFLAGS) -o funserver $(SRVFILES) $(LIBS)

multistuff: multi.o
	$(CPP) $(CPPFLAGS) -o multistuff multi.o

weeblyfun: $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o weeblyfun $(OBJFILES) $(LIBS)

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< $(INCLUDES)
