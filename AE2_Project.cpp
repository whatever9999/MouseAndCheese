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

using namespace std;

const int ESC_KEY = 27;

//Note that 'Char' is added to the end of each of these chars due to the skeleton code making use of this in the cMaze class (m_floorChar, m_wallChar, m_startChar, m_exitChar and m_errorChar) - For consistency
//Also for consistency with the skeleton program all constants make use of capitilisation and underscores whereas the rest of the program uses camelCase
const char MOUSE_CHAR = 'M';
const char INCORRECT_ROUTE_CHAR = 'X'; // Note that X and O were chosen for incorrectRouteChar and visitedChar because they are easy to differentiate from each other in the console window
const char VISITED_CHAR = 'O';
const int ONE_BLOCK = 1;

//Get the handle for the console buffer to be used to get chars at certain positions on the window
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

//Get the current position of the cursor in the console
COORD GetConsoleCursorPosition(HANDLE hConsoleOutput) {
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		// The function failed. Call GetLastError() for details.
		COORD invalid = {0, 0};
		return invalid;
	}
}

//Derived from cMaze, Maze implements interaction with the console window
class Maze : public cMaze {
	private:
		//currentPathChar changes according to if the mouse has visited the position already or not
		//This change is carried out so a clear appropriate path between the start and the end of the maze can be denoted by one char with dead end paths denoted using another
		char currentPathChar = VISITED_CHAR;

	public:
		void setCurrentPathChar(char newChar) {
			currentPathChar = newChar;
		}
		char setPositionValue(int col, int row, char value) {
			//Change the string that stores the maze data to be used to determine the mouse's direction
			m_strings[row][col] = value;

			//Move the cursor to the coordinates of pos and write value to that location
			COORD pos{ (short)col, (short)row };
			SetConsoleCursorPosition(hOutput, pos);
			cout << value;
			return '0';
		}
		char setPositionValue(int col, int row) {
			//Change the string that stores the maze data to be used to determine the mouse's direction
			m_strings[row][col] = currentPathChar;

			//Move the cursor to the coordinates of pos and write value to that location
			COORD pos{ (short)col, (short)row };
			SetConsoleCursorPosition(hOutput, pos);
			cout << currentPathChar;
			return '0';
		}
		char getPositionValue(int col, int row) {
			//Collect the value at col, row from the strings that store the maze's data
			int value = m_strings[row][col];
			return value;
		}

		//Make the mouse visible in the maze
		void DisplayMouse(cVector2 position) {
			setPositionValue(position.x, position.y, MOUSE_CHAR);
		}
};

//Mouse class carries out interaction with a stack
//and carries out the mouse movement algorithm by getting the values of the positions to the left, right, top and down of the the mouse or backtracking if there is no new path to take
class Mouse {
	private:
		//Stack that stores the path the mouse has taken to be used when the mouse reaches a dead end to back track
		stack<cVector2> path;
		bool hasFoundCheese = false;
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
		bool isEmpty() {
			return path.empty();
		}
		void HasFoundCheese() {
			hasFoundCheese = true;
		}

		//Put a current position onto the path stack so current position can hold where the mouse currently is
		bool MoveTo(const cVector2 toPos) {
			path.push(currentPos);
			currentPos = toPos;
			return true;
		}
		//Set the current position of the mouse to the position on top of the path stack and remove it from the stack
		cVector2 GoBack() {
			currentPos = GetPreviousPos();
			if(!isEmpty()) {
				path.pop();
			}
			return currentPos;
		}

