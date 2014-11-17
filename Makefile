CC=g++
DEBUGFLAG=-O0 
LINK=
OPTION=-g3 -std=c++11
WARNINGFLAG=
EXEC=loutre
ifeq ($(DEBUG),1)
DEBUGFLAG=-g3 -O0
endif

ifeq ($(WARNING),1)
WARNINGFLAG=-Wall -Wextra
endif

FILE=Memory.o State.o Action.o main.o

$(EXEC): $(FILE) 
	$(CC) $(OPTION) $(FILE) -o $@ $(LINK) $(DEBUGFLAG) $(WARNINGFLAG)

%.o : %.cpp
	$(CC) $(OPTION) -c $< -o $@ $(DEBUGFLAG) $(WARNINGFLAG)

clean:
	rm -f $(FILE) $(EXEC)
