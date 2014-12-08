#ifndef AMYGDALA_HPP
#define AMYGDALA_HPP

#include "Hypothalamus.hpp"
class State;

class Amygdala
{
	Hypothalamus hypothalamus;
	public:
		bool isImportant(State& state);
};

#endif
