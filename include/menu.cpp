#include "goal.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <menu.hpp>
#include <iostream>

using namespace INTERFACE;

Menu::Menu(){
	
	userIndex = 0;

	for (int i = 0; i < USERS; i++){
		profile[i] = "";
		history [i]= "message history : ";
		lastMessage[i] = "";
		inputInt[i] = -999;
		inputString[i] = "";
		goal[i] = nullptr;
		parentgoal[i] = nullptr;
	}

	ai = new AI::AI("/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf");
}

Menu::~Menu(){
	delete ai;
	for (int i = 0; i < USERS; i++){
		if (parentgoal[i] != nullptr)
			delete parentgoal[i];
	}
}

void Menu::drawMainMenu() {
	std::cout << "Welcome user [" << userIndex << "]\n";

	if (lastMessage[userIndex].length() > 0){
		std::cout << "\nAI response : \n" << lastMessage[userIndex] << "\n\n";
	}

	std::cout << "[q] - quit\n";
	std::cout << "[c] - clear\n";
	std::cout << "[u] - go to next user\n";
	if (lastMessage[userIndex].length() > 0)
		std::cout << "[e] - export\n";
	if (profile[userIndex].length() > 0){
		std::cout << "[g] - generate goal\n";
		std::cout << "[p] - see user profile\n";
		std::cout << "[j] - create a journey\n";
	}
	if (goal[userIndex] != nullptr){
		std::cout << "[s] - split goal\n";
	}

}

void Menu::drawSplitGoalTool() {
	system("clear");
	std::cout << "[-1] - quit\n";

	std::cout << "\n Current Goal : " << goal[userIndex]->name << "\n\n";

	for (int i = 0; i < goal[userIndex]->childrensize; i++) {
		std::cout << "[" << i << "] : " << goal[userIndex]->children[i]->name << "\n";
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
	std::cout << "\nEnter : ";
	getline(std::cin, inputString[userIndex]);
}

void Menu::readInt() {
	std::cout << "\nWrite [0-10]: ";
	std::cin >> inputInt[userIndex];
}

void Menu::start()
{
	initializeBrain();

	drawStartSession();

	while (true) {
		if (state == QUIT) break;
		update();
	}

	drawEndSession();
}

void Menu::enterSplitGoalTool()
{
	state = SPLITGOAL;

	if (goal[userIndex]->childrensize == 0)
		SPLIT_GOAL(*goal[userIndex], *ai); // split the actual goal

	drawSplitGoalTool();
	readInt();

	if (inputInt[userIndex] == -1){
		system("clear");
		return;
	}

	if (inputInt[userIndex] >= 0 && inputInt[userIndex] < goal[userIndex]->childrensize)
		goal[userIndex] = goal[userIndex]->children[inputInt[userIndex]];

}

void Menu::enterProfileView()
{
	state = PROFILE;

	system("clear");
	std::cout << "USER [" << userIndex << "] PROFILE ----\n\n";
	std::cout << profile[userIndex] << "\n\n";

	std::cout << "Press anything to continue...\n";

}

void Menu::update()
{
	drawMainMenu();
	readString();

	if (inputString[userIndex] == "q"){
		state = QUIT;
		return;
	}

	if (inputString[userIndex] == "c") {
		system("clear");
		return;
	}

	if (inputString[userIndex] == "u") {
		system("clear");
		userIndex = ( userIndex + 1 ) % USERS;
		return;
	}

	if (inputString[userIndex] == "e" && lastMessage[userIndex].length() > 0) {
		profile[userIndex] = ai->run(history[userIndex], AI::EXPORT);
		ai->modifyBrain(profile[userIndex]);
		system("clear");
		return;
	}

	if (inputString[userIndex] == "g" && profile[userIndex].length() > 0) {
		if (goal[userIndex] != nullptr) delete goal[userIndex];
		goal[userIndex] = new Goal(ai->run(profile[userIndex], AI::GENERATE_GOAL));
		parentgoal[userIndex] = goal[userIndex];
		history[userIndex] += " AI GENERATED A GOAL FOR THE USER : " + goal[userIndex]->name + "\n";
		return;
	}

	
	if (inputString[userIndex] == "s" && goal[userIndex] != nullptr) {
		enterSplitGoalTool();
		state = MAINMENU;
		return;
	}

	if (inputString[userIndex] == "p" && profile[userIndex].length() > 0) {
		enterProfileView();
		state = MAINMENU;
		return;
	}

	if (inputString[userIndex].length() <= 1) {
		system("clear"); 
		return;
	};
	
	history[userIndex] += "User said : " + inputString[userIndex] + '\n';
	auto output = ai->run(history[userIndex], AI::CHAT);
	history[userIndex] += "AI said : " + output + "\n";
	
	system("clear");

	lastMessage[userIndex] = output;

	std::cout << std::endl << std::endl;

}

