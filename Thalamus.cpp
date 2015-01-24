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
	{
		//amygdala.something
		//bot.
		hippocampus.addState(*action, state);
		bot.doAction(*action);
	}
	else
	{
		action = hippocampus.addState(state);
		bot.doAction(*action);
	}
}
}
