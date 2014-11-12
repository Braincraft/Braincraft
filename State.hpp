#ifndef STATE_HPP
#define STATE_HPP
#define STEPS 1000
#define TIME_BEFORE_STATE_BECOME_SAFE 20
#define TIME_FOR_DANGEROUS_STATE 2
#include <list>
#include <vector>
#include <set>

class State
{
	std::set < int > blockNearbyField;
	int min(int i1, int i2, int i3);
	State();
  public:
	State(std::vector <int> nf);
	std::list<int> elementInState(void);
	State intersection(State st);
	State difference(State st);
	int levenshteinDistance(State st);
};
#endif
