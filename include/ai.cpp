#include "ai.hpp"
#include <iostream>

AI::AI::AI() {
  init();
  std::cout << "AI [" << id << "] is ready.\n";
}

void AI::AI::init() {
  id = ++GLOBAL_ID;
  std::cout << "AI set id to [" << id << "]\n";
}

std::string AI::AI::run(std::string input) {
  std::string response = input == "hi" ? "Hello!\n" : "I don't know!\n";
  return response;
}

void AI::AI::modifyBrain(std::string input) {
  auto brain = Brain::getInstance();
  std::string newInfo = input == "add 1" ? brain->external_profile + " + 1 "
                                         : brain->external_profile;
  brain->updateExternalProfile(newInfo);
}

int AI::AI::GLOBAL_ID = 0;
