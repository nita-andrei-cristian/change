#include "brain.hpp"
#include "utils.hpp"
#include "llama.h"

void initializeBrain() {
	auto brain = Brain::getInstance();
	brain->internal_profile = "Intelect:70/100\nEmotional:50/100";
	brain->external_profile = "Job:Manager\nName:Richard";
}

void disableLogs() {
	llama_log_set(
			[](enum ggml_log_level level, const char *text, void * /* user_data */) {
			if (level >= GGML_LOG_LEVEL_ERROR) {
			fprintf(stderr, "%s", text);
			}
			},
			nullptr);
}
