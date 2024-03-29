#include "Mouse.h"

void Mouse::MoveTo(const cVector2 toPos) {
	path.push(currentPos);
	currentPos = toPos;
}
cVector2 Mouse::GoBack() {
	currentPos = GetPreviousPos();
	if (!isEmpty()) {
		path.pop();
	}
	return currentPos;
}
void Mouse::MakeMove(Maze &maze) {
	cVector2 currentPos = GetPos();

	//Get the values of chars around the mouse and their position
	cVector2 left = cVector2(currentPos.x - BLOCK, currentPos.y);
	char leftChar = maze.getPositionValue(left.x, left.y);

	cVector2 right = cVector2(currentPos.x + BLOCK, currentPos.y);
	char rightChar = maze.getPositionValue(right.x, right.y);

	cVector2 up = cVector2(currentPos.x, currentPos.y - BLOCK);
	char upChar = maze.getPositionValue(up.x, up.y);

	cVector2 down = cVector2(currentPos.x, currentPos.y + BLOCK);
	char downChar = maze.getPositionValue(down.x, down.y);


	maze.setCurrentPathChar(maze.GetVisitedChar());
	//Check Left
	if (CheckPos(maze, leftChar)) {
		DeadEnd(false);
		MoveInMaze(maze, left);
	}
	//Check Right
	else if (CheckPos(maze, rightChar)) {
		DeadEnd(false);
		MoveInMaze(maze, right);
	}
	//Check Up
	else if (CheckPos(maze, upChar)) {
		DeadEnd(false);
		MoveInMaze(maze, up);
	}
	//Check Down
	else if (CheckPos(maze, downChar)) {
		DeadEnd(false);
		MoveInMaze(maze, down);
	}
	//Go Back
	else {
		if (!InDeadEnd()) {
			AddDeadEnd();
		}
		DeadEnd(true);
		//Since there are no new tiles to visit, go back to the mouse's last position
		maze.setCurrentPathChar(maze.GetIncorrectRouteChar());
		maze.setPositionValue(currentPos.x, currentPos.y);
		GoBack();
		currentPos = GetPos();
		//NOTE: MoveTo and DisplayMouse are not used here because the stack should not store the last position of the mouse when backtracking
		maze.setPositionValue(currentPos.x, currentPos.y, mouseChar);
	}
}
void Mouse::MoveInMaze(Maze &maze, cVector2 destination) {
	//Set the char of the old position to the visited or incorrect route character
	maze.setPositionValue(currentPos.x, currentPos.y);
	//Add mouse's current position to the stack then change it to the new position
	MoveTo(destination);
	//Set the char of the new position to the mouse
	maze.DisplayMouse(currentPos, mouseChar);
}
bool Mouse::CheckPos(Maze &maze, char posChar) {
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