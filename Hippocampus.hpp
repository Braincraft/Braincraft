#pragma once
#include <iostream>
class State;

class Hippocampus
{
	int timeCount = 0;
	bool haveToSave;
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
};
