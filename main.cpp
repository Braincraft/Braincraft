#include <iostream>
#include <regex>
#include <iterator>

#include "Memory.hpp"
#include "State.hpp"

#define TYPE_DEATH '+'

Memory* createMemory();
bool readStateLine(FILE* file, std::vector<int> & listId, char &type);

int main()
{
	std::cout << "Hello world" << std::endl;
	std::vector<int> unColored;
	std::vector<int> unColored1;
	unColored.push_back(3);
	unColored.push_back(5);
	unColored.push_back(8);
	unColored1.push_back(3);
	unColored1.push_back(5);
	unColored1.push_back(8);
	State unColoredState(unColored);
	//State unColoredState1(unColored1);
	float prec;
	//std::cout << unColoredState.levenshteinDistance(unColoredState1) << std::endl;
	Memory* memory = createMemory();
	memory->updateGenericState();
	memory->print();
	std::cout << "Color : " << memory->coloreState(unColoredState, prec) << std::endl;
	return 0;
}

Memory* createMemory()
{
	std::vector<int> vec;
	FILE* f = fopen("canard.state", "r");
	char type;
	Memory* memory = new Memory();
	//FIXME if a line end with EOF, it's throw away
	while(readStateLine(f, vec, type))
	{
		State *s = new State(vec);
		memory->addState(s);
		if(type == TYPE_DEATH)
			memory->modifyRecentStateInDeath();
		vec.clear();
	}
	return memory;
}

bool readStateLine(FILE* file, std::vector<int> & listId, char &type)
{
	int id;

	type = getc(file);

	while(fscanf(file, "%d,", &id) == 1)
	{
		//std::cout << id << " ";
		listId.push_back(id);	
	}
	//std::cout << std::endl << "Padd -> ";

	while((id = getc(file)) != '\n' && id != EOF){
	//printf("[%c] ", id);
	}
	//std::cout << std::endl;
	if(id == EOF)
		return false;
	return true;
}
