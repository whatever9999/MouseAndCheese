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
		MoveInMaze(maze, left);
	}
	//Check Right
	else if (CheckPos(maze, rightChar)) {
		MoveInMaze(maze, right);
	}
	//Check Up
	else if (CheckPos(maze, upChar)) {
		MoveInMaze(maze, up);
	}
	//Check Down
	else if (CheckPos(maze, downChar)) {
		MoveInMaze(maze, down);
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
void Mouse::MoveInMaze(Maze &maze, cVector2 destination) {
	//Set the char of the old position to the visited or incorrect route character
	maze.setPositionValue(currentPos.x, currentPos.y);
	//Add mouse's current position to the stack then change it to the new position
	MoveTo(destination);
	//Set the char of the new position to the mouse
	maze.DisplayMouse(currentPos);
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