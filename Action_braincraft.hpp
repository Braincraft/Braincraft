#ifndef ACTION_BRAINCRAFT_HPP
#define ACTION_BRAINCRAFT_HPP

namespace braincraft
{
class Action
{
	double speed;
	double orientation;
	double rotation;
	bool jump;
	bool eat;
	void randomize(void);	
	public:
		Action(bool random = false);
		double getSpeed(void) const;
		void setSpeed(double const speed);
		double getOrientation(void) const;
		void setOrientation(double const orientation);
		double getRotation(void) const;
		void setRotation(double const rotation);
		bool getJump(void) const;
		void setJump(bool const jump);
		bool getEat(void) const;
		void setEat(bool const eat);
};
}
#endif
