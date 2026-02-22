#include "ai.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <llama.h>
#include <string>
#include <vector>

AI::AI::AI(std::string modelpath) {
	path = modelpath;

	init();

	std::cout << "AI [" << id << "] is ready.\n";
}


/*
   RECOMMENDED SETTINGS ./llama-cli -hf Qwen/Qwen3-8B-GGUF:Q8_0 --jinja
   --color -ngl 99 -fa -sm row --temp 0.6 --top-k 20 --top-p 0.95 --min-p 0
   --presence-penalty 1.5 -c 40960 -n 32768 --no-context-shift
   */

void AI::AI::init() {
	id = ++GLOBAL_ID;

	params = llama_model_default_params();

	params.n_gpu_layers = 999; // force GPU
	n_predict = 1024;

	model = llama_model_load_from_file(path.c_str(), params);
	if (model == nullptr || model == NULL) {
		fprintf(stderr, "ERROR : Unable to laod model %d : %s", id, __func__);
	}

	vocab = llama_model_get_vocab(model);

	std::cout << "AI set id to [" << id << "]\n";
}

std::string AI::AI::get_prompt(std::string user_prompt, MODE mode) {
	std::string prompt;
	prompt += "<|im_start|>";
	if (mode == CHAT) {
		gbnf = R"(
root ::= <think> thinking </think> .*
thinking ::= !</think>*
    )";
		prompt +=
		"system: your role is to find out as much external and intern info "
		"about "
		"a user. external may be : job, passions, current state, internal may "
		"be "
		": personality, ambitions. You will extract this info and we'll use "
		"continously to deepen the internal and external profile of the user. "
		"Be carefull to not overwhelm the user with too many questions and too "
		"diverse, keep them tight. Try to figure out also what the user wants "
		"to talk about. DO NOT INVENT USER MODEL CONVERSATION, AFTER THE USER "
		"RESPONSE DO ONLY ONE ANSWER. If the user repets the question, do not "
		"repeat the answer!";
	} else if (mode == EXPORT) {
		prompt +=
			"system: You are the AI that builds a user profile. you task is the "
			"following: extract from the current conversation the user internal "
			"and external profile, output in a short format, almost like a user "
			"ID, how the user is both in an external and external profile. At the "
			"end, give the user a title based on his/hers passions. It should be "
			"cool, like a description for a hero. Your message will be in JSON "
			"format. It will contain external : list, internal : list, title : "
			"title";
		gbnf = R"(
root   ::= object
value  ::=  string ws

object ::=
  "{" ws (
            string ":" ws value
    ("," ws string ":" ws value)*
  )? "}" ws

category ::= "internal" | "external" | "title" ws

string ::=
  "\"" (
    [^"\\\x7F\x00-\x1F] |
    "\\" (["\\bfnrt] | "u" [0-9a-fA-F]{4}) # escapes
  )* "\"" ws

ws ::= | " " | "\n" [ \t]{0,20}
    )";
	} else if (mode == GENERATE_GOAL) {
		gbnf = R"(
root ::= <think> thinking </think> .*
thinking ::= !</think>*
    )";
		prompt +=
		"system: your role is to generate a mid-long-term goal for the user to "
		"follow. so far, based on the current user external and internal "
		"profile you received, generate a mid to long-term goal tailroed for "
		"the user.Be realistic fun and tailored for the user. Your sentance "
		"will be short and pragmatic. Maximum 50 words. No instructions "
		"needed, just the sentance that describes the picture. Be "
		"professional. The goal should be a project-like goal, not a long-term "
		"one";
	}else if (mode == SPLIT_GOAL){
		gbnf = R"(
root ::= item+

# Excludes various line break characters
item ::= "- " [^\r\n\x0b\x0c\x85\u2028\u2029]+ "\n"
    )";
		prompt += "You are a creative model who is in charge for "
		"breaking a human.  "
		"goal into logical subgoals. Keep your response short, "
		"mechanical and accurate. No titles, no fancy formating, just "
		"steps. Your answers are straight forward and not vague, they "
		"are meant to guide a lost person. Your answer will be processed in "
		"list form according do youi grammar, so make sure to focus on the "
		"items themselfs, only steps are relevant, not any extra info. The "
		"steps are named like book chapters to spark interest (example : "
		"'Provide sunlight' -> 'Providing the necessary sunlight'). You "
		"will receive the details starting with the parent goals of the "
		"current to make sure you know the context. You will only split the "
		"present one. Provide as many steps as you want (max 7). Make sure "
		"steps are "
		"not repetitive!";
	}
	prompt += "<|im_end|>";
	prompt += "<|im_start|>user: ";
	prompt += user_prompt;
	prompt += "<|im_end|>";
	prompt += "<|im_start|>model : ";
	prompt += "<|im_end|>";
	return prompt;
}

