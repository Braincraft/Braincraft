#pragma once
#include "Hippocampus.hpp"
#include "Amygdala.hpp"
#include "Bot.hpp"

class State;
namespace braincraft
{
class Thalamus
{
	Hippocampus hippocampus;
	Amygdala* amygdala;
	Bot& bot;
	public:
		Thalamus(Bot& bot);
		void print(void);
		void newState(State& state);
};
}
