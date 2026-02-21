#pragma once

#include "llama.h"
#include <brain.hpp>
#include <string>

namespace AI {

class AI {
private:
  int id;
  static int GLOBAL_ID;

  llama_model *model;

  llama_model_params params;
  const llama_vocab *vocab;
  llama_sampler *smpl;
  llama_context *ctx;

  int n_predict;
  int n_parallel;

  std::string gbnf;
  std::string path;

  void init();
  std::string get_prompt(std::string user_prompt);

public:
  AI(std::string path);

  std::string run(std::string);
  void modifyBrain(std::string);

  void set_grammar(std::string new_gbnf);
  std::string get_grammar();
};

} // namespace AI
