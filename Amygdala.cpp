#include "Amygdala.hpp"
#include <iostream>
#include "config.hpp"
#include "State.hpp"

State* Amygdala::isCritical(State& state)
{
	std::cout << "Amygdala::isCritical" << std::endl;
	if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] < 1.0)
	{
		std::cout << " oxygen" << std::endl;
		State& reflexAction = *(new State());
		reflexAction[IDX_ACTION][IDX_JUMP] = true;
		return &reflexAction;
	}
	double x_target, y_target, z_target;
	if(isThereDangerousBlocks(state, x_target, y_target, z_target))
	{
		std::cout << "foe" << std::endl;
		State& reflexAction = runAway(state, x_target, y_target, z_target);
		return &reflexAction;
	}
	if( (double) state[IDX_SENSATION][IDX_VITAL][IDX_FOOD] < 0.5)
	{
		std::cout << "hungry" << std::endl;
		State& reflexAction = *(new State());
		//Mettre nourriture dans sa main (si jamais il n'y en a pas déjà dedans)
		reflexAction[IDX_ACTION][IDX_GESTURE][IDX_INGEST] = true;
		return &reflexAction;
	}
	//currentState[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES];
	return nullptr;
}
bool Amygdala::isThereDangerousBlocks(State& state, double &x, double &y, double &z)
{
	int i = 0;
	//TODO probably switch to a set or a list
	static int const lava_id = 11; //ID of lava
	while(! state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i].isEmpty())
	{
		if((int) state[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS][i]["id"] == lava_id)
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
		if((int) state[IDX_SENSATION][IDX_VISION][IDX_BLOCKS][i]["id"] == lava_id)
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
{
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
