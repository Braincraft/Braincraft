#ifndef BOT_HPP
#define BOT_HPP
#define STEPS 1000

#include "BotAPI.hpp"

using botplug::BotAPI;

class Bot : public BotAPI {
private:
  int n;
  /**
   * @brief FR : Fonction d'initialisation du bot. / EN : bot's initialization function
   */
  void initBot();
  /**
   * @brief FR : Condition d'arrêt du bot. / EN : bot's stop condition
   * @return FR : vrai si le bot doit s'arrêter, faux sinon. / EN : true if the bot must stop, else false
   */
  bool stopCondition() const;
  // for pixel view
   // Defines the internal bot-sampling-time incremented at each step
   unsigned int botSamplingTime = 0, botMaxSamplingTime = 1000;
   static void dump(const std::vector<botplug::Block>& blocks);
   static void dump(const botplug::Block& block);
   static void dump(const std::vector<botplug::item>& items, const std::vector<bool>& slots);
   // These are initialization/stopping mechanism.

protected:
  /**
   * @brief FR : Contient l'algorithme d'apprentissage/le cerveau du bot. / EN : Contain the learning algorithym/ the bot's brain
   * @return
   */
  int brainDo();

public:
  Bot() : BotAPI(){initBot();}
  Bot(const std::string file) : BotAPI(file){initBot();}
  // Botlogin
  Bot(const std::string addr, const unsigned int port, const std::string login) : BotAPI(addr,port,login){initBot();}
  virtual ~Bot(){}
  double frand_a_b(double a, double b);
  void dump();
  int nTour = 0;
  };

#endif
