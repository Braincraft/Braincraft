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
  double const high = 10;
  double const speed = 10;
  int randChoice = rand() %100;
  
  
  if(randChoice < 10) {//eat something
    setEat(true);
  }
  else if(randChoice <30) {//Walk forward
    setSpeed(speed);
  }
  else if(randChoice <50) {//Change orientation
    double randOrientation = rand() %360;
    setOrientation(randOrientation);
  }

  else if(randChoice <60) {//Jump
    setJump(high);
  }

  else {//stop everything
    setSpeed(0);
    setRotation(0);
  }
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
