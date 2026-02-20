#include <ai.hpp>
#include <brain.hpp>
#include <iostream>

void initializeBrain() {
  auto brain = Brain::getInstance();
  brain->internal_profile = "Intelect:70/100\nEmotional:50/100";
  brain->external_profile = "Job:Manager\nName:Richard";
}

int main() {
  auto ai = AI::AI();

  initializeBrain();

  std::cout << "\n---- Starting session ----\n(write quit to exit)\n\n";
  while (true) {

    std::string input;
    std::cout << "Write something about yourself : ";
    getline(std::cin, input);

    if (input == "quit")
      break;

    auto output = ai.run(input);
    std::cout << "\nAI Model said:\n" << output << "\n";

    ai.modifyBrain("User just said : " + input);
  }

  std::cout << "---- Session ended ----\n";
}
