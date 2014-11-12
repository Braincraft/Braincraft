#include "Memory.hpp"
#include "State.hpp"

void Memory::addState(Action * a, State * s)
{
	if(actionOnState.count(s) == 0)
	{
		addState(s);
		actionOnState[s] = a;
	}
}

void Memory::addState(State * s)
{
	currentState.push_front(s);
}

void Memory::modifyRecentStateInDeath(void)
{
	State *tmp = nullptr;

	//Put the state in death
	tmp = currentState.front();
	updateTypeState(tmp, IDX_FREQUENCY_ID_DEATH_STATE);
	currentState.pop_front();

	//Then put the following state in dangerous state
	for(int i = 0; i < COUNT_DANGEROUS_STATE_AFTER_DEATH; i++)
	{
		tmp = currentState.front();
		updateTypeState(tmp, IDX_FREQUENCY_ID_DANGEROUS_STATE);
		currentState.pop_front();
	}
}

void Memory::updateTypeState(State * state, int type)
{
	allIdentifiedState[type].push_front(state);
	updateFrequency(state, type);
}

void Memory::updateFrequency(State * state, int idxFrequency)
{
	for(int id : state->elementInState())
	{
		//Add data in frequencyId if they doesn't exist
		if(frequencyId.count(id) == 0)
			frequencyId[id] = std::vector<int>(4, 0);
		frequencyId[id][idxFrequency]++;
	}
}
