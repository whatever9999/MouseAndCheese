#include <iostream>
#include <conio.h>
#include <assert.h>
#include <vector>
#include <string>
#include "cMaze.h"
#include "cTimer.h"
#include "cVector2.h"
#include <windows.h>
#include <stack>

const int ESC_KEY = 27;

class Mouse {
	private:
		bool hasFoundCheese = false;
		const char mouseChar = 'M';
		std::stack<cVector2> path;
		cVector2 currentPos = cVector2(0, 0);

	public:
		cVector2 GetPreviousPos() {
			return path.top();
		}
		cVector2 GetPos() { 
			return currentPos; 
		}
		bool FoundCheese() { 
			return hasFoundCheese; 
		}
		char GetMouseChar() {
			return mouseChar;
		}
		bool isEmpty() {
			return path.empty();
		}
		void HasFoundCheese() {
			hasFoundCheese = true;
		}

		//Move the mouse to a certain position in the maze
		bool MoveTo(const cVector2& toPos) {
			path.push(currentPos);
			currentPos = toPos;
			return true;
		}
		cVector2 GoBack() {
			currentPos = GetPreviousPos();
			if(!isEmpty()) {
				path.pop();
			}
			return currentPos;
		}
};

class Maze : public cMaze {
	private:
		char visitedChar = 'V';

	public:
		char setPositionValue(int col, int row, char value) {
			//Change the value of the char at col, row to value
			m_strings[row][col] = value;
			return '0';
		}
		char getPositionValue(int col, int row) {
			//Collect the value at col, row
			int value = m_strings[row][col];
			return value;
		}

		//Make the mouse visible in the maze
		void DisplayMouse(Mouse &mouse) {
			cVector2 mousePos = mouse.GetPos();
			setPositionValue(mousePos.x, mousePos.y, mouse.GetMouseChar());
		}

		//Work out what the next best move for the mouse is
		void MakeMove(Mouse &mouse) {
			int oneBlock = 1;
			cVector2 currentPos = mouse.GetPos();

			cVector2 left = cVector2(currentPos.x - oneBlock, currentPos.y);
			char leftChar = getPositionValue(left.x, left.y);
			cVector2 right = cVector2(currentPos.x + oneBlock, currentPos.y);
			char rightChar = getPositionValue(right.x, right.y);
			cVector2 up = cVector2(currentPos.x, currentPos.y - oneBlock);
			char upChar = getPositionValue(up.x, up.y);
			cVector2 down = cVector2(currentPos.x, currentPos.y + oneBlock);
			char downChar = getPositionValue(down.x, down.y);
			if (!mouse.isEmpty()) {
				cVector2 previous = mouse.GetPreviousPos();
			}

			//Check Left
			if (CheckPos(leftChar, mouse)) {
				moveToDestination(mouse, left, currentPos);
			} 
			//Check Right
			else if (CheckPos(rightChar, mouse)) {
				moveToDestination(mouse, right, currentPos);
			}
			//Check Up
			else if (CheckPos(upChar, mouse)) {
				moveToDestination(mouse, up, currentPos);
			}
			//Check Down
			else if (CheckPos(downChar, mouse)) {
				moveToDestination(mouse, down, currentPos);
			}
			//Go Back
			else {
				//Since there are no new tiles to visit, go back through the mouse's path
				setPositionValue(currentPos.x, currentPos.y, visitedChar);
				mouse.GoBack();
				currentPos = mouse.GetPos();
				setPositionValue(currentPos.x, currentPos.y, mouse.GetMouseChar());
			}

			
		}

		void moveToDestination(Mouse &mouse, cVector2 destination, cVector2 currentPos) {
			mouse.MoveTo(destination);
			setPositionValue(currentPos.x, currentPos.y, visitedChar);
			setPositionValue(destination.x, destination.y, mouse.GetMouseChar());
		}

		void moveToCheese(Mouse &mouse, cVector2 destination, cVector2 currentPos) {
			if (mouse.FoundCheese()) {
				mouse.MoveTo(destination);
				setPositionValue(currentPos.x, currentPos.y, visitedChar);
				setPositionValue(destination.x, destination.y, mouse.GetMouseChar());
			}
		}

		bool CheckPos(char posChar, Mouse &mouse) {
			//Check to see if the current char has been visited and if it can be walked on
			if (posChar != 'V' && posChar == getFloorChar()) {
				return true;
			}
			else if (posChar == 'E') {
				mouse.HasFoundCheese();
				return true;
			}
			else {
				return false;
			}
			
		}
};

int main() {
	// create a maze variable and create a new maze
	Maze maze;
	maze.createRandomSize();

	// print the maze
	maze.print();

	// Display timing updates
	std::cout << std::endl << std::endl << "Limit =  " << maze.getTimeLimit_ms() << " ms" << std::endl;
	std::cout << std::endl << "Press a key to start the mouse ...";
	_getch();

	// create a timer to keep track of elapsed time
	cTimer timer;

	// create the "mouse"
	Mouse mouse;

	// ensure the maze was created successfully with a start point
	cVector2 startPos;
	maze.getStart(startPos);
	mouse.MoveTo(startPos);

	maze.DisplayMouse(mouse);
	maze.print();
	Sleep(200);

	do {
		// start keeping time
		timer.start();

		// make the mouse move
		maze.MakeMove(mouse);

		// stop keeping time
		timer.stop();

		maze.DisplayMouse(mouse);
		maze.print();

		// Display timing updates
		std::cout << std::endl << std::endl << "Limit =  " << maze.getTimeLimit_ms() << " ms" << std::endl;
		std::cout << "Elapsed =  " << timer.getElapsed_ms() << " ms";

		// short delay between moves to make them visible
		Sleep(200);

		// Quit if Escape is pressed.
		if (_kbhit()) {
			if (ESC_KEY == _getch())
				return 0;
		}

	} while (!mouse.FoundCheese());

	double elapsed = timer.getElapsed_ms();
	std::cout << std::endl << "Total Elapsed =  " << elapsed;
	std::cout << std::endl << std::endl << "Press a key to exit";
	_getch();
}


