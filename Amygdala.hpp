#pragma once
#include "Hypothalamus.hpp"
class State;

class Amygdala
{
	Hypothalamus hypothalamus;
	public:
		State* isCritical(State& state);
};
