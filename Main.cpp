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

//Get the handle for the console buffer to be used to get chars at certain positions on the window
const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	//Generate the maze
	Maze maze;
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