#include "Maze.h"

char Maze::setPositionValue(int col, int row, char value) {
	//Change the string that stores the maze data to be used to determine the mouse's direction
	m_strings[row][col] = value;

	//Move the cursor to the coordinates of pos and write value to that location
	COORD pos{ (short)col, (short)row };
	SetConsoleCursorPosition(hOutput, pos);
	cout << value;
	return '0';
}
char Maze::setPositionValue(int col, int row) {
	//Change the string that stores the maze data to be used to determine the mouse's direction
	m_strings[row][col] = currentPathChar;

	//Move the cursor to the coordinates of pos and write value to that location
	COORD pos{ (short)col, (short)row };
	SetConsoleCursorPosition(hOutput, pos);
	cout << currentPathChar;
	return '0';
}
char Maze::getPositionValue(int col, int row) {
	//Collect the value at col, row from the strings that store the maze's data
	int value = m_strings[row][col];
	return value;
}
void Maze::DisplayMouse(cVector2 position) {
	setPositionValue(position.x, position.y, MOUSE_CHAR);
}

COORD Maze::GetConsoleCursorPosition() {
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hOutput, &cbsi))
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