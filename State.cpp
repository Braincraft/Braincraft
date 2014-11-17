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
	print();
	st.print();
	std::vector < std::vector < int >>d(blockNearbyField.size() + 1, std::vector < int >(st.blockNearbyField.size() + 1, 0));
	unsigned int i, j, cost;
	//Start at 1 because the first is already at 0
	for(i = 1; i <= blockNearbyField.size(); i++)
		d[i][0] = i;
	//Start at 1 because the first is already at 0
	for(j = 1; j <= st.blockNearbyField.size(); j++)
		d[0][j] = j;
	auto it = blockNearbyField.begin();
	int tab[3];
	//for(i = 0; i < d.size();i++)
	//{
		//for(j = 0; j < d[i].size(); j++)
			//std::cout << d[i][j] << " ";
		//std::cout << std::endl;
	//}
	//std::cout << std::endl;
	for(i = 1; i <= blockNearbyField.size(); i++, it++)
	{
		auto jt = st.blockNearbyField.begin();
		for(j = 1; j <= st.blockNearbyField.size(); j++, jt++)
		{
			if(*it == *jt)
				cost = 0;
			else
				cost = 1;
			tab[0] = d[i-1][j] + 1;
			tab[1] = d[i][j-1] + 1;
			tab[2] = d[i-1][j-1] + cost;
			d[i][j] = min(tab, 3);
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

int State::min(int const * tab, const int size)
{
	int smallest = tab[0];
	for(int i = 1; i < size; i++)
		if(tab[i] < smallest)
			smallest = tab[i];
	return smallest;
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
