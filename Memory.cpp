#include <iostream>
#include "Memory.hpp"
#include "State.hpp"
#include "Action.hpp"

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
	std::cout << "New death state" << std::endl;
	State *tmp = nullptr;

	//Put the state in death
	tmp = currentState.front();
	updateTypeState(tmp, IDX_FREQUENCY_ID_DEATH_STATE);
	currentState.pop_front();

	//Then put the following state in dangerous state
	for(int i = 0; i < COUNT_DANGEROUS_STATE_AFTER_DEATH && currentState.size() > 0; i++)
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
			frequencyId[id] = std::vector < int >(COUNT_STATE, 0);
		frequencyId[id][idxFrequency]++;
	}
}

int Memory::coloreState(State & state, float &precision)
{
	std::cout << "coloreState ";
	state.print();
	int idxLowestDistance = 0, lowestDistance = 2147483647, i = 0;
  	for(State * st:genericState)
	{
		if(st != nullptr)
		{
			int currentDistance = st->levenshteinDistance(state);
			if(currentDistance < lowestDistance)
			{
				lowestDistance = currentDistance;
				idxLowestDistance = i;
			}
			std::cout << i << " (" << currentDistance << ") ";
			st->print();
		}
		i++;
	}
	precision = 1;
	return idxLowestDistance;
}

void Memory::updateGenericState(void)
{
	while(currentState.size() > TIME_BEFORE_STATE_BECOME_SAFE)
	{
		State* st = currentState.back();
		updateTypeState(st, IDX_FREQUENCY_ID_SAFE_STATE);
		currentState.pop_back();
	}
 
	for(int i = 0; i < genericState.size(); i++)
	{
		if(genericState[i] != nullptr)
			delete genericState[i];
		genericState[i] = updateGenericState(i);
	}
}

State *Memory::updateGenericState(int idx)
{
	std::vector<int> element;
	std::cout << idx << " == " << allIdentifiedState[idx].size() << " == ";
	for(auto line : frequencyId)
	{
		float percent = 0; 
		if(allIdentifiedState[idx].size() > 0)
			percent = (float)line.second[idx] / (float)allIdentifiedState[idx].size();
		std::cout << "(" << line.first << " -> " << percent << ", " << PERCENT_LIMIT_FOR_GENERIC << ") ";
		if(percent >= PERCENT_LIMIT_FOR_GENERIC)
			element.push_back(line.first);
	}
	std::cout << std::endl;
	if(element.size() > 0)
		return new State(element);
	return nullptr;
}

bool Memory::getRidOfId(std::vector<int> frequencyRow)
{
	auto itFrequency = frequencyRow.begin();
	auto itState = allIdentifiedState.begin();
	std::vector<float> average(frequencyRow.size());
	auto itAverage = average.begin();
	for(; itFrequency != frequencyRow.end(); itFrequency++, itState++)
		*itAverage = *itFrequency / itState->size();

	float av = 0;
	for(itAverage = average.begin(); itAverage != average.end(); itAverage++)
		av += *itAverage;
	av = av / average.size();
	for(itAverage = average.begin(); itAverage != average.end(); itAverage++)
		if(*itAverage - av > PERCENT_LIMIT_FOR_GENERIC)
			return false;
	return true; 
}

void Memory::print(void)
{
	for(auto p : frequencyId)
	{
		std::cout << p.first << " -> ";
		for(auto freq : p.second)
			std::cout << freq << " | ";
		std::cout << std::endl;
	}
	
	for(State* st : genericState)
		if(st != nullptr)
			st->print();
}

Memory::~Memory()
{
  	for(std::pair < State *, Action * >p:actionOnState)
		delete p.second;
	for(auto coloredList:allIdentifiedState)
	  	for(auto state:coloredList)
			delete state;
  	for(auto state:currentState)
		delete state;
}
