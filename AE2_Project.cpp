#include <windows.h>		// for sleep()
#include <iostream>
#include <conio.h>
#include <assert.h>
#include <vector>
#include <string>
#include "cMaze.h"
#include "cTimer.h"
#include "cVector2.h"

const int ESC_KEY = 27;


class Mouse
{
public:
	bool			MoveTo(const cVector2& toPos);
	cVector2		GetPos() { return pos; }
	bool			FoundCheese() { return hasFoundCheese; }
private:
	cVector2		pos{ 0, 0 };
	bool			hasFoundCheese{ false };
};

int main()
{
	// create a maze variable and create a new maze
	cMaze	maze;
	maze.createRandomSize();

	// print the maze
	maze.print();

	// Display timing updates
	std::cout << std::endl << std::endl << "Limit =  " << maze.getTimeLimit_ms() << " ms" << std::endl;
	std::cout << std::endl << "Press a key to start the mouse ...";
	_getch();

	// create a timer to keep track of elapsed time
	cTimer	timer;

	// create the "mouse"
	Mouse	mouse;

	// ensure the maze was created successfully with a start point
	cVector2 startPos;
	maze.getStart(startPos);

		do
		{
			// start keeping time
			timer.start();

			// make the mouse move - REPLACE WITH YOUR CODE

			// stop keeping time
			timer.stop();

			// Display timing updates
			std::cout << std::endl << std::endl << "Limit =  " << maze.getTimeLimit_ms() << " ms" << std::endl;
			std::cout << "Elapsed =  " << timer.getElapsed_ms() << " ms";

			// short delay between moves to make them visible
			Sleep(200);

			// Quit if Escape is pressed.
			if (_kbhit())
			{
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


