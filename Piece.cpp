#include "Piece.h"
#include "UserInterface.h"
#include "Well.h"

Piece::Piece(Well* well, int x, int y) : m_well(well), m_x(x), m_y(y), orientation(0) {
	// Creating the default piece as a 4x4 array of empty characters
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				pieceArray[i][j][k] = ' ';
			}
		}
	}
}

Piece::~Piece() { }

void Piece::display(Screen& screen, int x, int y) {
	// Looping through the 4x4 bounding box and displaying the characters in each cell
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pieceArray[orientation][j][i] == ' ') {
				continue;
			}
			screen.gotoXY(x + j + m_x, y + i + m_y);
			screen.printChar(pieceArray[orientation][j][i]);
		}
	}
}

int Piece::nextOrientation() {
	// Changes the orientation of the piece
	orientation += 1;
	orientation %= 4;
	return orientation;
}

void Piece::moveLeft() {
	// If there's no collision, move the piece left
	if (!movementCollision(-1, 0)) {
		m_x--;
	}
}

void Piece::moveRight() {
	// If there's no collision, move the piece right
	if (!movementCollision(1, 0)) {
		m_x++;
	}
}

void Piece::moveDown() {
	// If there's no collision, move the piece down
	if (!movementCollision(0, 1)) {
		m_y++;
	}
}

void Piece::arrowUp() {
	// If there's no collision, move to the next orientation of the piece
	if (!orientationCollision()) {
		nextOrientation();
	}
}

void Piece::dropDown() {
	int h = 0;
	// While h is less than the well's height
	while (h < m_well->getHeight()) {
		// If there's a collision at row h, set the 
		// y coordinate of the piece to the row above it
		if (movementCollision(0, h)) {
			m_y += h - 1;
			break;
		}
		// Check the next row
		else {
			h++;
		}
	}
}

bool Piece::movementCollision(int x, int y) const {
	// Looping through 4x4 bounding box in search of where the piece is
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char c = pieceArray[orientation][i][j];
			// Check if the piece overlaps with the current state of the well.
			// If there's overlap, return true.
			if (c == '#') {
				vector<vector<char> > currentWell = m_well->getWell();
				if (currentWell[m_x + i + x][m_y + j + y] != ' ') {
					return true;
				}
			}
		}
	}
	return false;
}

bool Piece::orientationCollision() const {
	int tempOrientation = (orientation + 1) % 4;
	// Looping through 4x4 bounding box in search of the piece
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char c = pieceArray[tempOrientation][i][j];
			// Checks if next orientation of piece overlaps with 
			// current state of the well. Returns true if there is an overlap.
			if (c == '#') {
				vector<vector<char> > currentWell = m_well->getWell();
				if (currentWell[m_x + i][m_y + j] != ' ') {
					return true;
				}
			}
		}
	}
	return false;
}

void Piece::convertPieceArray() {
	// Looping through 4x4 bounding box in search of the characters of the piece
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char c = pieceArray[orientation][i][j];
			// If character is found, convert it to '$' at its position in the well
			if (c == '#') {
				m_well->update(m_x + i, m_y + j, '$');
			}
		}
	}
}

bool Piece::wellIsFull() const {
	// Looping through 4x4 bounding box in search of where the piece is
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char c = pieceArray[orientation][i][j];
			// Check if the piece overlaps with the current state of the well.
			// If there's overlap, return true.
			if (c == '#') {
				vector<vector<char> > currentWell = m_well->getWell();
				if (currentWell[m_x + i][m_y + j] != ' ') {
					return true;
				}
			}
		}
	}
	return false;
}

Piece_I::Piece_I(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientations 0 and 2
	for (int j = 0; j < 4; j++) {
		pieceArray[0][j][1] = '#';
		pieceArray[2][j][1] = '#';
	}
	// Creating orientations 1 and 3
	for (int k = 0; k < 4; k++) {
		pieceArray[1][1][k] = '#';
		pieceArray[3][1][k] = '#';
	}
}

Piece_L::Piece_L(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0
	for (int j = 0; j < 3; j++) {
		pieceArray[0][j][1] = '#';
	}
	pieceArray[0][0][2] = '#';
	// Creating orientation 1
	for (int k = 0; k < 3; k++) {
		pieceArray[1][2][k] = '#';
	}
	pieceArray[1][1][0] = '#';
	// Creating orientation 2
	for (int j = 0; j < 3; j++) {
		pieceArray[2][j][2] = '#';
	}
	pieceArray[2][2][1] = '#';
	// Creating orientation 3
	for (int k = 1; k < 4; k++) {
		pieceArray[3][1][k] = '#';
	}
	pieceArray[3][2][3] = '#';
}

Piece_J::Piece_J(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0
	for (int j = 1; j < 4; j++) {
		pieceArray[0][j][1] = '#';
	}
	pieceArray[0][3][2] = '#';
	// Creating orientation 1
	for (int k = 1; k < 4; k++) {
		pieceArray[1][2][k] = '#';
	}
	pieceArray[1][1][3] = '#';
	// Creating orientation 2
	for (int j = 1; j < 4; j++) {
		pieceArray[2][j][2] = '#';
	}
	pieceArray[2][1][1] = '#';
	// Creating orientation 3
	for (int k = 0; k < 3; k++) {
		pieceArray[3][1][k] = '#';
	}
	pieceArray[3][2][0] = '#';
}

