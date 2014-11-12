#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <list>
#include <map>
#include <vector>

#define IDX_FREQUENCY_ID_DEATH_STATE 0
#define IDX_FREQUENCY_ID_DANGEROUS_STATE 1
#define IDX_FREQUENCY_ID_SAFE_STATE 2
#define IDX_FREQUENCY_ID_GREETING_STATE 3

#define COUNT_DANGEROUS_STATE_AFTER_DEATH 1

class State;
class Action;

class Memory
{
	std::vector<std::list<State*> > allIdentifiedState = std::vector<std::list<State*> >(4);

	std::list<State*> currentState;	


	std::map<State*, Action*> actionOnState; //Tous les States ici sont dans l'une des listes plus haut

	std::map<int, std::vector<int> > frequencyId; //Le vector doit avoir 4 éléments

	public:
		void addState(Action*, State*);
		void addState(State*);
		void modifyRecentStateInDeath(void);
		

	private:
		void updateTypeState(State* state, int type);
		void updateFrequency(State* state, int idxFrequency);
};

#endif
