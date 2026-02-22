#include <goal.hpp>
#include <utils.hpp>
#include <string>
#include <sstream>
#include <algorithm>

inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
				}));
}

Goal::Goal(std::string s) {
	name = s;
	parent = nullptr;

	childrensize = 0;
}

Goal::~Goal() {
	for (int i = 0; i < childrensize; i++)
		delete children[i];
}

std::string GET_GOAL_PARENT_CHAIN(Goal &goal) {
	std::string output = "";

	Goal *parent = goal.parent;
	while (parent != nullptr) {
		output = parent->name + "->" + output;
		parent = parent->parent;
		if (parent->parent == parent)
			break;
	}

	return output;
}

void SPLIT_GOAL(Goal &goal, AI::AI &creator) {
	std::string response = creator.run(GET_GOAL_PARENT_CHAIN(goal) +
			" | you will split : How to " + goal.name, AI::SPLIT_GOAL);
	std::istringstream stream(response);

	std::string line;

	int index = 0;
	while (std::getline(stream, line)) {
		line.erase(0, 2); // removes the " -"
		ltrim(line);

		if (line.empty())
			continue;

		Goal *child = new Goal(line);
		if (&goal != child)
			child->parent = &goal;
		goal.children[index++] = child;
	}

	goal.childrensize = index;
}
