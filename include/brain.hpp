#pragma once
#include <mutex>
#include <string>

class Brain {

private:
  static Brain *instancePtr;
  static std::mutex mtx;

  Brain();

public:
  Brain(const Brain &obj) = delete;

  std::string internal_profile;
  std::string external_profile;

  static Brain *getInstance();

  void updateExternalProfile(std::string newProfile);
};
