#include "BotAPI.hpp"

namespace botplug {

BotAPI::BotAPI() {
    initBotAPI();
}

BotAPI::BotAPI(const std::string file) {
    initBotAPI();
}

BotAPI::BotAPI(const std::string addr, const unsigned int port, const std::string login) {
    initBotAPI();
}
BotAPI::~BotAPI() {
}
int  BotAPI::brainDo() {}
}

int main()
{
  State s1 = new State();
  State s2 = new State();

  s1.load("./0.state.json");
  s2.load("./1.state.json");
  
  Hippocampus* hippo new Hippocampus();
  std::cout << hippo->distance(s1,s2) << std::endl;
  
  return 0;
}
