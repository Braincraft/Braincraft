#ifndef HIPPOCAMPUS_HPP
#define HIPPOCAMPUS_HPP

class State;
class Action;

class Hippocampus
{
	public:
	void addState(State& state);
	void addState(Action& action, State& state);
	void orderState(void);

};
#endif
