#include "Well.h"
#include "UserInterface.h"

// Creating a 12x19 vector of vectors
Well::Well(int width, int height) : m_well(width + 2, vector<char>(height + 1, ' ')), 
									m_width(width), m_height(height) {
	// Setting the boundary of the well
	for (int y = 0; y < height + 1; y++) {
		m_well[0][y] = '@';
		m_well[width + 1][y] = '@';
	}
	for (int x = 0; x < width + 2; x++) {
		m_well[x][height] = '@';
	}
}

void Well::display(Screen& screen, int x, int y)
{
	// Looping through each cell of the well and printing out what's there
	for (int i = 0; i < m_height + 1; i++) {
		for (int j = 0; j < m_width + 2; j++) {
			screen.gotoXY(x + j, y + i);
			screen.printChar(m_well[j][i]);
		}
	}
}

// Changes what's inside the cell at the 
// given position to the given character
void Well::update(int x, int y, char c) {
	m_well[x][y] = c;
}

// Return the character that's inside the cell at the given position
char Well::getChar(int x, int y) const {
	return m_well[x][y];
}

bool Well::willBeVaporized(int row) const {
	int count = 0;
	// Checking each cell in the row to see if it's filled.
	// If it's not empty, add 1 to count.
	for (int i = 1; i <= m_width; i++) {
		char c = m_well[i][row];
		if (c != ' ') {
			count += 1;
		}
	}
	// If every cell in the row is filled, then the row will be vaporized
	if (count == m_width) {
		return true;
	}
	return false;
}

void Well::vaporize(int row) {
	// Shifting all the characters in the well down by one row
	// The bottom row just gets replaced by the characters in the row above it.
	for (int j = row; j > 0; j--) {
		for (int i = 1; i <= m_width; i++) {
			char c = m_well[i][j - 1];
			m_well[i][j] = c;
		}
	}
}

void Well::clearWell() {
	// Changing every cell in the well to be the empty character
	for (int i = 1; i < m_width + 1; i++) {
		for (int j = 0; j < m_height; j++) {
			m_well[i][j] = ' ';
		}
	}
}