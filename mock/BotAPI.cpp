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
