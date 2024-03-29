#include <iostream>
#include <conio.h>
#include <assert.h>
#include <Windows.h>
#include <string>

#include "Maze.h"
#include "Mouse.h"
#include "cTimer.h"
#include "cVector2.h"

using namespace std;

const int ESC_KEY = 27;
const int minSpeed = 40;
const int maxSpeed = 200;

//Get the handle for the console buffer to be used to get chars at certain positions on the window
const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

void clearScreen() {
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	GetConsoleScreenBufferInfo(hOutput, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(hOutput, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(hOutput, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(hOutput, tl);
}

char getMouseChar() {
	char mouseChar = 'M';
	bool gotChar = false;
	cout << "Enter the character you would like to represent the mouse (E.g. M): ";

	//Ensure that the character input isn't a floor or a wall character
	do {
		cin >> mouseChar;

		if (mouseChar != ' ' && mouseChar != (char)178) {
			gotChar = true;
		}
	} while (!gotChar);
	return mouseChar;
}

char getVisitedChar(char mouseChar) {
	char visitedChar = 'O';
	bool gotChar = false;
	cout << "Enter the character you would like to represent the trail behind the mouse (E.g. O): ";
	do {
		cin >> visitedChar;

		if (visitedChar != ' ' && visitedChar != (char)178 && visitedChar != mouseChar) {
			gotChar = true;
		}
	} while (!gotChar);
	return visitedChar;
}

char getIncorrectRouteChar(char mouseChar, char visitedChar) {
	char incorrectRouteChar = 'O';
	bool gotChar = false;
	cout << "Enter the character you would like to represent the trails that lead to dead ends (E.g. X): ";
	do {
		cin >> incorrectRouteChar;

		if (incorrectRouteChar != ' ' && incorrectRouteChar != (char)178 && incorrectRouteChar != mouseChar && incorrectRouteChar != visitedChar) {
			gotChar = true;
		}
	} while (!gotChar);
	return incorrectRouteChar;
}

int getSpeed() {
	int speed = maxSpeed;
	bool gotSpeed = false;
	cout << "Enter how long you want a single mouse move to take in ms (Between " << minSpeed << " and " << maxSpeed << " inclusive): ";
	do {
		cin >> speed;

		if (speed <= maxSpeed && speed >= minSpeed) {
			gotSpeed = true;
		}
	} while (!gotSpeed);
	return speed;
}

int main() {
	char mouseChar = getMouseChar();
	char visitedChar = getVisitedChar(mouseChar);
	char incorrectRouteChar = getIncorrectRouteChar(mouseChar, visitedChar);
	int speed = getSpeed();
	clearScreen();

	//Generate the maze
	Maze maze;
	maze.SetIncorrectRouteChar(incorrectRouteChar);
	maze.SetVisitedChar(visitedChar);
	maze.createRandomSize();
	maze.setHandle(hOutput);

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
	COORD previousCursorPos = maze.GetConsoleCursorPosition();

	//Create a timer to keep track of elapsed time
	cTimer timer;
	double lastTime = timer.getElapsed_ms();

	//Create the "mouse"
	Mouse mouse;
	mouse.SetMouseChar(mouseChar);

	//Put the mouse at the start of the maze and make it visible
	cVector2 startPos;
	maze.getStart(startPos);
	mouse.MoveTo(startPos);
	maze.DisplayMouse(mouse.GetPos(), mouse.GetMouseChar());

	Sleep(speed);

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

		Sleep(speed);

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
	cout << "Total Elapsed (Mouse Thinking Only) =  " + to_string(elapsed) + " ms";
	cout << endl << "Number of Dead Ends =  " + to_string(mouse.GetDeadEnds());
	cout << endl << endl << "Press a key to exit";
	_getch();
}