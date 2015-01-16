#include "Hippocampus.hpp"
#include "config.hpp"
#include "State.hpp"

void Hippocampus::addState(State& action, State& state)
{
}
State* Hippocampus::addState(State& state)
{
	//intern boui boui to create random action	
	State* randAction = new State();
	randomizeAction(*randAction);
	return randAction;
}
void Hippocampus::orderState(void)
{
}
void Hippocampus::randomizeAction(State& action)
{
	double const high = 10;
	double const speed = 10;
	int randChoice = rand() %100;


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
