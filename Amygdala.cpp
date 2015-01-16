#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

State* Amygdala::isCritical(State& state)
{
	std::cout << "Amygdala::isCritical" << std::endl;
	if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < 1.0)
	{
		std::cout << " oxygen" << std::endl;
		State& reflexAction = *(new State());
		reflexAction[IDX_ACTION][IDX_JUMP] = true;
		return &reflexAction;
	}

	if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < 0.5)
	{
		std::cout << "hungry" << std::endl;
		State& reflexAction = *(new State());
		//Mettre nourriture dans sa main (si jamais il n'y en a pas déjà dedans)
		reflexAction[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
		return &reflexAction;
	}
	return nullptr;
}
