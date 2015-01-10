#pragma once
#include "Hypothalamus.hpp"
class State;

class Amygdala
{
	Hypothalamus hypothalamus;
	public:
		State* isImportant(State& state);
};
