#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

Amygdala::Amygdala() : dangerousBlock(), isFood(), dangerousEntity()
{
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
	
	isFood.insert(260); //Apple ID
	isFood.insert(357); //Cookie ID
	isFood.insert(364); //Steak ID
	isFood.insert(365); //Raw Chicken ID
	isFood.insert(366); //Chicken ID
	std::cout << "loutre " << isFood.count(364) << " " << dangerousBlock.count(11) << std::endl;
}
State* Amygdala::isCritical(State& state)
{
  int i;

  std::cout << "Amygdala::isCritical" << std::endl;
  //std::cout << (int) state[IDX_SENSATION][IDX_HAND][IDX_ITEM] << std::endl;
  //std::cout << (String) state[IDX_SENSATION][IDX_CARRY] << std::endl;

  if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < 1.0) // Bot jumps if underwater
    {
      std::cout << "oxygen" << std::endl;
      State& reflexAction = *(new State());
      reflexAction[IDX_ACTION][IDX_JUMP] = true;
      return &reflexAction;
    }
  double x_target, y_target, z_target;
  if(isThereDangerousBlocks(state, x_target, y_target, z_target))  //Bot runs away from dangerous blocks
    {
      std::cout << "dangerous block" << std::endl;
      State& reflexAction = runAway(state, x_target, y_target, z_target);
      return &reflexAction;
    }
	
  if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] <= 1) //Bot looks for food in carry if hungry, places it in hand and eats
    {
      i = 0;
      int id;
      int index;

      std::cout << "hungry" << std::endl;
      State& reflexAction = *(new State());

      while (!state[IDX_SENSATION][IDX_CARRY][i].isEmpty()){ //looks through what the bot carries
	
	id = (int) state[IDX_SENSATION][IDX_CARRY][i]["id"];

	std::cout << isFood.count(id) <<" " << id << std::endl;
	if(isFood.count(id)){                         // checks if item is food
	  index = (int) state[IDX_SENSATION][IDX_CARRY][i]["index"];
	  //state[IDX_ACTION][IDX_HAND][IDX_PUT_SLOT] = true;
	  state[IDX_ACTION][IDX_HAND][IDX_GET_SLOT] = index;        //puts current item in hand
     
	  std::cout << "Coucou je rentre bien dans le if" << std::endl;
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
bool Amygdala::isThereDangerousBlocks(State& state, double &x, double &y, double &z)
// for blocks and entities
{
	int i = 0;
	int id;
	while(! state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES][i]["id"];
		if(dangerousEntity.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	while(! state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES][i]["id"];
		if(dangerousEntity.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	while(! state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES][i]["id"];
		if(dangerousEntity.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	while(! state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS][i]["id"];
		if(dangerousBlock.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	while(! state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i]["id"];
		if(dangerousBlock.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	while(! state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i].isEmpty())
	{
		id = (int) state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i]["id"];
		if(dangerousBlock.count(id) > 0)
		{
			x = (double) state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i][IDX_POSITION_X];
			y = (double) state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i][IDX_POSITION_Y];
			z = (double) state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}


	return false;
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
		reflex[IDX_ACTION][IDX_BODY][IDX_ROTATION] = turn;
	//Run like hell
	reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 15;
	//Because your back facing the foe, no orientation needed. Just left 0 to go forward.
	reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = turn; //Move in the direction we are turning to
	printf("runAway foe(%f, %f) me(%f, %f) obj(%f, %f) theta(%f) yaw(%f) turn(%f)\n", x_foe, z_foe, x_me, z_me, x_object, z_object, theta, yaw, turn);
	return reflex;
}
