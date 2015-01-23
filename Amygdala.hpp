#pragma once
#include <set>
#include "Hypothalamus.hpp"
class State;

class Amygdala
{
  std::set<int> dangerousBlock;
  std::set<int> isFood;
  std::set<int> dangerousEntity;

  
  Hypothalamus hypothalamus;
  bool isThereDangerousStuff(State& state, double &x, double &y, double &z);
  bool isThereDangerousStuff(State& state, double &x, double &y, double &z, std::set<int> const& list) const;
  State& runAway(State& state, double x_foe, double y_foe, double z_foe);
public:
  Amygdala();
  State* isCritical(State& state);
};
