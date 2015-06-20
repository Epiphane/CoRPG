CPP=g++
CFLAGS=
CPPFLAGS=

OBJFILES=main.o game.o player.o gameinfo.o box.o lua_shell.o

all: weeblyfun
	

clean: 
	rm *.o

weeblyfun: $(OBJFILES)
	$(CPP) -o weeblyfun $(OBJFILES) -lncurses -Llua-5.3.1/src/ -llua

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< -Ilua-5.3.1/src/
