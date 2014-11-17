#ifndef STATE_HPP
#define STATE_HPP
#include <list>
#include <vector>
#include <set>

class State
{
	std::set < int > blockNearbyField;
	int min(int const * tab, const int size);
	State();
  public:
	State(std::vector <int> nf);
	std::list<int> elementInState(void);
	State intersection(State st);
	State difference(State st);
	int levenshteinDistance(State& st);
	void print(void);
};
#endif
