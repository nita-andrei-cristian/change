#pragma once
#include <string>
#include <ai.hpp>

struct Goal {
	std::string name;

	Goal *children[10];
	Goal *parent;
	char childrensize;

	Goal(std::string s) ;

	~Goal() ;
};

std::string GET_GOAL_PARENT_CHAIN(Goal &goal);

void SPLIT_GOAL(Goal &goal, AI::AI &creator);
