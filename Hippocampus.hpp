#pragma once
#include <iostream>
#include <list>
class State;

class Hippocampus
{
	static const char no_color = 0;
	static const char oxygen_color = 1;
	static const char hungry_color = 2;
	static const char energy_color = 3;
	int timeCount = 0;
	bool haveToSave;
	std::list<std::pair<State*, State*>> allMemory;
	std::string savingLocation = "memory/";
	
	
	void randomizeAction(State& action);
	void saveState(State& action, State& state);
	bool dirExists(std::string const& path);
	bool dirExists(const char *path);
	bool isThereWorryingStuff(State& action, State& state);
	char colorate(State& state);
	char colorate(State& previous, State& state);
	bool findNearColoredState(char color, std::pair<State*, State*>& sameColoredState, State& state);
	double distance(State& s1, State& s2);
	public:
		Hippocampus(bool haveToSaveMemory = false);
		State* addState(State& state);
		void addState(State& action, State& state);
		void orderState(void);
		void alzheimer(void);
		~Hippocampus();
};
