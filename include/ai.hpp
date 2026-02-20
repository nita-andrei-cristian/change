#pragma once

#include <brain.hpp>
#include <string>

namespace AI {

class AI {
private:
  int id;
  static int GLOBAL_ID;

  void init();

public:
  AI();

  std::string run(std::string);
  void modifyBrain(std::string);
};

} // namespace AI
