/**
 * @file Hippocampus.cpp
 * @brief FR : La classe Hippocampus correspond à la mémoire longue durée dans laquelle tout les états successifs par lesquels le bot est passé seront sauvegardé. Les actions effectués à chaque état sont aussi sauvegardé dans cette classe. Par ailleurs, cette classe dispose de méthodes pour chercher des états similaires au moyen d’une fonction de distance.
 * EN :  The Hippocampus class matches the long term memory in which every successive state the bot went through is saved. Actions perform in each state are also saved in this class. This class also owns methods in order to look for similar states using a distance function.
 * @author BraincraftTeam
 */


#include "Hippocampus.hpp"
#include <sstream>
#include <cmath>
#include <ctgmath>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.hpp"
#include "State.hpp"

/**
 * @brief FR : Constructeur / EN : Constructor
 * @param haveToSaveMemory FR : Booléen / EN : boolean
 */
Hippocampus::Hippocampus(bool haveToSaveMemory)
{
	haveToSave = haveToSaveMemory;
	if(haveToSave && ! dirExists(savingLocation))
		haveToSave = false;
	if(haveToSave)
		std::cout << "Saving memory in : " << savingLocation << std::endl;
}

/**
 * @brief FR : Destructeur / EN : Destructor
 */

Hippocampus::~Hippocampus()
{
	alzheimer();
}

/**
 * @brief FR : Efface la mémoire du bot. / EN : Erases the bot's memory
 */
void Hippocampus::alzheimer(void)
{
	for(auto pair : allMemory)
	{
		delete pair.first;
		delete pair.second;
	}
	allMemory.clear();
}

/**
 * @brief FR : Ajoute un état dans la mémoire ainsi qu'une action associé / EN : Adds a state and a matching action in the memory
 * @param action FR : Action associée à l'état à ajouter / EN : Matching action of the state to add
 * @param state FR : Etat à ajouter dans la mémoire / EN : State to add 
 */
void Hippocampus::addState(State& action, State& state)
{
	saveState(action, state);
	++timeCount;
}

/**
 * @brief FR : Ajoute un état dans la mémoire et génère une action associée. / EN : Adds a state in the memory and generates a matching action
 * @param state FR : Etat à ajouter dans la mémoire / EN : State to add 
 */
State* Hippocampus::addState(State& state)
{
	//intern boui boui to create random action	
	State* action = new State();
	if( ! isThereWorryingStuff(*action, state))
		randomizeAction(*action);
	saveState(*action, state);
	++timeCount;
	return action;
}

/**
 * @brief FR : Sauvegarde un état et une action dans la mémoire / EN : Saves a state and an action
 * @param action FR : Action à sauvegarder / EN :   Action to save
 * @param state FR : Etat à sauvegarder / EN : State to save
 */
void Hippocampus::saveState(State& action, State& state)
{
	std::vector<bool> color = colorate(state);
	gaugeVariation.push_front(color);
	allMemory.push_front(std::make_pair(&action, &state));
	if(!haveToSave)
		return;
	std::stringstream streamAction, streamState;
	streamAction << savingLocation << timeCount << "_action.json";
	streamState << savingLocation << timeCount << "_state.json";
	action.save(streamAction.str(), "plain");
	state.save(streamState.str(), "plain");
}

/**
 * @brief FR : Génère une action associé à un état qui dépend des états déjà en mémoire. / EN : Generates an action matching a state depending on already saved states
 * @param action FR : Instance de l'action à générer / EN : Instance of the action to generate
 * @param state FR : Etat à comparer avec ceux de la mémoire / EN : State to compare to those already saved
 * @return FR : Booléen: True si la génération à réussi, False sinon. / EN : True if generation suceeded, else False
 */
bool Hippocampus::isThereWorryingStuff(State& action, State& state)
{
	//Found some coloration
	//Pour le moment une couleur est une variation négative d'une jauge
	std::list<int> dangerousGauge = getDangerousGauge(state);
	std::pair<State*, State*> loutre;
	for(int gauge : dangerousGauge)
	{
		if(findNearColoredState(gauge, loutre, state))
		{
			fprintf(stderr, "Use a previous state (%d)\n", (int)(*loutre.second)[IDX_NOW][IDX_TIME_STEP]);
			action = *loutre.first;
			return true;
		}
	}
	return false;
}

/**
 * @brief FR : Détecte les jauges d'un état qui ont une valeur critique. / EN : Spots critical gauges of a state
 * @param state FR : Etat à vérifier / EN : State analyze
 * @return FR : Liste des jauges critiques / EN : List of critical gauges
 */
