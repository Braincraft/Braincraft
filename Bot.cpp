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

int Bot::brainDo()
{
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
double Bot::frand_a_b(double a, double b)
{
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
void Bot::inputCurrentState(State& currentState)
{
	currentState.reset("");
	currentState["now"]["time-step"] = botSamplingTime;
	currentState["now"]["real-time"] = (unsigned int) time(NULL);
	currentState["world"] = getWorld();
	// Position/Orientation
	currentState["sensation"]["location"]["position"]["x"] = getPosition()._x;
	currentState["sensation"]["location"]["position"]["y"] = getPosition()._y;
	currentState["sensation"]["location"]["position"]["z"] = getPosition()._z;
	currentState["sensation"]["location"]["orientation"]["yaw"] = getDirection()._yaw;
	currentState["sensation"]["location"]["orientation"]["pitch"] = getDirection()._pitch;
	// Vital variables
	currentState["sensation"]["vital"]["health"] = getHealth();
	currentState["sensation"]["vital"]["food"] = getFood();
	currentState["sensation"]["vital"]["water"] = getWater();
	currentState["sensation"]["vital"]["energy"] = getEnergy();
	currentState["sensation"]["vital"]["oxygen"] = getOxygen();
	// Hand variables
	currentState["sensation"]["hand"]["item"] = getItemInHand();
	currentState["sensation"]["hand"]["roughness"] = getHandRoughness();
	currentState["sensation"]["hand"]["hardness"] = getHandHardness();
	// Olfaction
	currentState["sensation"]["olfaction"]["intensity"] = getOlfactionIntensity();
	currentState["sensation"]["olfaction"]["friendlyness"] = getOlfactionFriendlyness();
	currentState["sensation"]["olfaction"]["edibleness"] = getOlfactionEdibleness();
	currentState["sensation"]["olfaction"]["toxicity"] = getOlfactionToxicity();
	// Visual-field
	{
		currentState["sensation"]["visual-field"]["range"] = getViewRange();
		currentState["sensation"]["visual-field"]["angle"] = getViewAngle();
		//botplug::PixelicView view(*this);
		//currentState["sensation"]["visual-field"]["width"] = view.getWidth();
		//currentState["sensation"]["visual-field"]["height"] = view.getHeight();
		currentState["sensation"]["visual-field"]["tactile-range"] = getTactileRange();
	}
	// Blocks perception
	inputCurrentState(currentState["sensation"]["vision"]["blocks"], getVisualField());
	inputCurrentState(currentState["sensation"]["nearby"]["blocks"], getNearbyField());
	inputCurrentState(currentState["sensation"]["touch"]["blocks"], getTouchField());
	inputCurrentState(currentState["sensation"]["vision"]["entities"], getEntities("vision"));
	inputCurrentState(currentState["sensation"]["nearby"]["entities"], getEntities("nearby"));
	inputCurrentState(currentState["sensation"]["touch"]["entities"], getEntities("touch"));
	inputCurrentState(currentState["sensation"]["carry"], carryList());
	inputCurrentState(currentState["sensation"]["inventory"], inventoryList());
	inputCurrentState(currentState["sensation"]["locker"], lockerList());
}
void Bot::inputCurrentState(State& currentSubState, const std::vector<botplug::Block>& blocks)
{
	for(unsigned int i = 0; i < blocks.size(); i++) {
		State block;
		block.reset("{'id':'%d','position':{'x':'%f','y':'%f','z':'%f'},'distance':'%f','roughness':'%f','hardness':'%f','temperature':'%f','color':'#%x'}",
				blocks[i]._id, blocks[i]._p._x, blocks[i]._p._y, blocks[i]._p._z, blocks[i]._distance, blocks[i]._roughness, blocks[i]._hardness, blocks[i]._temperature, blocks[i]._color);
		currentSubState.add(block);
	}
}
void Bot::inputCurrentState(State& currentSubState, const std::vector<botplug::item>& items)
{
	for(unsigned int i = 0; i < items.size(); i++) {
		State item;
		item.reset("{'id':'%d','index':'%d','number':'%d'",
				items[i]._id, items[i]._slot, items[i]._number);
		currentSubState.add(item);
	}
}
void Bot::inputCurrentState(State& currentSubState, const std::vector<botplug::Entity>& entities)
{
	for(unsigned int i = 0; i < entities.size(); i++) {
		State entity;
		entity.reset("{'id':'%d','position':{'x':'%f','y':'%f','z':'%f'}}",
				entities[i]._id, entities[i]._p._x, entities[i]._p._y, entities[i]._p._z);
		currentSubState.add(entity);
	}
}
void Bot::outputCurrentState(State& currentState)
{
	if(!currentState["action"]["body"]["rotation"].isEmpty()) {
		Bot::setBodyRotation((double) currentState["action"]["body"]["rotation"]);
	}
	if(!(currentState["action"]["body"]["translation"]["speed"].isEmpty() || currentState["action"]["body"]["translation"]["orientation"].isEmpty())) {
		Bot::setBodyTranslation((double) currentState["action"]["body"]["translation"]["speed"], (double) currentState["action"]["body"]["translation"]["orientation"]);
	}
	if(!currentState["action"]["gesture"]["rest"].isEmpty()) {
		Bot::setIngest((bool) currentState["action"]["gesture"]["rest"]);
	}
	if(!(currentState["action"]["head"]["vertical"].isEmpty() || currentState["action"]["head"]["horizontal"].isEmpty())) {
		Bot::setHeadMove((double) currentState["action"]["head"]["vertical"], (double) currentState["action"]["head"]["horizontal"]);
	}
	if(!currentState["action"]["attack"]["strongness"].isEmpty()) {
		Bot::setAttack((double) currentState["action"]["attack"]["strongness"]);
	}
	if(!currentState["action"]["jump"]["height"].isEmpty()) {
		Bot::setAttack((double) currentState["action"]["jump"]["height"]);
	}
	if(!currentState["action"]["gesture"]["touch"].isEmpty()) {
		Bot::setIngest((bool) currentState["action"]["gesture"]["touch"]);
	}
	if(!currentState["action"]["gesture"]["ingest"].isEmpty()) {
		Bot::setIngest((bool) currentState["action"]["gesture"]["ingest"]);
	}
	if(!currentState["action"]["gesture"]["throw"].isEmpty()) {
		Bot::setIngest((bool) currentState["action"]["gesture"]["throw"]);
	}
	if(!currentState["action"]["gesture"]["climb"].isEmpty()) {
		Bot::setIngest((bool) currentState["action"]["gesture"]["climb"]);
	}
	if(!(currentState["action"]["hand"]["get"]["slot"].isEmpty() && currentState["action"]["hand"]["put"]["slot"].isEmpty())) {
		if(currentState["action"]["hand"]["put"].isEmpty()) {
			Bot::switchItem((int) currentState["action"]["hand"]["get"]["slot"]);
		} else {
			Bot::moveItem(currentState["action"]["hand"]["get"]["slot"].isEmpty() ? 0 : (int) currentState["action"]["hand"]["get"]["slot"],
					(int) currentState["action"]["hand"]["put"]["slot"],
					currentState["action"]["hand"]["count"].isEmpty() ? 1 : (int) currentState["action"]["hand"]["count"]);
		}
	}
}
