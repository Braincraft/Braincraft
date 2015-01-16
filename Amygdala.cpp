#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

State* Amygdala::isImportant(State& state)
{
	std::cout << "Amygdala::isImportant" << std::endl;
	if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < 1.0)
	{
		std::cout << " oxygen" << std::endl;
		State& reflexAction = *(new State());
		reflexAction[IDX_ACTION][IDX_JUMP] = true;
		return &reflexAction;
	}
	return nullptr;
}