std::list<int> Hippocampus::getDangerousGauge(State& state)
{
	std::list<int> ret;
	const double limit = 0.7;
	
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < limit)
		ret.push_front(hungry_color);
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < limit)
		ret.push_front(oxygen_color);
	if((double) state[IDX_SENSATION][IDX_VITAL][IDX_ENERGY] < limit)
		ret.push_front(energy_color);
	return ret;
}

/**
 * @brief FR: Calcul la distance de Levenstein entre deux états considérés comme des chaines de caractères. / EN : This function calculates the Levenshtein's distance between two states considered as Strings.
 * @param s1 FR : Etat à comparer / EN : State to compare
 * @param s2 FR : Etat à comparer / EN : State to compare
 * @return FR : Distance / EN : Distance
*/
double Hippocampus::levenshteinStateDistance(State& s1, State& s2)
{
  std::string str1 = (std::string) s1;
  std::string str2 = (std::string) s2;
  const size_t len1 = str1.size(), len2 = str2.size();
  std::vector<unsigned int> col(len2+1), prevCol(len2+1);
  for (unsigned int i = 0; i < prevCol.size(); i++)
    prevCol[i] = i;
  for (unsigned int i = 0; i < len1; i++) {
    col[0] = i+1;
    for (unsigned int j = 0; j < len2; j++)
      col[j+1] = std::min( std::min(prevCol[1 + j] + 1, col[j] + 1),
				     prevCol[j] + (str1[i]==str2[j] ? 0 : 1) );
    col.swap(prevCol);
  }
  return (double)prevCol[len2];
}

/**
 * @brief FR : Calcul la distance entre deux Etats de manière récursive. / EN : Calculates the distance between two states in a recursive way.
 * @details FR : Cette fonction parcours récursivement les deux états afin de comparer un à un les éléments des deux états. Lorsqu'un élément est manquant, la distance est incrémenté de 5. La distance entre deux booléens ou deux valeurs numériques est égale à leur différence. Pour les chaines de caractère, on utilise la distance de Levenstein. On ignore les champs "Location" des états du bot car il ne sont pas pertinent. /EN : This function recursively runs through both states in other to compare one by one the elements from both states. If there is a missing element, the distance is increased by 5. The distance between 2 booleans or numeric values is equal to their difference. Levenstein's distance is used for strings. The location of the bot is ignore due to it's irrelevance.
 * @param s1 FR : Etat à comparer / EN : State to compare
 * @param s2 FR : Etat à comparer / EN : State to compare
 * @return FR : Distance / EN : Distance
 */
double Hippocampus::distance(State& s1, State& s2) 
{
  State::Iterator it1(s1);
  State::Iterator it2(s2);
  double val=0;
  int count=0;
  int indx=-1;
  bool b1=it1.next(),b2=it2.next();
  
  while(b1 && b2) 
    {
      //ignoring location on the state
      if(it1.getName() == "location") 
	{
	  it1.next();
	  continue;
	}
      if(it2.getName() == "location") 
	{
	  it2.next();
	  continue;
	}
      //save it2
      indx = it2.getIndex();
      //parse the elements
      while( it1.getName() != it2.getName() ) 
	{
	  //count the non corresponding elements
	  count++;
	  if(!it2.next()) 
	    {
	      count = -1;
	      //load it2 to the old value saved in "indx"
	      it2.setIndex(indx);
	      break;
	    }
	}
      if(count < 0) 
	{
	  count = 0;
	  val += 5;
	  b1 = it1.next();
	  continue;
	}
      
      val += count*5; //Add distance for each missing element.
      
      if (it1.getValue().isAtomic() && it2.getValue().isAtomic())
	{
	  /*Add 1 to the distance value if not equal booleans*/
	  if( it1.getValue().isBool() )
	    {
	      if(it1.getValue() != it2.getValue())
		val += 1.0;
	    }
	  else if ( it1.getValue().isDouble() )
	  {
	    val += fabs((double)it1.getValue() - (double)it2.getValue());
	  }
	  else
	    {
	      /* calculate the Levenshtein's distance between two Strings */
	      String S1 = it1.getValue();
	      String S2 = it2.getValue();
	      const size_t len1 = S1.size(), len2 = S2.size();
	      std::vector<unsigned int> col(len2+1), prevCol(len2+1);
	      for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	      for (unsigned int i = 0; i < len1; i++) 
		{
		  col[0] = i+1;
		  for (unsigned int j = 0; j < len2; j++)
		    col[j+1] = std::min( std::min(prevCol[1 + j] + 1, col[j] + 1), prevCol[j] + (S1[i]==S2[j] ? 0 : 1));
		  col.swap(prevCol);
		}
	      val += (double)prevCol[len2];
	    }
	}
      else /* recursive call of this function if at least one of the current elements is not atomic*/
	{
	  val += distance(it1.getValue(),it2.getValue());
	} 
      
      b1 = it1.next();
      b2 = it2.next();
    }
  return val;
}