Piece_T::Piece_T(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0
	for (int j = 0; j < 3; j++) {
		pieceArray[0][j][1] = '#';
	}
	pieceArray[0][1][0] = '#';
	// Creating orientation 1
	for (int k = 0; k < 3; k++) {
		pieceArray[1][1][k] = '#';
	}
	pieceArray[1][2][1] = '#';
	// Creating orientation 2
	for (int j = 0; j < 3; j++) {
		pieceArray[2][j][1] = '#';
	}
	pieceArray[2][1][2] = '#';
	// Creating orientation 3
	for (int k = 0; k < 3; k++) {
		pieceArray[3][1][k] = '#';
	}
	pieceArray[3][0][1] = '#';
}

Piece_O::Piece_O(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating all 4 orientations 
	for (int j = 0; j < 2; j++) {
		for (int k = 0; k < 2; k++) {
			pieceArray[0][j][k] = '#';
			pieceArray[1][j][k] = '#';
			pieceArray[2][j][k] = '#';
			pieceArray[3][j][k] = '#';
		}
	}
}

Piece_S::Piece_S(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0 and 2
	for (int j = 1; j < 3; j++) {
		pieceArray[0][j][1] = '#';
		pieceArray[2][j][1] = '#';
	}
	for (int j = 0; j < 2; j++) {
		pieceArray[0][j][2] = '#';
		pieceArray[2][j][2] = '#';
	}
	// Creating orientation 1 and 3
	for (int k = 0; k < 2; k++) {
		pieceArray[1][1][k] = '#';
		pieceArray[3][1][k] = '#';
	}
	for (int k = 1; k < 3; k++) {
		pieceArray[1][2][k] = '#';
		pieceArray[3][2][k] = '#';
	}
}

Piece_Z::Piece_Z(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0 and 2
	for (int j = 0; j < 2; j++) {
		pieceArray[0][j][1] = '#';
		pieceArray[2][j][1] = '#';
	}
	for (int j = 1; j < 3; j++) {
		pieceArray[0][j][2] = '#';
		pieceArray[2][j][2] = '#';
	}
	// Creating orientation 1 and 3
	for (int k = 0; k < 2; k++) {
		pieceArray[1][2][k] = '#';
		pieceArray[3][2][k] = '#';
	}
	for (int k = 1; k < 3; k++) {
		pieceArray[1][1][k] = '#';
		pieceArray[3][1][k] = '#';
	}
}

Piece_Vapor::Piece_Vapor(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating all 4 orientations
	for (int j = 1; j < 3; j++) {
		pieceArray[0][j][0] = '#';
		pieceArray[1][j][0] = '#';
		pieceArray[2][j][0] = '#';
		pieceArray[3][j][0] = '#';
	}
}

Piece_Foam::Piece_Foam(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating all 4 orientations
	pieceArray[0][1][1] = '#';
	pieceArray[1][1][1] = '#';
	pieceArray[2][1][1] = '#';
	pieceArray[3][1][1] = '#';
}

Piece_Crazy::Piece_Crazy(Well* well, int x, int y) : Piece(well, x, y) {
	// Creating orientation 0
	for (int j = 1; j < 3; j++) {
		pieceArray[0][j][1] = '#';
	}
	pieceArray[0][0][0] = '#';
	pieceArray[0][3][0] = '#';
	// Creating orientation 1
	for (int k = 1; k < 3; k++) {
		pieceArray[1][2][k] = '#';
	}
	pieceArray[1][3][0] = '#';
	pieceArray[1][3][3] = '#';
	// Creating orientation 2
	for (int j = 1; j < 3; j++) {
		pieceArray[2][j][2] = '#';
	}
	pieceArray[2][0][3] = '#';
	pieceArray[2][3][3] = '#';
	// Creating orientation 3
	for (int k = 1; k < 3; k++) {
		pieceArray[3][1][k] = '#';
	}
	pieceArray[3][0][0] = '#';
	pieceArray[3][0][3] = '#';
}

void Piece_Vapor::specialAction() {
	// Vaporizes the blocks 2 rows above and 2 rows below the VaporBomb
	for (int i = m_x + 1; i < m_x + 3; i++) {
		for (int j = m_y - 2; j < m_y + 3; j++) {
			if (j < 0) {
				continue;
			}
			// Don't vaporize the border of the well
			if (m_well->getChar(i, j) == '@') {
				break;
			}
			m_well->update(i, j, ' ');
		}
	}
}

void Piece_Foam::specialAction() {
	// Fill the 5x5 bounding box surrounding the FoamBomb
	RecurseYou(m_x + 1, m_y + 1);
}

void Piece_Foam::RecurseYou(int x, int y) {
	// Checking the boundaries of the 5x5 bounding box
	// Do nothing if past the boundary
	if (y < 0 || x < m_x - 1 || x > m_x + 3 || y < m_y - 1 || y > m_y + 3) {
		return;
	}
	// Do nothing if the cell isn't empty
	char c = m_well->getChar(x, y);
	if (c != ' ') {
		return;
	}
	// Fill the empty cell with the '*' character
	m_well->update(x, y, '*');
	// Move to the cell on the right
	RecurseYou(x + 1, y);
	// Move to the cell on the left
	RecurseYou(x - 1, y);
	// Move to the cell below
	RecurseYou(x, y + 1);
	// Move to the cell above
	RecurseYou(x, y - 1);
}

void Piece_Crazy::moveLeft() {
	// If there's no collision, move the piece to the right
	if (!movementCollision(1, 0)) {
		m_x++;
	}
}

void Piece_Crazy::moveRight() {
	// If there's no collision, move the piece to the left
	if (!movementCollision(-1, 0)) {
		m_x--;
	}
}