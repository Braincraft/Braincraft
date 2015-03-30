/**
 * @file Amygdala.hpp
 * @brief FR : La classe Amygdala est destinée à repérer des éléments importants de l’environnement Minecraft et d’exécuter une action associée si c’est possible.
 * EN :  The Amygdala class aims to extract important elements from the Minecraft environment and execute a matching action if possible.
 * @author BraincraftTeam
 */

#pragma once
#include <set>
#include "Hypothalamus.hpp"
class State;


/**
 * @brief FR : La classe Amygdala est destinée à repérer des éléments importants de l’environnement Minecraft et d’exécuter une action associée si c’est possible.
 * EN :  The Amygdala class aims to extract important elements from the Minecraft environment and execute a matching action if possible.
 */
class Amygdala
{
	std::set < int >dangerousBlock;
	std::set < int >isFood;
	std::set < int >dangerousEntity;
	std::set < int >interrestingEntity;

	//const double hungry = 0.8;
	// const double drowning = 1;


	Hypothalamus hypothalamus;
	bool isThereDangerousStuff(State & state, double &x, double &y, double &z);
	bool isThereInterrestingStuff(State & state, double &x, double &y, double &z);
	bool isThereDangerousStuff(State & state, double &x, double &y, double &z, std::set < int >const &list) const;
	bool tryToEat(State & state, State* &reflex);
	State & runAway(State & state, double x_foe, double y_foe, double z_foe);
	State & aim(State & state, double x_foe, double y_foe, double z_foe);
	
	public:
	Amygdala();
	State *isCritical(State & state);
};