/**
 * @brief FR : Parcours la memoire pour trouver un état coloré de couleur color et où l'instant suivant, la couleur n'est plus. / EN : Runs through the memory in order to find a stated colored in color and uncolored just after
 * @param color FR : Couleur / EN : Color
 * @param sameColoredState FR : Emplacement du résultat / EN : Location of the resut
 * @param state FR : Etat à comparer / EN : State to compare
 * @return FR : Réussite / EN : Success
 */
bool Hippocampus::findNearColoredState(char color, std::pair<State*, State*>& sameColoredState, State& state)
{
  //Parcours la memoire pour trouver un état coloré de couleur color et où l'instant suivant, la couleur n'est plus
	auto itAllMemory = allMemory.begin();
	auto itGaugeVariation = gaugeVariation.begin();
	std::pair<State*, State*> best;
	double bestDistance;
	bool foundBest = false;
	while(itAllMemory != allMemory.end())
	{
		if(itGaugeVariation->at(color))
		{
			State* current = itAllMemory->second;
			double dist = distance(*current, state);
			//I used < instead of <= because the more recent it is, the best.
			if(foundBest == false || dist < bestDistance)
			{
				best = *itAllMemory;
				bestDistance = dist;
				foundBest = true;
			}
		}
		++itGaugeVariation;
		++itAllMemory;
	}
	if(foundBest)
		sameColoredState = best;
	return foundBest;
}

/**
 * @brief FR : Colore l'état en entrée. / EN : Colors the state given
 * @param state FR : Etat à colorer / EN : State to color
 * @return FR : Triplet indiquant les jauges en baisse: (oxygen_color,hungry_color,energy_color). / EN : 3-tuple indicating the decreasing gauges :(oxygen_color,hungry_color,energy_color).
 */
std::vector<bool> Hippocampus::colorate(State& state)
{
	if(allMemory.size() == 0)
		return std::vector<bool>(3, false);//FIXME Magic number
	State& previous = *(allMemory.front().second);
	return colorate(previous, state);
}

/**
 * @brief FR : Colore l'état en entrée. / EN : Colors the state given
 * @details FR : La couleur est vraie si il y a eu une baisse de la jauge entre l'état previous et l'état state. / EN : The color is true if the gauge decreased between the previous state and the state state
 * @param previous FR : Etat à comparer / EN : State to compare
 * @param state FR : Etat à colorer / EN : State to color
 * @return FR : Triplet indiquant les jauges en baisse: (oxygen_color,hungry_color,energy_color). / EN : 3-tuple indicating the decreasing gauges :(oxygen_color,hungry_color,energy_color).
 */
std::vector<bool> Hippocampus::colorate(State& previous, State& state)
{
	std::vector<bool> ret(3, false);
	ret[oxygen_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_FOOD]);
	ret[hungry_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN]);
	ret[energy_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_ENERGY] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_ENERGY]);
	return ret;
}

/**
 * @brief FR : Génére une action aléatoire. (NON FONCTIONNELLE) / EN : Generates random action (NOT FUNCTIONNAL)
 */
void Hippocampus::randomizeAction(State& action)
{
	double const high = 10;
	double speed = 10;
	double orientation = 5, rotation;
	int randChoice = rand() %100;
	//std::cout << "randomizeAction " << randChoice << std::endl;
	//std::cin >> rotation;
	//std::cin >> orientation;
	//std::cin >> speed;
	//action[IDX_ACTION][IDX_BODY][IDX_ROTATION] = rotation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = orientation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;

	//if(randChoice < 10) {//eat something
		//action[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
	//}
	//else if(randChoice <30) {//Walk forward
		//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;
	//}
	//else if(randChoice <50) {//Change orientation
		//double randOrientation = rand() %360;
		//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = randOrientation;
	//}
	//else if(randChoice <60) {//Jump
		//action[IDX_ACTION][IDX_JUMP] = true;
	//}
	//else {//stop everything
		//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 0;
	//}
}

/**
 * @brief FR : Vérifie si le path existe. / EN : Checks if path exists
 * @param path FR : Chemin à vérifier. / EN : Path to check
 * @return FR : Résultat / EN : Result
 */
bool Hippocampus::dirExists(std::string const& path)
{
	return dirExists(path.c_str());
}


/**
 * @brief FR : Vérifie si le path existe. / EN : Checks if path exists
 * @param path FR : Chemin à vérifier. / EN : Path to check
 * @return FR : Résultat / EN : Result
 */
bool Hippocampus::dirExists(const char *path)
{
	struct stat info;
	if(stat( path, &info ) != 0)
		return false;
	else if(info.st_mode & S_IFDIR)
		return true;
	return false;
}
