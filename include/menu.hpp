#include "ai.hpp"
#include "goal.hpp"
#include <string>
#pragma one

namespace INTERFACE {

	enum MenuState {MAINMENU, SPLITGOAL, QUIT};

	class Menu{
		private:
			AI::AI *ai;
				
			Goal *goal;
			Goal *parentgoal;
			std::string profile;
			std::string history;
			bool sentOneMessage;

			std::string inputString;
			int inputInt;

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
			void readString();
			void readInt();
			void start();

	};
};

