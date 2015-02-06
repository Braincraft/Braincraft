#pragma once
#include <iostream>
#include <list>
#include <vector>
class State;

class Hippocampus
{
	static const char oxygen_color = 0;
	static const char hungry_color = 1;
	static const char energy_color = 2;
	int timeCount = 0;
	bool haveToSave;
	std::list<std::pair<State*, State*>> allMemory;
	std::list<std::vector<bool>> gaugeVariation;
	std::string savingLocation = "memory/";
	
	
	void randomizeAction(State& action);
	void saveState(State& action, State& state);
	bool dirExists(std::string const& path);
	bool dirExists(const char *path);
	bool isThereWorryingStuff(State& action, State& state);
	std::vector<bool> colorate(State& state);
	std::vector<bool> colorate(State& previous, State& state);
	bool findNearColoredState(char color, std::pair<State*, State*>& sameColoredState, State& state);
	
	std::list<int> getDangerousGauge(State& state);
	public:
		Hippocampus(bool haveToSaveMemory = false);
		State* addState(State& state);
		void addState(State& action, State& state);
		void alzheimer(void);
                double distance(State& s1, State& s2);
		~Hippocampus();
};
