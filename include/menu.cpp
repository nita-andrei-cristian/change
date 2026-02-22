#include "goal.hpp"
#include "utils.hpp"
#include <menu.hpp>
#include <iostream>

using namespace INTERFACE;

Menu::Menu(){
	profile = "";
	history = "message history : ";
	sentOneMessage = false;
	inputInt = -999;
	inputString = "";

	ai = new AI::AI("/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf");
}

Menu::~Menu(){
	delete ai;
	if (parentgoal != nullptr)
		delete parentgoal;
}

void Menu::drawMainMenu() {
	std::cout << "[q] - quit\n";
	std::cout << "[c] - clear\n";
	if (sentOneMessage)
		std::cout << "[e] - export\n";
	if (profile.length() > 0)
		std::cout << "[g] - generate goal\n";
	if (goal != nullptr)
		std::cout << "[s] - split goal\n";
}

void Menu::drawSplitGoalTool() {
	system("clear");
	std::cout << "[-1] - quit\n";

	std::cout << "\n Current Goal : " << goal->name << "\n\n";

	for (int i = 0; i < goal->childrensize; i++) {
		std::cout << "[" << i << "] : " << goal->children[i]->name << "\n";
	}

	std::cout << '\n';
}

void Menu::drawStartSession() {
	system("clear");
	std::cout << "\n---- Starting session ----\n";
}

void Menu::drawEndSession() {
	std::cout << "---- Session ended ----\n";
}

void Menu::readString() {
	std::cout << "\nType : ";
	getline(std::cin, inputString);
}

void Menu::readInt() {
	std::cout << "\nWrite [0-10]: ";
	std::cin >> inputInt;
}

void Menu::start(){
	initializeBrain();

	drawStartSession();

	while (true) {
		if (state == QUIT) break;
		update();
	}

	drawEndSession();
}

void Menu::enterSplitGoalTool(){
	state = SPLITGOAL;

	if (goal->childrensize == 0)
		SPLIT_GOAL(*goal, *ai); // split the actual goal

	drawSplitGoalTool();
	readInt();

	if (inputInt == -1){
		system("clear");
		return;
	}

	if (inputInt >= 0 && inputInt < goal->childrensize)
		goal = goal->children[inputInt];

}

void Menu::update(){
	readString();

	if (inputString == "q"){
		state = QUIT;
		return;
	}

	if (inputString == "c") {
		system("clear");
		drawMainMenu();
		return;
	}

	if (inputString == "e" && sentOneMessage) {
		profile = ai->run(history, AI::EXPORT);
		ai->modifyBrain(profile);
		return;
	}

	if (inputString == "g" && profile.length() > 0) {
		if (goal != nullptr) delete goal;
		goal = new Goal(ai->run(profile, AI::GENERATE_GOAL));
		parentgoal = goal;
		history += " AI GENERATED A GOAL FOR THE USER : " + goal->name + "\n";
		return;
	}

	
	if (inputString == "s" && goal != nullptr) {
		enterSplitGoalTool();
		state = MAINMENU;
		return;
	}


	
	history += "User said : " + inputString + '\n';
	auto output = ai->run(history, AI::CHAT);
	history += "AI said : " + output + "\n";
	
	system("clear");


	//std::cout << output << "\n";

	sentOneMessage = true;

	std::cout << std::endl << std::endl;
	drawMainMenu();

}

