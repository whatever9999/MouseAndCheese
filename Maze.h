#pragma once

#include <conio.h>
#include <Windows.h>
#include <iostream>

#include "cMaze.h"

using namespace std;

//For consistency with the skeleton program all constants make use of capitilisation and underscores whereas the rest of the program uses camelCase
const int BLOCK = 1;

//Derived from cMaze, Maze implements interaction with the console window
class Maze : public cMaze {
private:
	//Note that 'Char' is added to the end of each of these chars due to the skeleton code making use of this in the cMaze class (m_floorChar, m_wallChar, m_startChar, m_exitChar and m_errorChar) - For consistency
	char visitedChar = 'O';
	char incorrectRouteChar = 'X';
	//currentPathChar changes according to if the mouse has visited the position already or not
	//This change is carried out so a clear appropriate path between the start and the end of the maze can be denoted by one char with dead end paths denoted using another
	char currentPathChar = visitedChar;
	HANDLE hOutput;

public:
	void setCurrentPathChar(char newChar) {currentPathChar = newChar;}
	//Change the maze's data and appearance to the char value
	void setHandle(HANDLE hConsoleOutput) {hOutput = hConsoleOutput;}
	void SetIncorrectRouteChar(char newIncorrectRouteChar) { incorrectRouteChar = newIncorrectRouteChar; }
	void SetVisitedChar(char newVisistedChar) { visitedChar = newVisistedChar; }
	char GetIncorrectRouteChar() { return incorrectRouteChar; } 
	char GetVisitedChar() { return visitedChar; }

	char setPositionValue(int col, int row, char value);
	//Change the maze's data and appearance to the currentPathChar value
	char setPositionValue(int col, int row);
	//Collect the value at col, row from the strings that store the maze's data
	char getPositionValue(int col, int row);
	//Make the mouse visible in the maze
	void DisplayMouse(cVector2 position, char mouseChar);
	//Since the maze handles the console it holds the ability to return the position of the cursor on the console
	COORD GetConsoleCursorPosition();
};