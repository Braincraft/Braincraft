#pragma once
class State;

class Hippocampus
{
	void randomizeAction(State& action);
	public:
		void addState(State& state);
		void addState(State& action, State& state);
		void orderState(void);
};
