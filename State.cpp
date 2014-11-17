#include "State.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

State::State(std::vector < int >nearbyField)
{
	for(auto it = nearbyField.begin(); it != nearbyField.end(); it++)
	{
		if(blockNearbyField.find(*it) == blockNearbyField.end())
			blockNearbyField.insert(*it);
	}
	//for(auto it = blockNearbyField.begin(); it != blockNearbyField.end(); it++)
		//std::cout << *it << " ";
	//std::cout << std::endl;
}

State::State()
{
}

State State::intersection(State st)
{
	State ret;
	std::set_intersection(blockNearbyField.begin(), blockNearbyField.end(), st.blockNearbyField.begin(), st.blockNearbyField.end(), std::inserter(ret.blockNearbyField, ret.blockNearbyField.begin()));
	return ret;
}

State State::difference(State st)
{
	State ret;
	std::set_difference(blockNearbyField.begin(), blockNearbyField.end(), st.blockNearbyField.begin(), st.blockNearbyField.end(), std::inserter(ret.blockNearbyField, ret.blockNearbyField.begin()));
	return ret;
}

int State::levenshteinDistance(State& st)
{
	std::vector < std::vector < int >>d(blockNearbyField.size() + 1, std::vector < int >(st.blockNearbyField.size() + 1, 0));
	unsigned int i, j, cost;
	for(i = 0; i < blockNearbyField.size(); i++)
		d[i][0] = i;
	for(j = 0; j < st.blockNearbyField.size(); j++)
		d[0][j] = j;
	auto it = blockNearbyField.begin();
	auto jt = st.blockNearbyField.begin();
	//for(i = 0; i < d.size();i++)
	//{
		//for(j = 0; j < d[i].size(); j++)
			//std::cout << d[i][j] << " ";
		//std::cout << std::endl;
	//}
	//std::cout << std::endl;
	for(i = 1; i < blockNearbyField.size()+1; i++, it++)
	{
		for(j = 1; j < st.blockNearbyField.size()+1; j++, jt++)
		{
			if(*it == *jt)
				cost = 0;
			else
				cost = 1;
			d[i][j] = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost);
		}
	}
	//for(i = 0; i < d.size();i++)
	//{
		//for(j = 0; j < d[i].size(); j++)
			//std::cout << d[i][j] << " ";
		//std::cout << std::endl;
	//}
	return d[blockNearbyField.size()][st.blockNearbyField.size()];
}

int State::min(int i1, int i2, int i3)
{
	if(i1 <= i2 && i1 <= i3)
		return i1;
	if(i2 <= i1 && i2 <= i3)
		return i2;
	return i3;
}


std::list <int> State::elementInState(void)
{
	return std::list<int>(blockNearbyField.begin(), blockNearbyField.end());
}

void State::print(void)
{
	std::cout << "[";
	for(int id : blockNearbyField)
		std::cout << " " << id << ",";
	std::cout << "]" << std::endl;
}
