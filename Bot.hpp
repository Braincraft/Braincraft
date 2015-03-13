#pragma once
#define STEPS 1000

#include "BotAPI.hpp"
#include "State.hpp"

using botplug::BotAPI;
//class Action_braincraft;
namespace braincraft
{
	class Thalamus;
}
class Bot : public BotAPI
{
	private:
		braincraft::Thalamus* thalamus;
		/**
		 * @brief FR : Fonction d'initialisation du bot. / EN : bot's initialization function
		 */
		void initBot();
		/**
		 * @brief FR : Condition d'arrêt du bot. / EN : bot's stop condition
		 * @return FR : vrai si le bot doit s'arrêter, faux sinon. / EN : true if the bot must stop, else false
		 */
		bool stopCondition() const;
		// for pixel view
		// Defines the internal bot-sampling-time incremented at each step
		unsigned int botSamplingTime = 0, botMaxSamplingTime = 1000;
		void inputCurrentState(State& currentState);
                static bool sortBlocks(botplug::Block b1 , botplug::Block b2);
                static bool sortItems(botplug::item i1 , botplug::item i2);
                static bool sortEntities(botplug::Entity i1 , botplug::Entity i2);
		void inputCurrentState(State& currentSubState, std::vector<botplug::Block>& blocks);
		void inputCurrentState(State& currentSubState, std::vector<botplug::item>& items);
		void inputCurrentState(State& currentSubState, std::vector<botplug::Entity>& entities);
		void outputCurrentState(State& currentState);
		static void dump(const std::vector<botplug::Block>& blocks);
		static void dump(const botplug::Block& block);
		static void dump(const std::vector<botplug::item>& items, const std::vector<bool>& slots);
		// These are initialization/stopping mechanism.

	protected:
		/**
		 * @brief FR : Contient l'algorithme d'apprentissage/le cerveau du bot. / EN : Contain the learning algorithym/ the bot's brain
		 * @return
		 */
		int brainDo();

	public:
		Bot();
		Bot(const std::string file);
		// Botlogin
		Bot(const std::string addr, const unsigned int port, const std::string login);
		virtual ~Bot();
		double frand_a_b(double a, double b);
		void doAction(State & action);
		void dump();
		int nTour = 0;
};

