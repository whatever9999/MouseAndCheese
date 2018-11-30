#pragma once

#include <stack>
#include <vector>

#include "cVector2.h"
#include "Maze.h"

using namespace std;

//Mouse class carries out interaction with a stack
//and carries out the mouse movement algorithm by getting the values of the positions to the left, right, top and down of the the mouse or backtracking if there is no new path to take
class Mouse {
private:
	//Stack that stores the path the mouse has taken to be used when the mouse reaches a dead end to back track
	stack<cVector2> path;
	bool hasFoundCheese = false;
	cVector2 currentPos = cVector2(0, 0);

public:
	cVector2 GetPreviousPos() {return path.top();}
	cVector2 GetPos() {return currentPos;}
	bool FoundCheese() {return hasFoundCheese;}
	bool isEmpty() {return path.empty();}
	void HasFoundCheese() {hasFoundCheese = true;}

	//Put a current position onto the path stack so current position can hold where the mouse currently is
	void MoveTo(const cVector2 toPos);
	//Set the current position of the mouse to the position on top of the path stack and remove it from the stack
	cVector2 GoBack();
	//Mouse movement algorithm
	void MakeMove(Maze &maze);
	//Interact with the maze and the stack to update the mouse's position
	void MoveInMaze(Maze &maze, cVector2 destination);
	//See if posChar is a path or the cheese (accessible for the mouse)
	bool CheckPos(Maze &maze, char posChar);
};
