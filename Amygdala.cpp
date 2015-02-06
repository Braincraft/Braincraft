#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

#define HUNGRY 0.8
#define DROWNING 1

Amygdala::Amygdala() : dangerousBlock(), isFood(), dangerousEntity()
{
	std::cerr << "Create Amygdala" << std::endl;
	dangerousBlock.insert(11); //ID of lava
	dangerousBlock.insert(10); //ID of spreading lava
	dangerousBlock.insert(51); //ID of fire
	dangerousBlock.insert(52); //ID of  mob spawner
	dangerousBlock.insert(81); //ID of cactus

	dangerousEntity.insert(50); // ID of Creeper
	dangerousEntity.insert(51); // ID of Skeleton
	dangerousEntity.insert(52); // ID of Spider
	dangerousEntity.insert(53); // ID of Giant
	dangerousEntity.insert(54); // ID of Zombie
	dangerousEntity.insert(55); // ID of Slime

	dangerousEntity.insert(0); // ID of Creeper
	dangerousEntity.insert(1); // ID of Skeleton
	dangerousEntity.insert(2); // ID of Spider
	dangerousEntity.insert(3); // ID of Giant
	dangerousEntity.insert(4); // ID of Zombie
	dangerousEntity.insert(5); // ID of Slime	
	dangerousEntity.insert(6); // ID of Creeper
	dangerousEntity.insert(7); // ID of Skeleton
	dangerousEntity.insert(8); // ID of Spider
	dangerousEntity.insert(9); // ID of Giant
	dangerousEntity.insert(10); // ID of Zombie
	dangerousEntity.insert(11); // ID of Slime
	dangerousEntity.insert(12); // ID of Creeper
	dangerousEntity.insert(13); // ID of Skeleton
	dangerousEntity.insert(14); // ID of Spider
	dangerousEntity.insert(15); // ID of Giant
	dangerousEntity.insert(16); // ID of Zombie
	dangerousEntity.insert(17); // ID of Slime
	dangerousEntity.insert(18); // ID of Creeper
	dangerousEntity.insert(19); // ID of Skeleton
	dangerousEntity.insert(20); // ID of Spider
	dangerousEntity.insert(21); // ID of Giant
	dangerousEntity.insert(22); // ID of Zombie
	dangerousEntity.insert(23); // ID of Slime
	dangerousEntity.insert(24); // ID of Creeper
	dangerousEntity.insert(25); // ID of Skeleton
	dangerousEntity.insert(26); // ID of Spider
	dangerousEntity.insert(27); // ID of Giant
	dangerousEntity.insert(28); // ID of Zombie
	dangerousEntity.insert(29); // ID of Slime
	dangerousEntity.insert(30); // ID of Creeper
	dangerousEntity.insert(31); // ID of Skeleton
	dangerousEntity.insert(32); // ID of Spider
	dangerousEntity.insert(33); // ID of Giant
	dangerousEntity.insert(34); // ID of Zombie
	dangerousEntity.insert(35); // ID of Slime
	dangerousEntity.insert(36); // ID of Creeper
	dangerousEntity.insert(37); // ID of Skeleton
	dangerousEntity.insert(38); // ID of Spider
	dangerousEntity.insert(39); // ID of Giant
	dangerousEntity.insert(40); // ID of Zombie
	dangerousEntity.insert(41); // ID of Slime
	dangerousEntity.insert(42); // ID of Creeper
	dangerousEntity.insert(43); // ID of Skeleton
	dangerousEntity.insert(44); // ID of Spider
	dangerousEntity.insert(45); // ID of Giant
	dangerousEntity.insert(46); // ID of Zombie
	dangerousEntity.insert(47); // ID of Slime
	dangerousEntity.insert(48); // ID of Creeper
	dangerousEntity.insert(49); // ID of Skeleton
	dangerousEntity.insert(56); // ID of Spider
	dangerousEntity.insert(57); // ID of Giant
	dangerousEntity.insert(58); // ID of Zombie
	dangerousEntity.insert(59); // ID of Slime*/

	isFood.insert(260); //Apple ID
	isFood.insert(357); //Cookie ID
	isFood.insert(364); //Steak ID
	isFood.insert(365); //Raw Chicken ID
	isFood.insert(366); //Chicken ID
	
}
State* Amygdala::isCritical(State& state)
{
  int i;

  std::cout << "Amygdala::isCritical" << std::endl;

  if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < DROWNING) // Bot jumps if underwater
    {
      std::cout << "oxygen" << std::endl;
      State& reflexAction = *(new State());
      reflexAction[IDX_ACTION][IDX_JUMP] = true;
      return &reflexAction;
    }
  double x_target, y_target, z_target;
  if(isThereDangerousStuff(state, x_target, y_target, z_target))  //Bot runs away from dangerous blocks
    {
      std::cout << "dangerous stuff" << std::endl;
      State& reflexAction = runAway(state, x_target, y_target, z_target);
      return &reflexAction;
    }
	
  if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < HUNGRY) //Bot looks for food in carry if hungry, places it in hand and eats
    {
      i = 0;
      int id;
      int index;

      std::cout << "hungry" << std::endl;
      State& reflexAction = *(new State());

      while (!state[IDX_SENSATION][IDX_CARRY][i].isEmpty()){ //looks through what the bot carries

	id = (int) state[IDX_SENSATION][IDX_CARRY][i]["id"];
	if(isFood.count(id)){                         // checks if item is food
	  index = (int) state[IDX_SENSATION][IDX_CARRY][i]["index"];
	  reflexAction[IDX_ACTION][IDX_HAND][IDX_SWITCH] = index;        //puts current item in hand
	  reflexAction[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true; 
	  return &reflexAction;
	}
	i++;
      }	

      //  return &reflexAction;
    }
  //currentState[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES];
  return nullptr;
}

