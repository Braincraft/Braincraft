#include "Action_braincraft.hpp"
namespace braincraft
{
Action::Action(bool random)
{
	if(random)
		randomize();
}
void Action::randomize(void)
{
}
double Action::getSpeed(void) const
{
	return speed;
}
void Action::setSpeed(double const speed)
{
	this->speed = speed;
}
double Action::getOrientation(void) const
{
	return orientation;
}
void Action::setOrientation(double const orientation)
{
	this->orientation = orientation;
}
double Action::getRotation(void) const
{
	return rotation;
}
void Action::setRotation(double const rotation)
{
	this->rotation = rotation;
}
bool Action::getJump(void) const
{
	return jump;
}
void Action::setJump(bool const jump)
{
	this->jump = jump;
}
bool Action::getEat(void) const
{
	return eat;
}
void Action::setEat(bool const eat)
{
	this->eat = eat;
}
}
