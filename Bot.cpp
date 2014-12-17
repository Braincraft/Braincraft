#include "Bot.hpp"
#include <vector>
#include "Thalamus.hpp"
#include "Action_braincraft.hpp"

using botplug::Block;

void Bot::doAction(braincraft::Action const& action)
{
	Bot::setJump(action.getJump());
	Bot::setBodyRotation(action.getRotation());
	Bot::setBodyTranslation(action.getSpeed(), action.getOrientation());
	Bot::setIngest(action.getEat());
}

int Bot::brainDo() {
	Thalamus t;
	t.print();

     // The bot-sampling-time must be incremented at each step
     fprintf(stderr, "Platypus It is good to be alive, Time = %d.\n", botSamplingTime++);

     // Clear the XML flow before sendong commands
     Bot::clearAll();
     // dump work but comment because is too verbose to let it by default
     //dump();

     // Random movement parameters
     double speed = frand_a_b(1, 2), orientation = frand_a_b(0, 360), rotation = frand_a_b(-90, 90);
     Bot::setBodyRotation(rotation);
     Bot::setBodyTranslation(speed, orientation);

     Bot::nTour++;

     return 1;

}


// pour les deplacements aleatoires ..
// renvoie aleatoirement entre deux bornes
double Bot::frand_a_b(double a, double b){
    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}


void Bot::dump()
{
  printf("\n\nWorld = '%s' time = %d\n", getWorld().c_str(), botSamplingTime);
  // Position/Orientation
  {
    double xpos = getPosition()._x;
    double ypos = getPosition()._y;
    double zpos = getPosition()._z;
    double yaw = getDirection()._yaw;
    double pitch = getDirection()._pitch;
    printf("Position (%f %f %f) orientation (%f %f) deg\n",
           xpos, ypos, zpos, 180 / M_PI * yaw, 180 / M_PI * pitch);
  }
  // Lymbic state
  {
    double health = getHealth();
    double food = getFood();
    double water = getWater();
    double energy = getEnergy();
    double oxygen = getOxygen();
    //double temperature = getBodyTemperature();
    printf("Lymbic state            health=%f food=%f water=%f energy=%f oxygen=%f \n",
           health, food, water, energy, oxygen);
  }
  // Environment elements
  {
    printf("Visual field range=%f angle=%f:\n", getViewRange(), getViewAngle());
    dump(getVisualField());
    printf("Nearby field:\n");
    dump(getNearbyField());
    // @tvieville impossible de faire cela : odorat ne fonctionne pas comme cela
    //printf("Olfactory field:\n");
    //dump(getOlfactoryField());
    printf("Touch field range=%f:\n", getTactileRange());
    dump(getTouchField());
    printf("Item in hand: id=%d roughness=%f hardness=%f \n",
           getItemInHand(), getHandRoughness(), getHandHardness());
  }
  // Pixelic view // comment because use CImg property and code must not be dependant of it
  {
    //botplug::PixelicView view(*this);
    //printf("Image width=%d height=%d\n", view.getWidth(), view.getHeight());
  }
  // Smell
  {
      printf("Smell intensity=%f friendlyness=%f edibleness=%f toxicity=%f\n", getOlfactionIntensity(), getOlfactionFriendlyness(), getOlfactionEdibleness() ,getOlfactionToxicity() );
  }
  // Inventory elements
  {
    printf("Inventory items:\n");
    dump(inventoryList(), inventoryFreeSlots());
    printf("Carry items:\n");
    dump(carryList(), carryFreeSlots());
    printf("Locker items:\n");
    dump(lockerList(), lockerFreeSlots());
  }
}


void Bot::dump(const std::vector<botplug::Block>& blocks)
{
  for(unsigned int i = 0; i < blocks.size(); i++)
    dump(blocks[i]);
}
void Bot::dump(const botplug::Block& block)
{
  printf("\tid=%d position=(%f %f %f) at d=%f roughness=%f hardness=%f temperature=%f color=0x%x\n",
         block._id, block._p._x, block._p._y, block._p._z, block._distance, block._roughness, block._hardness, block._temperature, block._color);
}
void Bot::dump(const std::vector<botplug::item>& items, const std::vector<bool>& slots)
{
  for(unsigned int i = 0; i < items.size(); i++) {
    const botplug::item& item = items[i];
    printf("\tid=%d slot=%d number=%d slot=%d\n",
           item._id, item._slot, item._number, slots[i]);
  }
}
// Manage the internal bot-sampling-time
void Bot::initBot()
{
  botMaxSamplingTime = 0;
}
bool Bot::stopCondition() const
{
  return botSamplingTime < botMaxSamplingTime;
}