bool Amygdala::isThereDangerousStuff(State& state, double &x, double &y, double &z, std::set<int> const& list) const
{
	int i = 0;
	int id;
	while(! state[i].isEmpty())
	{
		id = (int) state[i]["id"];
		if(list.count(id) > 0)
		{
			x = (double) state[i][IDX_POSITION_X];
			y = (double) state[i][IDX_POSITION_Y];
			z = (double) state[i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	return false;
}

bool Amygdala::isThereDangerousStuff(State& state, double &x, double &y, double &z)
{
	return
	isThereDangerousStuff(state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES], x, y, z, dangerousEntity)
	||
	isThereDangerousStuff(state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES], x, y, z, dangerousEntity)
	||
	isThereDangerousStuff(state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES], x, y, z, dangerousEntity)
	||
	isThereDangerousStuff(state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS], x, y, z, dangerousBlock)
	||
	isThereDangerousStuff(state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS], x, y, z, dangerousBlock)
	||
	isThereDangerousStuff(state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS], x, y, z, dangerousBlock);
}

State& Amygdala::runAway(State& state, double x_foe, double y_foe, double z_foe)
{//TODO where will it goes if it's over lava
	State& reflex = *(new State());
	double x_me, z_me, yaw, x_object, z_object;
	double const distanceObject = 10;
	x_me = (double) state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_X];
	z_me = (double) state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Z];
	yaw = (double) state[IDX_SENSATION][IDX_LOCATION][IDX_ORIENTATION][IDX_YAW]; 
	yaw *= -1; //As explain below, degree are flipped
	//sin and cos are flipped because z and x are flipped too (z is abscissa)
	x_object = distanceObject * sin(yaw * PI / 180) + x_me; //Convert yaw in radian
	z_object = distanceObject * cos(yaw * PI / 180) + z_me; //Convert yaw in radian
	double x_v1, z_v1, x_v2, z_v2, n_v1, n_v2, c, s, theta, turn;
	//vector coordinate of me and a position ahead, and me and the foe
	x_v1 = z_object - z_me; //z is abscissa , because in minecraft y is the heigh 
	z_v1 = x_object - x_me;
	x_v2 = z_foe - z_me;
	z_v2 = x_foe - x_me;
	//In v1 and v2 x is abscissa
	//Angle between vetors v1 and v2
	n_v1 = sqrt(x_v1*x_v1 + z_v1*z_v1);
	n_v2 = sqrt(x_v2*x_v2 + z_v2*z_v2);
	c = (x_v1 * x_v2 + z_v1 * z_v2) / (n_v1 * n_v2);
	s = (x_v1 * z_v2 - x_v2 * z_v1);
	if(s < 0)
		s = -1;
	else
		s = 1;
	theta = s * acos(c) * (180 / PI); //180 / PI -> Convert back in degree
	//How much do I have to turn so my back face my foe
	turn = 180 + theta;
	if(turn > 180)
		turn = -180 + theta;
	//If you look on an axis, to go left you use positive number of degree
	//In ve, you need a negative one.
	//In ve, to turn right, you have to set rotation at 90°
	turn *= -1;
	//Then run
	if(fabs(turn) > 10)
	{
		reflex[IDX_ACTION][IDX_BODY][IDX_ROTATION] = turn;
		//Because your back facing the foe, no orientation needed. Just left 0 to go forward.
		reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = turn; //Move in the direction we are turning to
	}
	//Run like hell
	reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 15;
	reflex[IDX_ACTION][IDX_PRIMARY_ACTION] = "run away";
	printf("runAway foe(%f, %f) me(%f, %f) obj(%f, %f) theta(%f) yaw(%f) turn(%f)\n", x_foe, z_foe, x_me, z_me, x_object, z_object, theta, yaw, turn);
	return reflex;
}
