#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

#include "UserInterface.h"

enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType();

class Screen;
class Well;

class Piece {
public:
	Piece(Well* well, int x, int y);
	virtual ~Piece();
	void display(Screen& screen, int x, int y);
	int nextOrientation();
	virtual void moveLeft();
	virtual void moveRight();
	void moveDown();
	void arrowUp();
	void dropDown();
	bool movementCollision(int x, int y) const;
	bool orientationCollision() const;
	virtual void convertPieceArray();
	void increaseYCoord() { m_y += 1; }
	bool wellIsFull() const;
	virtual void specialAction() { } 
protected:  
	Well* m_well;
	char pieceArray[4][4][4];  // 4x4 array containing the piece
	int m_x, m_y, orientation;
};

class Piece_I : public Piece {
public:
	Piece_I(Well* well, int x, int y);
};

class Piece_L : public Piece {
public:
	Piece_L(Well* well, int x, int y);
};

class Piece_J : public Piece {
public:
	Piece_J(Well* well, int x, int y);
};

class Piece_T : public Piece {
public:
	Piece_T(Well* well, int x, int y);
};

class Piece_O : public Piece {
public:
	Piece_O(Well* well, int x, int y);
};

class Piece_S : public Piece {
public:
	Piece_S(Well* well, int x, int y);
};

class Piece_Z : public Piece {
public:
	Piece_Z(Well* well, int x, int y);
};

class Piece_Vapor : public Piece {
public:
	Piece_Vapor(Well* well, int x, int y);
	virtual void specialAction();
	// Don't convert the piece into a '$'
	virtual void convertPieceArray() { }
};

class Piece_Foam : public Piece {
public:
	Piece_Foam(Well* well, int x, int y);
	virtual void specialAction();
	void RecurseYou(int x, int y);
	// Don't convert the piece into a '$'
	virtual void convertPieceArray() { }
};

class Piece_Crazy : public Piece {
public:
	Piece_Crazy(Well* well, int x, int y);
	virtual void moveLeft();
	virtual void moveRight();
};
#endif // PIECE_INCLUDED
