#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

#define HUNGRY 0.8
#define DROWNING 1

Amygdala::Amygdala():dangerousBlock(), isFood(), dangerousEntity()
{
	std::cerr << "Create Amygdala" << std::endl;
	dangerousBlock.insert(11);	//ID of lava
	dangerousBlock.insert(10);	//ID of spreading lava
	dangerousBlock.insert(51);	//ID of fire
	dangerousBlock.insert(52);	//ID of  mob spawner
	dangerousBlock.insert(81);	//ID of cactus
	

	for (int i = 50; i < 69 ; i ++) //ID of hostile mobs
	  dangerousEntity.insert(i);

	isFood.insert(260);			//Apple ID
	isFood.insert(357);			//Cookie ID
	isFood.insert(364);			//Steak ID
	isFood.insert(365);			//Raw Chicken ID
	isFood.insert(366);			//Chicken ID

	interrestingEntity.insert(92);
}

State *Amygdala::isCritical(State & state)
{
	int i;

	std::cout << "Amygdala::isCritical" << std::endl;
	double x_target, y_target, z_target;

	//if(isThereInterrestingStuff(state, x_target, y_target, z_target))
	//{
		//double my_x, my_z;
		//my_x = (double) state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_X];
		//my_z = (double) state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Z];
		//printf("There is interrestingEntity (%f,%f)\n", x_target, z_target);
		//State & reflexAction = aim(state, x_target, y_target, z_target);
		//double dist = sqrt(pow(my_x - x_target, 2) + pow(my_x - x_target, 2));
		//if(dist > 3) //Distance from where I can't reach the cow anymore
			//reflexAction[IDX_ACTION][IDX_ATTACK][IDX_STRONGNESS] = 1.0;
		//else
			//reflexAction[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 15;
		//return &reflexAction;
	//}
	if((double)state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < DROWNING)	// Bot jumps if underwater
	{
		std::cout << "oxygen" << std::endl;
		State & reflexAction = *(new State());
		reflexAction[IDX_ACTION][IDX_JUMP] = true;
		return &reflexAction;
	}
	if(isThereDangerousStuff(state, x_target, y_target, z_target))	//Bot runs away from dangerous blocks
	{
		std::cout << "dangerous stuff" << std::endl;
		State & reflexAction = runAway(state, x_target, y_target, z_target);
		return &reflexAction;
	}

	if((double)state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < HUNGRY)	//Bot looks for food in carry if hungry, places it in hand and eats
	{
		std::cout << "hungry" << std::endl;
		State *reflex = nullptr;
		if(tryToEat(state, reflex))
			return reflex;
	}
	return nullptr;
}

bool Amygdala::tryToEat(State & state, State* &reflex)
{
	int i = 0;
	int id;
	int index;
	while(!state[IDX_SENSATION][IDX_CARRY][i].isEmpty())
	{							//looks through what the bot carries

		id = (int)state[IDX_SENSATION][IDX_CARRY][i]["id"];
		if(isFood.count(id))
		{						// checks if item is food
			reflex = new State();
			State& reflexAction = *reflex;
			index = (int)state[IDX_SENSATION][IDX_CARRY][i]["index"];
			reflexAction[IDX_ACTION][IDX_HAND][IDX_SWITCH] = index;	//puts current item in hand
			reflexAction[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
			return true;
		}
		i++;
	}
	return false;
}

bool Amygdala::isThereDangerousStuff(State & state, double &x, double &y, double &z, std::set < int >const &list) const
{
	//check for members of the list in the state
	int i = 0;
	int id;
	while(!state[i].isEmpty())
	{
		id = (int)state[i]["id"];
		if(list.count(id) > 0)
		{
			x = (double)state[i][IDX_POSITION_X];
			y = (double)state[i][IDX_POSITION_Y];
			z = (double)state[i][IDX_POSITION_Z];
			return true;
		}
		++i;
	}
	return false;
}

bool Amygdala::isThereInterrestingStuff(State & state, double &x, double &y, double &z)
{
	return isThereDangerousStuff(state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES], x, y, z, interrestingEntity) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES], x, y, z, interrestingEntity) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES], x, y, z, interrestingEntity);
}
bool Amygdala::isThereDangerousStuff(State & state, double &x, double &y, double &z)
{
	return  //check hostiles entityies
			   isThereDangerousStuff(state[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES], x, y, z, dangerousEntity) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES], x, y, z, dangerousEntity) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_VISION][IDX_ENTITIES], x, y, z, dangerousEntity) 
			//check dangerous blocks
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS], x, y, z, dangerousBlock) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS], x, y, z, dangerousBlock) 
			|| isThereDangerousStuff(state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS], x, y, z, dangerousBlock);
}

