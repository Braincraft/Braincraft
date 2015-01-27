#pragma once
#include <iostream>
#include <list>
class State;

class Hippocampus
{
	int timeCount = 0;
	bool haveToSave;
	std::list<std::pair<State*, State*>> allMemory;
	std::string savingLocation = "memory/";
	
	
	void randomizeAction(State& action);
	void saveState(State& action, State& state);
	bool dirExists(std::string const& path);
	bool dirExists(const char *path);
	public:
		Hippocampus(bool haveToSaveMemory = false);
		State* addState(State& state);
		void addState(State& action, State& state);
		void orderState(void);
		void alzheimer(void);
		~Hippocampus();
};
