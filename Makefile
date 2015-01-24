CC=g++
OPTION=-g -std=c++11
FILE=obj/Bot.o obj/Amygdala.o obj/Hippocampus.o obj/Hypothalamus.o obj/Thalamus.o mock/BotAPI.o

mockbrain: obj $(FILE) 
	$(CC) $(OPTION) $(FILE) -o mock/$@ 

obj/%.o : %.cpp
	$(CC) $(OPTION) -I mock/ -c $< -o $@ 
obj:
	mkdir obj
clean:
	rm -f $(FILE)
ve:
	@(cd .. && make -f Makefile)
