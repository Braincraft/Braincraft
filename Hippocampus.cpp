#include "Hippocampus.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.hpp"
#include "State.hpp"

Hippocampus::Hippocampus(bool haveToSaveMemory)
{
	haveToSave = haveToSaveMemory;
	if(haveToSave && ! dirExists(savingLocation))
		haveToSave = false;
	if(haveToSave)
		std::cout << "Saving memory in : " << savingLocation << std::endl;
}
Hippocampus::~Hippocampus()
{
	alzheimer();
}
void Hippocampus::alzheimer(void)
{
	for(auto pair : allMemory)
	{
		delete pair.first;
		delete pair.second;
	}
	allMemory.clear();
}
void Hippocampus::addState(State& action, State& state)
{
	saveState(action, state);
	++timeCount;
}
State* Hippocampus::addState(State& state)
{
	//intern boui boui to create random action	
	State* randAction = new State();
	randomizeAction(*randAction);
	saveState(*randAction, state);
	++timeCount;
	return randAction;
}
void Hippocampus::saveState(State& action, State& state)
{
	allMemory.push_front(std::make_pair(&action, &state));
	if(!haveToSave)
		return;
	std::stringstream streamAction, streamState;
	streamAction << savingLocation << timeCount << "_action.json";
	streamState << savingLocation << timeCount << "_state.json";
	action.save(streamAction.str(), "plain");
	state.save(streamState.str(), "plain");
}
void Hippocampus::orderState(void)
{
}
void Hippocampus::randomizeAction(State& action)
{
	double const high = 10;
	double speed = 10;
	double orientation = 5, rotation;
	int randChoice = rand() %100;
	//std::cout << "randomizeAction " << randChoice << std::endl;
	//std::cin >> rotation;
	//std::cin >> orientation;
	//std::cin >> speed;
	//action[IDX_ACTION][IDX_BODY][IDX_ROTATION] = rotation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = orientation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;

	if(randChoice < 10) {//eat something
		action[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
	}
	else if(randChoice <30) {//Walk forward
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;
	}
	else if(randChoice <50) {//Change orientation
		double randOrientation = rand() %360;
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = randOrientation;
	}
	else if(randChoice <60) {//Jump
		action[IDX_ACTION][IDX_JUMP] = true;
	}

	else {//stop everything
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 0;
	}
}
bool Hippocampus::dirExists(std::string const& path)
{
	return dirExists(path.c_str());
}
bool Hippocampus::dirExists(const char *path)
{
	struct stat info;
	if(stat( path, &info ) != 0)
		return false;
	else if(info.st_mode & S_IFDIR)
		return true;
	return false;
}
