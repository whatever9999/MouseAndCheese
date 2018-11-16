#include <iostream>
#include <conio.h>
#include <assert.h>
#include <vector>
#include <string>
#include "cMaze.h"
#include "cTimer.h"
#include "cVector2.h"
#include <windows.h>

const int ESC_KEY = 27;

class Mouse {
	private:
		cVector2 previousPos = cVector2(0, 0);
		cVector2 pos = cVector2(0, 0);
		bool hasFoundCheese = false;
		char mouseChar = 'M';

	public:
		cVector2 GetPreviousPos() {
			return previousPos;
		}
		cVector2 GetPos() { 
			return pos; 
		}
		bool FoundCheese() { 
			return hasFoundCheese; 
		}
		char GetMouseChar() {
			return mouseChar;
		}
		void SetPos(cVector2 newPos) {
			previousPos = pos;
			pos = cVector2(newPos);
		}

		//Move the mouse to a certain position in the maze
		bool MoveTo(const cVector2& toPos) {

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
			char leftChar = getPositionValue(left.y, left.x);
			cVector2 right = cVector2(currentPos.x + oneBlock, currentPos.y);
			char rightChar = getPositionValue(left.y, left.x);
			cVector2 up = cVector2(currentPos.x, currentPos.y - oneBlock);
			char upChar = getPositionValue(left.y, left.x);
			cVector2 down = cVector2(currentPos.x, currentPos.y + oneBlock);
			char downChar = getPositionValue(left.y, left.x);
			cVector2 previous = mouse.GetPreviousPos();

			//Check Left
			if (checkPos(leftChar)) {
				mouse.SetPos(left);
				setPositionValue(left.y, left.x, mouse.GetMouseChar());
			} 
			//Check Right
			else if (checkPos(rightChar)) {
				mouse.SetPos(right);
				setPositionValue(right.y, right.x, mouse.GetMouseChar());
			}
			//Check Up
			else if (checkPos(upChar)) {
				mouse.SetPos(up);
				setPositionValue(up.y, up.x, mouse.GetMouseChar());
			}
			//Check Down
			else if (checkPos(downChar)) {
				mouse.SetPos(down);
				setPositionValue(down.y, down.x, mouse.GetMouseChar());
			}
			//Go Back
			else {
				mouse.SetPos(previous);
				setPositionValue(previous.y, previous.x, mouse.GetMouseChar());
			}
		}

		bool checkPos(char posChar) {
			if (posChar != 'V' && posChar != getWallChar()) {
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
	mouse.SetPos(startPos);

	do {
		// start keeping time
		timer.start();

		// make the mouse move
		maze.MakeMove(mouse);
		maze.DisplayMouse(mouse);
		maze.print();

		// stop keeping time
		timer.stop();

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
	std::cout << std::endl << std::endl << "Press a key to exit" << elapsed;
	std::cin.ignore();
	_getch();
}


