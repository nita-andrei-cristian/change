#include "ai.hpp"
#include "goal.hpp"
#include <string>
#pragma one

#define USERS 2

namespace INTERFACE {

	enum MenuState {MAINMENU, SPLITGOAL, QUIT, PROFILE};

	class Menu{
		private:
			AI::AI *ai;

			int userIndex;
				
			Goal *goal[USERS];
			Goal *parentgoal[USERS];
			std::string profile[USERS];
			std::string history[USERS];
			std::string lastMessage[USERS];

			std::string inputString[USERS];
			int inputInt[USERS];

			MenuState state;

			void drawStartSession();
			void drawEndSession();

		public:
	
			Menu();
			~Menu();

			void update();
			void drawMainMenu();
			
			void drawSplitGoalTool();
			void enterSplitGoalTool();
			void enterProfileView();
			void enterJourneyView();

			void readString();
			void readInt();
			void start();

	};
};

