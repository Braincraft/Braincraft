CC=g++
OPTION=-std=c++11
FILE=Bot.o Amygdala.o Hippocampus.o Hypothalamus.o Thalamus.o mock/BotAPI.o

brain: $(FILE)
	$(CC) $(OPTION) $(FILE) -o $@ 

%.o : %.cpp
	$(CC) $(OPTION) -I mock/ -c $< -o $@ 

clean:
	rm -f $(FILE)
ve:
	@(cd .. && make -f Makefile)
