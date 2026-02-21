#include "llama.h"
#include <ai.hpp>
#include <brain.hpp>
#include <iostream>
#include <locale>

void initializeBrain() {
  auto brain = Brain::getInstance();
  brain->internal_profile = "Intelect:70/100\nEmotional:50/100";
  brain->external_profile = "Job:Manager\nName:Richard";
}

int main() {
  auto ai = AI::AI("/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf");

  ggml_backend_load_all();

  initializeBrain();

  llama_log_set(
      [](enum ggml_log_level level, const char *text, void * /* user_data */) {
        if (level >= GGML_LOG_LEVEL_ERROR) {
          fprintf(stderr, "%s", text);
        }
      },
      nullptr);

  system("clear");
  std::cout << "\n---- Starting session ----\n";
  std::cout << "[q] - quit\n";
  std::cout << "[c] - clear\n";
  std::cout << '\n';

  std::string history = "message history : ";
  while (true) {

    std::string input;
    std::cout << "\nWrite something about yourself : ";
    getline(std::cin, input);

    if (input == "q")
      break;

    if (input == "c") {
      system("clear");
      continue;
    }

    history += "User said : " + input + '\n';
    auto output = ai.run(history);
    // std::cout << "AI Model said: " << output << "\n";
    history += "AI said : " + output + "\n";

    ai.modifyBrain("User just said : " + input);
  }

  std::cout << "---- Session ended ----\n";
}
