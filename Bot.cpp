#include "Bot.hpp"
#include <iostream>
#include <vector>
#include "config.hpp"
#include "Thalamus.hpp"

using botplug::Block;

Bot::Bot() : BotAPI()
{
	initBot();
}
Bot::Bot(const std::string file) : BotAPI(file)
{
	initBot();
}
Bot::Bot(const std::string addr, const unsigned int port, const std::string login) : BotAPI(addr,port,login)
{
	initBot();
}
int Bot::brainDo()
{
	//double temperature = getBodyTemperature();
	fprintf(stderr, "health=%f food=%f water=%f energy=%f oxygen=%f time=%d\n",
			getHealth(), getFood(), getWater(), getEnergy(), getOxygen(), botSamplingTime);

	Bot::clearAll();
	State currentState = new State();
	inputCurrentState(currentState);
	std::cout << (String) currentState[IDX_SENSATION][IDX_VISION][IDX_ENTITIES] << std::endl;
	std::cout << (String) currentState[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES] << std::endl;
	
	//switchItem(1);
	
	thalamus->newState(currentState);

//	Hippocampus* hippo = new Hippocampus();
//	State stateBis = currentState;
//	stateBis[IDX_SENSATION][IDX_VITAL][IDX_FOOD] = 0.3;
// 	stateBis[IDX_WORLD] = "worldy";	
//	hippo->distance(currentState,stateBis);
	
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
				xpos, ypos, zpos, yaw, pitch);
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
	std::cerr << "Init Bot" << std::endl;
	thalamus = new braincraft::Thalamus(*this);
	botMaxSamplingTime = 0;
}
Bot::~Bot()
{
	delete thalamus;
}
bool Bot::stopCondition() const
{
	return botSamplingTime < botMaxSamplingTime;
}
void Bot::inputCurrentState(State& currentState)
{
	currentState.reset("");
	currentState[IDX_NOW][IDX_TIME_STEP] = botSamplingTime;
	currentState[IDX_NOW][IDX_REAL_TIME] = (unsigned int) time(NULL);
	currentState[IDX_WORLD] = getWorld();
	// Position/Orientation
	currentState[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_X] = getPosition()._x;
	currentState[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Y] = getPosition()._y;
	currentState[IDX_SENSATION][IDX_LOCATION][IDX_POSITION][IDX_POSITION_Z] = getPosition()._z;
	currentState[IDX_SENSATION][IDX_LOCATION][IDX_ORIENTATION][IDX_YAW] = getDirection()._yaw;
	currentState[IDX_SENSATION][IDX_LOCATION][IDX_ORIENTATION][IDX_PITCH] = getDirection()._pitch;
	// Vital variables
	currentState[IDX_SENSATION][IDX_VITAL][IDX_HEALTH] = getHealth();
	currentState[IDX_SENSATION][IDX_VITAL][IDX_FOOD] = getFood();
	currentState[IDX_SENSATION][IDX_VITAL][IDX_WATER] = getWater();
	currentState[IDX_SENSATION][IDX_VITAL][IDX_ENERGY] = getEnergy();
	currentState[IDX_SENSATION][IDX_VITAL][IDX_OXYGEN] = getOxygen();
	// Hand variables
	currentState[IDX_SENSATION][IDX_HAND][IDX_ITEM] = getItemInHand();
	currentState[IDX_SENSATION][IDX_HAND][IDX_ROUGHNESS] = getHandRoughness();
	currentState[IDX_SENSATION][IDX_HAND][IDX_HARDNESS] = getHandHardness();
	// Olfaction
	currentState[IDX_SENSATION][IDX_OLFACTION][IDX_INTENSIVITY] = getOlfactionIntensity();
	currentState[IDX_SENSATION][IDX_OLFACTION][IDX_FRIENDLYNESS] = getOlfactionFriendlyness();
	currentState[IDX_SENSATION][IDX_OLFACTION][IDX_EDIBLENESS] = getOlfactionEdibleness();
	currentState[IDX_SENSATION][IDX_OLFACTION][IDX_TOXICITY] = getOlfactionToxicity();
	// Visual-field
	{
		currentState[IDX_SENSATION][IDX_VISUAL_FIELD][IDX_RANGE] = getViewRange();
		currentState[IDX_SENSATION][IDX_VISUAL_FIELD][IDX_ANGLE] = getViewAngle();
		//botplug::PixelicView view(*this);
		//currentState[IDX_SENSATION][IDX_VISUAL_FIELD]["width"] = view.getWidth();
		//currentState[IDX_SENSATION][IDX_VISUAL_FIELD]["height"] = view.getHeight();
		currentState[IDX_SENSATION][IDX_VISUAL_FIELD][IDX_TACTILE_RANGE] = getTactileRange();
	}
	// Blocks perception
	inputCurrentState(currentState[IDX_SENSATION][IDX_VISION][IDX_BLOCKS], getVisualField());
	inputCurrentState(currentState[IDX_SENSATION][IDX_NEARBY][IDX_BLOCKS], getNearbyField());
	inputCurrentState(currentState[IDX_SENSATION][IDX_TOUCH][IDX_BLOCKS], getTouchField());
	inputCurrentState(currentState[IDX_SENSATION][IDX_VISION][IDX_ENTITIES], getEntities("vision"));
	inputCurrentState(currentState[IDX_SENSATION][IDX_NEARBY][IDX_ENTITIES], getEntities("nearby"));
	inputCurrentState(currentState[IDX_SENSATION][IDX_TOUCH][IDX_ENTITIES], getEntities("touch"));
	inputCurrentState(currentState[IDX_SENSATION][IDX_CARRY], carryList());
	inputCurrentState(currentState[IDX_SENSATION][IDX_INVENTORY], inventoryList());
	inputCurrentState(currentState[IDX_SENSATION][IDX_LOCKER], lockerList());
}
void Bot::inputCurrentState(State& currentSubState, const std::vector<botplug::Block>& blocks)
{
	for(unsigned int i = 0; i < blocks.size(); i++) {
		State block;
		block.reset("{'id':'%d','x':'%f','y':'%f','z':'%f','distance':'%f','roughness':'%f','hardness':'%f','temperature':'%f','color':'#%x'}",
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
		entity.reset("{'id':'%d','x':'%f','y':'%f','z':'%f'}",
				entities[i]._typeId, entities[i]._p._x, entities[i]._p._y, entities[i]._p._z);
		currentSubState.add(entity);
	}
}
void Bot::outputCurrentState(State& currentState)
{
	if(!currentState[IDX_ACTION][IDX_BODY][IDX_ROTATION].isEmpty()) {
		Bot::setBodyRotation((double) currentState[IDX_ACTION][IDX_BODY][IDX_ROTATION]);
	}
	if(!(currentState[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED].isEmpty() || currentState[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION].isEmpty())) {
	  Bot::setBodyTranslation((double) currentState[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_SPEED], (double) currentState[IDX_ACTION][IDX_BODY][IDX_TRANSLATION][IDX_ORIENTATION]);
	}
	if(!currentState[IDX_ACTION][IDX_GESTURE][IDX_REST].isEmpty()) {
		Bot::setIngest((bool) currentState[IDX_ACTION][IDX_GESTURE][IDX_REST]);
	}
	if(!(currentState[IDX_ACTION][IDX_HEAD][IDX_VERTICAL].isEmpty() || currentState[IDX_ACTION][IDX_HEAD][IDX_HORIZONTAL].isEmpty())) {
		Bot::setHeadMove((double) currentState[IDX_ACTION][IDX_HEAD][IDX_VERTICAL], (double) currentState[IDX_ACTION][IDX_HEAD][IDX_HORIZONTAL]);
	}
	if(!currentState[IDX_ACTION][IDX_ATTACK][IDX_STRONGNESS].isEmpty()) {
		Bot::setAttack((double) currentState[IDX_ACTION][IDX_ATTACK][IDX_STRONGNESS]);
	}
	if(!currentState[IDX_ACTION][IDX_JUMP].isEmpty()) {
		Bot::setJump((bool) currentState[IDX_ACTION][IDX_JUMP]);
		//Bot::setJump((bool) currentState[IDX_ACTION][IDX_JUMP]);
	}
	if(!currentState[IDX_ACTION][IDX_GESTURE][IDX_TOUCH].isEmpty()) {
		Bot::setIngest((bool) currentState[IDX_ACTION][IDX_GESTURE][IDX_TOUCH]);
	}
	if(!currentState[IDX_ACTION][IDX_GESTURE][IDX_INGEST].isEmpty()) {
		Bot::setIngest((bool) currentState[IDX_ACTION][IDX_GESTURE][IDX_INGEST]);
	}
	if(!currentState[IDX_ACTION][IDX_GESTURE][IDX_THROW].isEmpty()) {
		Bot::setIngest((bool) currentState[IDX_ACTION][IDX_GESTURE][IDX_THROW]);
	}
	if(!currentState[IDX_ACTION][IDX_GESTURE][IDX_CLIMB].isEmpty()) {
		Bot::setIngest((bool) currentState[IDX_ACTION][IDX_GESTURE][IDX_CLIMB]);
	}
	if(!currentState[IDX_ACTION][IDX_HAND][IDX_SWITCH].isEmpty()){
	  Bot::switchItem((int) currentState[IDX_ACTION][IDX_HAND][IDX_SWITCH]);
	}
	if(!(currentState[IDX_ACTION][IDX_HAND][IDX_GET_SLOT].isEmpty() && currentState[IDX_ACTION][IDX_HAND][IDX_PUT_SLOT].isEmpty())) {
		if(currentState[IDX_ACTION][IDX_HAND][IDX_PUT_SLOT].isEmpty()) {
			Bot::switchItem((int) currentState[IDX_ACTION][IDX_HAND][IDX_GET_SLOT]);
		} else {
			Bot::moveItem(currentState[IDX_ACTION][IDX_HAND][IDX_GET_SLOT].isEmpty() ? 0 : (int) currentState[IDX_ACTION][IDX_HAND][IDX_GET_SLOT],
					(int) currentState[IDX_ACTION][IDX_HAND][IDX_PUT_SLOT],
					currentState[IDX_ACTION][IDX_HAND]["count"].isEmpty() ? 1 : (int) currentState[IDX_ACTION][IDX_HAND]["count"]);
		}
	}
}
void Bot::doAction(State& action)
{
	outputCurrentState(action);
}
