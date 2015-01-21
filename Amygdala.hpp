#pragma once
#include "Hypothalamus.hpp"
class State;

class Amygdala
{
	Hypothalamus hypothalamus;
	bool isThereDangerousBlocks(State& state, double &x, double &y, double &z);
	State& runAway(State& state, double x_foe, double y_foe, double z_foe);
	public:
		State* isCritical(State& state);
};
