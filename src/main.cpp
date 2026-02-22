#include <ai.hpp>
#include <brain.hpp>
#include <utils.hpp>
#include <menu.hpp>

int main() {
	auto ai = AI::AI("/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf");

	ggml_backend_load_all();

	INTERFACE::Menu* menu = new INTERFACE::Menu();
	menu->start();

	delete menu;
}