std::string AI::AI::run(std::string user_prompt, MODE mode) {
	std::string output = "";
	std::string prompt = get_prompt(user_prompt, mode);

	// find the number of tokens in the prompt
	const int n_prompt = -llama_tokenize(vocab, prompt.c_str(), prompt.size(),
			NULL, 0, true, true);

	// allocate space for the tokens and tokenize the prompt
	std::vector<llama_token> prompt_tokens(n_prompt);
	if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(),
				prompt_tokens.size(), true, true) < 0) {
		fprintf(stderr, "%s: error: failed to tokenize the prompt\n", __func__);
		return "ERROR (see in logs)";
	}

	llama_context_params ctx_params = llama_context_default_params();
	ctx_params.n_ctx = n_prompt + n_predict + 1;
	ctx_params.n_batch = n_prompt;
	ctx_params.no_perf = false;

	ctx = llama_init_from_model(model, ctx_params);

	if (ctx == NULL) {
		fprintf(stderr, "%s: error: failed to create the llama_context\n",
				__func__);
		return "ERROR (see in logs)";
	}

	auto sparams = llama_sampler_chain_default_params();
	sparams.no_perf = false;
	smpl = llama_sampler_chain_init(sparams);
	// initialize the sampler
	llama_sampler_chain_add(
			smpl, llama_sampler_init_grammar(vocab, gbnf.c_str(), "root"));
	llama_sampler_chain_add(smpl, llama_sampler_init_greedy());
	//  llama_sampler_chain_add(smpl, llama_sampler_init_top_p(0.95, 0.05));
	// llama_sampler_chain_add(smpl, llama_sampler_init_top_k(.25));
	// llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.6));

	// prepare batch
	llama_batch batch =
		llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());

	if (llama_model_has_encoder(model)) {
		if (llama_encode(ctx, batch)) {
			fprintf(stderr, "%s : failed to eval\n", __func__);
			return "ERROR (see in logs)";
		}

		llama_token decoder_start_token_id = llama_model_decoder_start_token(model);
		if (decoder_start_token_id == LLAMA_TOKEN_NULL) {
			decoder_start_token_id = llama_vocab_bos(vocab);
		}

		batch = llama_batch_get_one(&decoder_start_token_id, 1);
	}

	// main loop

	llama_token new_token_id;

	for (int n_pos = -100; n_pos + batch.n_tokens < n_prompt + n_predict;) {
		// evaluate the current batch with the transformer model
		if (llama_decode(ctx, batch)) {
			fprintf(stderr, "%s : failed to eval, return code %d\n", __func__, 1);
			return "ERROR (see in logs)";
		}

		n_pos += batch.n_tokens;

		// sample the next token
		{
			new_token_id = llama_sampler_sample(smpl, ctx, -1);
			// llama_sampler_accept(smpl, new_token_id);

			if (llama_vocab_is_eog(vocab, new_token_id)) {
				break;
			}

			char buf[128];
			int n =
				llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);

			if (n < 0) {
				fprintf(stderr, "%s: error: failed to convert token to piece\n",
						__func__);
				return "ERROR (see in logs)";
			}

			std::string s(buf, n);

			output += s;
			std::cout << s;

			// prepare the next batch with the sampled token
			batch = llama_batch_get_one(&new_token_id, 1);
		}
	}

	return output;
}

void AI::AI::modifyBrain(std::string input) {
	auto brain = Brain::getInstance();
	std::string newInfo = input == "add 1" ? brain->external_profile + " + 1 "
		: brain->external_profile;
	brain->updateExternalProfile(newInfo);
}

void AI::AI::set_grammar(std::string new_gbnf) { gbnf = new_gbnf; }
std::string AI::AI::get_grammar() { return gbnf; }

int AI::AI::GLOBAL_ID = 0;
