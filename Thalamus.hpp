#ifndef THALAMUS_HPP
#define THALAMUS_HPP

#include "Hippocampus.hpp"
#include "Amygdala.hpp"

class State;
class Thalamus
{
	Hippocampus hippocampus;
	Amygdala amygdala;
	public:
		void print(void);
		void newState(State& state);
};
#endif
