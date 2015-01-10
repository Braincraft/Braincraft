#include "Thalamus.hpp"
#include <iostream>

#include "State.hpp"

namespace braincraft
{
Thalamus::Thalamus(Bot& bot): bot(bot)
{
	
}
//Thalamus::Thalamus()
//{
//}
void Thalamus::print(void)
{
	std::cout << "Inside Thalamus" << std::endl;
}
void Thalamus::newState(State& state)
{
	State* action = nullptr;
	if((action = amygdala.isImportant(state)) != nullptr)
	{
		//amygdala.something
		hippocampus.addState(*action, state);
		//bot.outputCurrentState(state);
	}
	else
	{
		hippocampus.addState(state);
	}
}
}