State & Amygdala::runAway(State & state, double x_foe, double y_foe, double z_foe)
{								//TODO where will it goes if it's over lava
	State & reflex = *(new State());
	double x_me, z_me, yaw, x_object, z_object;
	double const distanceObject = 10;
	x_me = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_X];
	z_me = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Z];
	yaw = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_ORIENTATION][IDX_YAW];
	yaw *= -1;					//As explain below, degree are flipped
	//sin and cos are flipped because z and x are flipped too (z is abscissa)
	x_object = distanceObject * sin(yaw * PI / 180) + x_me;	//Convert yaw in radian
	z_object = distanceObject * cos(yaw * PI / 180) + z_me;	//Convert yaw in radian
	
	double x_v1, z_v1, x_v2, z_v2, n_v1, n_v2, c, s, theta, turn;
	//vector coordinate of me and a position ahead, and me and the foe
	x_v1 = z_object - z_me;		//z is abscissa , because in minecraft y is the heigh 
	z_v1 = x_object - x_me;
	x_v2 = z_foe - z_me;
	z_v2 = x_foe - x_me;
	
	//In v1 and v2 x is abscissa
	//Angle between vetors v1 and v2
	
	n_v1 = sqrt(x_v1 * x_v1 + z_v1 * z_v1);
	n_v2 = sqrt(x_v2 * x_v2 + z_v2 * z_v2);
	c = (x_v1 * x_v2 + z_v1 * z_v2) / (n_v1 * n_v2);
	s = (x_v1 * z_v2 - x_v2 * z_v1);
	if(s < 0)
		s = -1;
	else
		s = 1;
	theta = s * acos(c) * (180 / PI);	//180 / PI -> Convert back in degree
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
		reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = turn;	//Move in the direction we are turning to
	}
	
	//Run like hell
	reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED] = 15;
	reflex[IDX_ACTION][IDX_PRIMARY_ACTION] = "run away";
	printf("runAway foe(%f, %f) me(%f, %f) obj(%f, %f) theta(%f) yaw(%f) turn(%f)\n", x_foe, z_foe, x_me, z_me, x_object, z_object, theta, yaw, turn);
	return reflex;
}
State & Amygdala::aim(State & state, double x_foe, double y_foe, double z_foe)
{								//TODO where will it goes if it's over lava
	State & reflex = *(new State());
	double x_me, z_me, yaw, x_object, z_object;
	double const distanceObject = 10;
	x_me = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_X];
	z_me = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Z];
	yaw = (double)state[IDX_SENSATION][IDX_LOCATION][IDX_ORIENTATION][IDX_YAW];
	yaw *= -1;					//As explain below, degree are flipped
	//sin and cos are flipped because z and x are flipped too (z is abscissa)
	x_object = distanceObject * sin(yaw * PI / 180) + x_me;	//Convert yaw in radian
	z_object = distanceObject * cos(yaw * PI / 180) + z_me;	//Convert yaw in radian
	double x_v1, z_v1, x_v2, z_v2, n_v1, n_v2, c, s, theta, turn;
	//vector coordinate of me and a position ahead, and me and the foe
	x_v1 = z_object - z_me;		//z is abscissa , because in minecraft y is the heigh 
	z_v1 = x_object - x_me;
	x_v2 = z_foe - z_me;
	z_v2 = x_foe - x_me;
	//In v1 and v2 x is abscissa
	//Angle between vetors v1 and v2
	n_v1 = sqrt(x_v1 * x_v1 + z_v1 * z_v1);
	n_v2 = sqrt(x_v2 * x_v2 + z_v2 * z_v2);
	c = (x_v1 * x_v2 + z_v1 * z_v2) / (n_v1 * n_v2);
	s = (x_v1 * z_v2 - x_v2 * z_v1);
	if(s < 0)
		s = -1;
	else
		s = 1;
	theta = s * acos(c) * (180 / PI);	//180 / PI -> Convert back in degree
	//How much do I have to turn so my back face my foe
	turn = theta;
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
		reflex[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION] = turn;	//Move in the direction we are turning to
	}
	//Run like hell
	reflex[IDX_ACTION][IDX_PRIMARY_ACTION] = "aim";
	printf("aim foe(%f, %f) me(%f, %f) obj(%f, %f) theta(%f) yaw(%f) turn(%f)\n", x_foe, z_foe, x_me, z_me, x_object, z_object, theta, yaw, turn);
	return reflex;
}
