/**
 * @file   BotAPI.hpp
 * @author virtual_enaction_C@enseirb-matmeca.fr
 *
 * This is a dummy emty BotAPI with no output and random input, just to test marginal functions of the braynybot before real tests.
 * Only the high-level input/output functions are declared.
 *
 */

#ifndef BOTAPI_HPP
#define BOTAPI_HPP

#include <stdlib.h>
#include <string>
#include <vector>
#include "utils.hpp"

namespace botplug {
class BotAPI {
private:
  bool runStatus;
  Position emptyPosition;
  direction emptyDirection;
  std::vector<item> emptyItemList;
  std::vector<bool> emptyBoolList;
  std::vector<Entity> emptyEntityList;
  std::vector<Block> emptyBlockList;
  char dummyImageBuffer[400 * 300 * 3];
  static double dummyValue()
  {
    return rand() / ((double) RAND_MAX);
  }
  void initBotAPI()
  {
    runStatus = false;
  }
public:
  BotAPI();
  BotAPI(const std::string file);
  BotAPI(const std::string addr, const unsigned int port, const std::string login);
  virtual ~BotAPI();
  int run()
  {
    runStatus = true;
    while(!stopCondition() && runStatus) {
      if(!brainDo()) {
        runStatus = false;
      }
    }
    return runStatus;
  }
protected:
  virtual int brainDo();
  virtual bool stopCondition() const = 0;
public:
  int getStatus()
  {
    return runStatus;
  }
  void play() {}
  void pause() {}
  void stop()
  {
    runStatus = false;
  }
  Position getPosition() const
  {
    return emptyPosition;
  }
  direction getDirection() const
  {
    return emptyDirection;
  }
  std::string getWorld() const
  {
    return "dummy";
  }
  float getHandRoughness() const
  {
    return dummyValue();
  }
  float getHandHardness() const
  {
    return dummyValue();
  }
  float getHealth() const
  {
    return dummyValue();
  }
  float getFood() const
  {
    return dummyValue();
  }
  float getOxygen() const
  {
    return dummyValue();
  }
  float getWater() const
  {
    return dummyValue();
  }
  float getEnergy() const
  {
    return dummyValue();
  }
  float getOlfactionIntensity() const
  {
    return dummyValue();
  }
  float getOlfactionFriendlyness() const
  {
    return dummyValue();
  }
  float getOlfactionEdibleness() const
  {
    return dummyValue();
  }
  float getOlfactionToxicity() const
  {
    return dummyValue();
  }
  int getItemInHand() const
  {
    return 0;
  }
  std::vector<item> inventoryList() const
  {
    return emptyItemList;
  }
  std::vector<bool> inventoryFreeSlots() const
  {
    return emptyBoolList;
  }
  std::vector<item> carryList() const
  {
    return emptyItemList;
  }
  std::vector<bool> carryFreeSlots() const
  {
    return emptyBoolList;
  }
  std::vector<item> lockerList() const
  {
    return emptyItemList;
  }
  std::vector<bool> lockerFreeSlots() const
  {
    return emptyBoolList;
  }
  std::vector<Entity> getEntities(std::string) const
  {
    return emptyEntityList;
  }
  const char *getView() const
  {
    return dummyImageBuffer;
  }
  float getViewRange() const
  {
    return 0;
  }
  float getViewAngle() const
  {
    return 0;
  }
  float getTactileRange() const
  {
    return 0;
  }
  std::vector<Block> getVisualField() const
  {
    return emptyBlockList;
  }
  std::vector<Block> getNearbyField() const
  {
    return emptyBlockList;
  }
  std::vector<Block> getTouchField() const
  {
    return emptyBlockList;
  }
  void setBodyTranslation(float speed, float dir) {}
  void setBodyRotation(float horizontal) {}
  void setHeadMove(float vertical, float horizontal) {}
  void setTouch(bool touch) {}
  void setAttack(float t_arm) {}
  void setThrow(bool throwing) {}
  void setDrop(bool droping) {}
  void setIngest(bool ingest) {}
  void setClimb(bool climb) {}
  void clearAll() {}
  void setJump(float jump) {}
  void setRest(bool rest) {}
  void switchItem(int numitem) {}
  void setOpenInventory(bool) {}
  void moveItem(int src, int dst, int quantity) {}
  void throwItem(int slot, int quantity) {}
  void stopCraft() {}
  void craft(std::vector<item> vec) {}
};
}

#endif
