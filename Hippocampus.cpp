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

Hippocampus::Hippocampus(bool haveToSaveMemory)
{
	haveToSave = haveToSaveMemory;
	if(haveToSave && ! dirExists(savingLocation))
		haveToSave = false;
	if(haveToSave)
		std::cout << "Saving memory in : " << savingLocation << std::endl;
}
Hippocampus::~Hippocampus()
{
	alzheimer();
}
void Hippocampus::alzheimer(void)
{
	for(auto pair : allMemory)
	{
		delete pair.first;
		delete pair.second;
	}
	allMemory.clear();
}
void Hippocampus::addState(State& action, State& state)
{
	saveState(action, state);
	++timeCount;
}
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
double Hippocampus::distance(State& s1, State& s2)
{
  State::Iterator it1(s1);
  State::Iterator it2(s2);
  double val=0;
  bool b1=it1.next(),b2=it2.next();
  while(b1 && b2){
    //std::cout << "calculating distance"<< std::endl;
    if (it1.getValue().isAtomic() && it2.getValue().isAtomic())//might be a problem if only one is atomic
      {
	if( it1.getValue().isBool() )
	  {
	    //std::cout << "calculating bool"<< std::endl;
	    if(it1.getValue() != it2.getValue())
	      val += 1.0;
	  }
	else if ( it1.getValue().isDouble() )
	  {
	    val += fabs((double)it1.getValue() - (double)it2.getValue());
	    //std::cout << "calculating double "<< std::endl;
	  }
	else
	  {
	    //std::cout << "calculating string"<< std::endl;
	    String S1 = it1.getValue();
	    String S2 = it2.getValue();
	    //unsigned int levenshtein_distance(const T &S1, const T & S2) {
	    const size_t len1 = S1.size(), len2 = S2.size();
	    std::vector<unsigned int> col(len2+1), prevCol(len2+1);
	    
	    for (unsigned int i = 0; i < prevCol.size(); i++)
	      prevCol[i] = i;
	    for (unsigned int i = 0; i < len1; i++) {
	      col[0] = i+1;
	      for (unsigned int j = 0; j < len2; j++)
		col[j+1] = std::min( std::min(prevCol[1 + j] + 1, col[j] + 1),
				     prevCol[j] + (S1[i]==S2[j] ? 0 : 1) );
	      col.swap(prevCol);
	    }
	    val += (double)prevCol[len2];
	    //return prevCol[len2];
	  }
	   
      }
    else
      {
    	val += distance(it1.getValue(),it2.getValue());
      }
    b1 = it1.next();
    b2 = it2.next();
  }
  std::cout << "distance of " << val << " calculated"<<std::endl;
  return val;
}
bool Hippocampus::findNearColoredState(char color, std::pair<State*, State*>& sameColoredState, State& state)
{
	//Parcour la memory pour trouver un instant coloré de color et où l'instant suivant, la couleur n'est plus
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
std::vector<bool> Hippocampus::colorate(State& state)
{
	if(allMemory.size() == 0)
		return std::vector<bool>(3, false);//FIXME Magic number
	State& previous = *(allMemory.front().second);
	return colorate(previous, state);
}
std::vector<bool> Hippocampus::colorate(State& previous, State& state)
{
	std::vector<bool> ret(3, false);
	ret[oxygen_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_FOOD]);
	ret[hungry_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN]);
	ret[energy_color] = ((double) state[IDX_SENSATION][IDX_VITAL][IDX_ENERGY] > (double) previous[IDX_SENSATION][IDX_VITAL][IDX_ENERGY]);
	return ret;
}
void Hippocampus::randomizeAction(State& action)
{
	double const high = 10;
	double speed = 10;
	double orientation = 5, rotation;
	int randChoice = rand() %100;
	std::cout << "randomizeAction " << randChoice << std::endl;
	//std::cin >> rotation;
	//std::cin >> orientation;
	//std::cin >> speed;
	//action[IDX_ACTION][IDX_BODY][IDX_ROTATION] = rotation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = orientation;
	//action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;

	if(randChoice < 10) {//eat something
		action[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
	}
	else if(randChoice <30) {//Walk forward
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = speed;
	}
	else if(randChoice <50) {//Change orientation
		double randOrientation = rand() %360;
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = randOrientation;
	}
	else if(randChoice <60) {//Jump
		action[IDX_ACTION][IDX_JUMP] = true;
	}
	else {//stop everything
		action[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 0;
	}
}
bool Hippocampus::dirExists(std::string const& path)
{
	return dirExists(path.c_str());
}
bool Hippocampus::dirExists(const char *path)
{
	struct stat info;
	if(stat( path, &info ) != 0)
		return false;
	else if(info.st_mode & S_IFDIR)
		return true;
	return false;
}
