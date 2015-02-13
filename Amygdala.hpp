#pragma once
#include <set>
#include "Hypothalamus.hpp"
class State;

class Amygdala
{
	std::set < int >dangerousBlock;
	std::set < int >isFood;
	std::set < int >dangerousEntity;

	//const double hungry = 0.8;
	// const double drowning = 1;


	Hypothalamus hypothalamus;
	bool isThereDangerousStuff(State & state, double &x, double &y, double &z);
	bool isThereDangerousStuff(State & state, double &x, double &y, double &z, std::set < int >const &list) const;
	bool tryToEat(State & state, State* &reflex);
	 State & runAway(State & state, double x_foe, double y_foe, double z_foe);
	public:
	Amygdala();
	State *isCritical(State & state);
};
