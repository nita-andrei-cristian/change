#include <brain.hpp>

Brain *Brain::getInstance() {
  if (instancePtr == nullptr) {
    std::lock_guard<std::mutex> lock(mtx);
    if (instancePtr == nullptr) {
      instancePtr = new Brain();
    }
  }
  return instancePtr;
}

void Brain::updateExternalProfile(std::string newProfile) {
  this->external_profile = newProfile;
}

Brain *Brain::instancePtr = nullptr;
std::mutex Brain::mtx;

Brain::Brain() {}
