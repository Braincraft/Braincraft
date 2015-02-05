#include "Thalamus.hpp"
#include <iostream>

#include "State.hpp"

namespace braincraft
{
Thalamus::Thalamus(Bot& bot): bot(bot), hippocampus()
{
	std::cout << "Create Thalamus" << std::endl;
	amygdala = new Amygdala();
	
}
void Thalamus::print(void)
{
	std::cout << "Inside Thalamus" << std::endl;
}
void Thalamus::newState(State& state)
{
	State* action = nullptr;
	if((action = amygdala->isCritical(state)) != nullptr)
		hippocampus.addState(*action, state);
	else
		action = hippocampus.addState(state);
	bot.doAction(*action);
}
}
