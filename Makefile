CPP=g++
CFLAGS=
CPPFLAGS=

OBJFILES=main.o game.o player.o gameinfo.o

all: weeblyfun
	

clean: 
	rm *.o

weeblyfun: $(OBJFILES)
	$(CPP) -o weeblyfun $(OBJFILES) -lncurses

%.o: %.cpp %.h
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