		//Mouse movement algorithm
		void MakeMove(Maze &maze) {
			cVector2 currentPos = GetPos();

			//Get the values of chars around the mouse and their position
			cVector2 left = cVector2(currentPos.x - ONE_BLOCK, currentPos.y);
			char leftChar = maze.getPositionValue(left.x, left.y);

			cVector2 right = cVector2(currentPos.x + ONE_BLOCK, currentPos.y);
			char rightChar = maze.getPositionValue(right.x, right.y);

			cVector2 up = cVector2(currentPos.x, currentPos.y - ONE_BLOCK);
			char upChar = maze.getPositionValue(up.x, up.y);

			cVector2 down = cVector2(currentPos.x, currentPos.y + ONE_BLOCK);
			char downChar = maze.getPositionValue(down.x, down.y);

			//Get the last position of the mouse if if is not empty
			if (isEmpty()) {
				cVector2 previous = GetPreviousPos();
			}


			maze.setCurrentPathChar(VISITED_CHAR);
			//Check Left
			if (CheckPos(maze, leftChar)) {
				moveToDestination(maze, left);
			}
			//Check Right
			else if (CheckPos(maze, rightChar)) {
				moveToDestination(maze, right);
			}
			//Check Up
			else if (CheckPos(maze, upChar)) {
				moveToDestination(maze, up);
			}
			//Check Down
			else if (CheckPos(maze, downChar)) {
				moveToDestination(maze, down);
			}
			//Go Back
			else {
				//Since there are no new tiles to visit, go back to the mouse's last position
				maze.setCurrentPathChar(INCORRECT_ROUTE_CHAR);
				maze.setPositionValue(currentPos.x, currentPos.y);
				GoBack();
				currentPos = GetPos();
				//NOTE: MoveTo and DisplayMouse are not used here because the stack should not store the last position of the mouse when backtracking
				maze.setPositionValue(currentPos.x, currentPos.y, MOUSE_CHAR);
			}
		}

		void moveToDestination(Maze &maze, cVector2 destination) {
			//Set the char of the old position to the visited or incorrect route character
			maze.setPositionValue(currentPos.x, currentPos.y);
			//Add mouse's current position to the stack then change it to the new position
			MoveTo(destination);
			//Set the char of the new position to the mouse
			maze.DisplayMouse(currentPos);
		}

		bool CheckPos(Maze &maze, char posChar) {
			//See if the char being checked can be walked on or if it is the cheese
			if (posChar == maze.getFloorChar()) {
				return true;
			}
			else if (posChar == maze.getExitChar()) {
				HasFoundCheese();
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
	string limit = "Limit =  " + to_string(maze.getTimeLimit_ms()) + " ms";
	cout << endl << endl << limit << endl;

	//Get user input to start simulation
	string request = "Press a key to start the mouse ...";
	cout << endl << request;
	_getch();

	//Get rid of input request
	int requestLength = request.length();
	string empty = string(requestLength, ' ');
	cout << "\r" + empty + "\r";

	//Get the location of the cursor on the screen once everything (maze, time limit, time elapsed) has ben drawn in order to rewrite elapsed time
	COORD previousCursorPos = GetConsoleCursorPosition(hOutput);

	//Create a timer to keep track of elapsed time
	cTimer timer;
	double lastTime = timer.getElapsed_ms();

	//Create the "mouse"
	Mouse mouse;

	//Put the mouse at the start of the maze and make it visible
	cVector2 startPos;
	maze.getStart(startPos);
	mouse.MoveTo(startPos);
	maze.DisplayMouse(mouse.GetPos());

	Sleep(200);

	do {
		//Start keeping time
		timer.start();

		//Commence algorithm to determine mouse movement
		mouse.MakeMove(maze);

		//Stop keeping time
		timer.stop();

		//Display timing updates
		SetConsoleCursorPosition(hOutput, previousCursorPos);
		string elapsed = "Elapsed =  " + to_string(timer.getElapsed_ms() - lastTime) + " ms";
		cout << elapsed;

		//Get the current time on the timer to be used to calculate the elapsed time in the next loop cycle
		lastTime = timer.getElapsed_ms();

		Sleep(200);

		//Get the length of the strings used to show timing updates and erase before next update
		int elapsedLength = elapsed.length();
		empty = "\r" + string(elapsedLength, ' ');
		cout << empty;

		//Quit if Escape is pressed.
		if (_kbhit()) {
			if (ESC_KEY == _getch())
				return 0;
		}

	} while (!mouse.FoundCheese());

	//Display the total elapsed time
	double elapsed = timer.getElapsed_ms();
	SetConsoleCursorPosition(hOutput, previousCursorPos);
	cout << "Total Elapsed =  " + to_string(elapsed) + " ms";
	cout << endl << endl << "Press a key to exit";
	_getch();
}


