EXEC=lynx
CC=g++
DEBUGFLAG=-O0 -g #-O2 -Os -march=native -pipe
LINK=-lgtest
OPTION=-std=c++11  
WARNINGFLAG=

ifeq ($(WARNING),1)
WARNINGFLAG=-Wall -Wextra
endif

FILE=main.o

$(EXEC): $(FILE) 
	$(CC) $(OPTION) $(FILE) -o ../../build/$@ $(LINK) $(DEBUGFLAG) $(WARNINGFLAG)

%.o : %.cpp
	$(CC) $(OPTION) -c $< -o $@ $(DEBUGFLAG) $(WARNINGFLAG)

clean:
	rm -f *.o ../../build/$(EXEC) 
