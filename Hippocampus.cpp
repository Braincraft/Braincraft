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
	State* action = new State();
	if( ! isThereWorryingStuff(*action, state))
		randomizeAction(*action);
	saveState(*action, state);
	++timeCount;
	return action;
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
bool Hippocampus::isThereWorryingStuff(State& action, State& state)
{
	//Found some coloration
	//Pour le moment une couleur est une variation négative d'une jauge
	int color = colorate(state);
	if(color == no_color)
		return false;
	std::pair<State*, State*> loutre;
	if(findNearColoredState(color, loutre, state))
	{
		fprintf(stderr, "Use a previous state (%d)\n", (int)(*loutre.second)[IDX_NOW][IDX_TIME_STEP]);
		action = *loutre.first;
		return true;
	}
	
	return false;
}
double Hippocampus::distance(State& s1, State& s2)
{
	return 1.0;
}
bool Hippocampus::findNearColoredState(char color, std::pair<State*, State*>& sameColoredState, State& state)
{
	//Parcour la memory pour trouver un instant coloré de color et où l'instant suivant, la couleur n'est plus
	if(allMemory.size() == 1)
		return false;
	auto firstTime = allMemory.begin();
	auto secondTime = allMemory.begin();
	auto thirdTime = allMemory.begin();
	++secondTime;
	++thirdTime;
	++thirdTime;
	std::pair<State*, State*> best;
	double bestDistance;
	bool foundBest = false;
	while(secondTime != allMemory.end())
	{
		State* firstState = firstTime->second;
		State* secondState = secondTime->second;
		State* thirdState = thirdTime->second;
		char firstColor = colorate(*secondState, *firstState);
		char secondColor = colorate(*thirdState, *secondState);
		if(secondColor == color && secondColor != firstColor)
		{
			double dist = distance(*firstState, state);
			//I used < instead of <= because the more recent it is, the best.
			if(foundBest == false || dist < bestDistance)
			{
				best = * firstTime;
				bestDistance = dist;
				foundBest = true;
			}
		}	
		++secondTime;
		++firstTime;
		++thirdTime;
	}
	if(foundBest)
		sameColoredState = best;
	return foundBest;
}
char Hippocampus::colorate(State& state)
{
	State& previous = *(allMemory.front().second);
	return colorate(previous, state);
}
char Hippocampus::colorate(State& previous, State& state)
{
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < (double) previous[IDX_SENSATION][IDX_VITAL][IDX_FOOD])
		return hungry_color;
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < (double) previous[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN])
		return oxygen_color;
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_ENERGY] < (double) previous[IDX_SENSATION][IDX_VITAL][IDX_ENERGY])
		return hungry_color;
	return no_color;
}
void Hippocampus::randomizeAction(State& action)
{
	double const high = 10;
	double speed = 10;
	double orientation = 5, rotation;
	int randChoice = rand() %100;
	std::cout << "randomizeAction " << randChoice << std::endl;
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
