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

HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coordinate;
COORD previousCursorPos;

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

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
		char cheeseChar = 'E';
		char pathChar = 'V';

	public:
		char setPositionValue(int col, int row, char value) {
			//Change the value of the char at col, row to value
			m_strings[row][col] = value;
			coordinate.X = col, coordinate.Y = row;
			SetConsoleCursorPosition(hOutput, coordinate);
			std::cout << value;
			SetConsoleCursorPosition(hOutput, previousCursorPos);
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


			pathChar = 'V';
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
				//Since the mouse is going back along a new route increase the path's ID number
				pathChar = 'D';
				//Since there are no new tiles to visit, go back through the mouse's path
				setPositionValue(currentPos.x, currentPos.y, pathChar);
				mouse.GoBack();
				currentPos = mouse.GetPos();
				setPositionValue(currentPos.x, currentPos.y, mouse.GetMouseChar());
			}

			
		}

		void moveToDestination(Mouse &mouse, cVector2 destination, cVector2 currentPos) {
			mouse.MoveTo(destination);
			setPositionValue(currentPos.x, currentPos.y, pathChar);
			setPositionValue(destination.x, destination.y, mouse.GetMouseChar());
		}

		void moveToCheese(Mouse &mouse, cVector2 destination, cVector2 currentPos) {
			if (mouse.FoundCheese()) {
				mouse.MoveTo(destination);
				setPositionValue(currentPos.x, currentPos.y, pathChar);
				setPositionValue(destination.x, destination.y, mouse.GetMouseChar());
			}
		}

		bool CheckPos(char posChar, Mouse &mouse) {
			//Check to see if the current char has been visited and if it can be walked on
			if (posChar == getFloorChar()) {
				return true;
			}
			else if (posChar == cheeseChar) {
				mouse.HasFoundCheese();
				return true;
			}
			else {
				return false;
			}
			
		}
};

int main() {
	//Generate the maze
	Maze maze;
	maze.createRandomSize();

	//Print the maze
	maze.print();

	//Display time limit
	std::string limit = "Limit =  " + std::to_string(maze.getTimeLimit_ms()) + " ms";
	std::cout << std::endl << std::endl << limit << std::endl;

	//Get user input to start simulation
	std::string request = "Press a key to start the mouse ...";
	std::cout << std::endl << request;
	_getch();

	//Get rid of input request
	int requestLength = request.length();
	std::string empty = std::string(requestLength, ' ');
	std::cout << "\r" + empty + "\r";

	//Get the position of the cursor after the time limit output and the input request erased
	previousCursorPos = GetConsoleCursorPosition(hOutput);

	//Create a timer to keep track of elapsed time
	cTimer timer;
	double lastTime = timer.getElapsed_ms();

	//Create the "mouse"
	Mouse mouse;

	//Put the mouse at the start of the maze and make it visible
	cVector2 startPos;
	maze.getStart(startPos);
	mouse.MoveTo(startPos);
	maze.DisplayMouse(mouse);

	Sleep(200);

	do {
		//Start keeping time
		timer.start();

		//Commence algorithm to determine mouse movement
		maze.MakeMove(mouse);

		//Stop keeping time
		timer.stop();

		//Display timing updates
		std::string elapsed = "Elapsed =  " + std::to_string(timer.getElapsed_ms() - lastTime) + " ms";
		std::cout << elapsed;

		//Get the current time on the timer to be used to calculate the elapsed time in the next loop cycle
		lastTime = timer.getElapsed_ms();

		Sleep(200);

		//Get the length of the strings used to show timing updates and erase before next update
		int elapsedLength = elapsed.length();
		empty = std::string(elapsedLength, ' ');
		std::cout << empty;

		//Quit if Escape is pressed.
		if (_kbhit()) {
			if (ESC_KEY == _getch())
				return 0;
		}

	} while (!mouse.FoundCheese());

	//Display the total elapsed time
	double elapsed = timer.getElapsed_ms();
	std::string total = "Total Elapsed =  " + std::to_string(elapsed) + " ms";
	std::cout << std::endl << std::endl << total;

	std::cout << std::endl << std::endl << "Press a key to exit";
	_getch();
}


