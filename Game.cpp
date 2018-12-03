#include "Game.h"
#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <queue>
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

Game::Game(int width, int height)
	: m_well(width, height), m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1),
	startX(4), startY(0), score(0), rowsLeft(5)
{
}

Piece* Game::generatePiece() {
	Piece* p;
	// Generating a random piece and assigning the 
	// pointer p to the memory address of the new piece
	switch (chooseRandomPieceType()) {
	case PIECE_I:
		p = new Piece_I(&m_well, startX, startY);
		break;
	case PIECE_L:
		p = new Piece_L(&m_well, startX, startY);
		break;
	case PIECE_J:
		p = new Piece_J(&m_well, startX, startY);
		break;
	case PIECE_T:
		p = new Piece_T(&m_well, startX, startY);
		break;
	case PIECE_O:
		p = new Piece_O(&m_well, startX, startY);
		break;
	case PIECE_S:
		p = new Piece_S(&m_well, startX, startY);
		break;
	case PIECE_Z:
		p = new Piece_Z(&m_well, startX, startY);
		break;
	case PIECE_VAPOR:
		p = new Piece_Vapor(&m_well, startX, startY);
		break;
	case PIECE_FOAM:
		p = new Piece_Foam(&m_well, startX, startY);
		break;
	case PIECE_CRAZY:
		p = new Piece_Crazy(&m_well, startX, startY);
		break;
	}
	return p;
}

void Game::clearNextPieceDisplay() {
	// Creating an empty 4x4 box
	char empty[4][4] = {
	{ ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ' }
    };
	// Displaying the empty box to clear where 
	// the next piece should be displayed
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char c = empty[i][j];
			m_screen.gotoXY(NEXT_PIECE_X + i, NEXT_PIECE_Y + j);
			m_screen.printChar(c);
		}
	}
}

int Game::maxTimer() {
	// Determining the max amount of time allowed before
	// automatically shifting the piece down a row
	// T = maximum(1000 - (100 * (L - 1)), 100)
	int time = 1000 - (100 * (m_level - 1));
	int result = time;
	if (time < 100) {
		result = 100;
	}
	return result;
}

void Game::refreshWell(Piece* currentPiece) {
	// Redisplaying the well and currentPiece
	m_well.display(m_screen, WELL_X, WELL_Y);
	currentPiece->display(m_screen, WELL_X, WELL_Y);
}

void Game::play()
{
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]

    for(;;)
    {
        if ( ! playOneLevel())
            break;
        displayPrompt("Good job!  Press the Enter key to start next level!");
		displayStatus();
        waitForEnter();
		// Displaying empty well after each level
		m_well.clearWell();
		m_well.display(m_screen, WELL_X, WELL_Y);
        m_level++;
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}

void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus()
{
	// Converting integers to strings
	string myScore = to_string(score);
	string myRows = to_string(rowsLeft);
	string myLevel = to_string(m_level);
	// Printing everything out in their correct places
	// Printing the score, rows left, and level in a right-justified 7 character wide field
	m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
	m_screen.printString("Next piece:");
	m_screen.gotoXY(SCORE_X, SCORE_Y);
	m_screen.printString("Score:     " + string(7 - myScore.length(), ' ') + myScore);
	m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
	m_screen.printString("Rows left: " + string(7 - myRows.length(), ' ') + myRows);
	m_screen.gotoXY(LEVEL_X, LEVEL_Y);
	m_screen.printString("Level:     " + string(7 - myLevel.length(), ' ') + myLevel);
    // [Replace this with code to display the status.]
}

bool Game::playOneLevel()
{
	// Need to vaporize 5 * m_level of rows to advance to next level
	rowsLeft = 5 * m_level;
	// Creating the current piece
	Piece* currentPiece = generatePiece();
	Piece* nextPiece;
	// Level isn't finished until certain number of rows are cleared
	while (rowsLeft > 0) {
		Timer timer;  // Start timer
		bool atRest = false;
		nextPiece = generatePiece();  // Generating next piece
		// Displaying currentPiece and nextPiece
		currentPiece->display(m_screen, WELL_X, WELL_Y);
		clearNextPieceDisplay();
		nextPiece->display(m_screen, (NEXT_PIECE_X - startX), NEXT_PIECE_Y); 
		displayStatus();  // Update status
		// If new piece can't be placed in well, then game is over
		if (currentPiece->wellIsFull()) {
			return false;
		}
		// While the piece hasn't come to rest, allow player to move the piece
		while (!atRest) {
			timer.start();
			while (timer.elapsed() < maxTimer()) {
				char c;
				// Break out of loop if piece has come to rest
				if (atRest) {
					break;
				}
				if (getCharIfAny(c)) {
					switch(c) {
					// Quit the game
					case 'q':
					case 'Q':
						delete currentPiece;
						delete nextPiece;
						return false;
					// Drop current piece down as far as it can go
					// Piece has come to rest
					case ' ':
						currentPiece->dropDown();
						refreshWell(currentPiece);
						atRest = true;
						break;
					// Carry out moveLeft function of current piece
					case ARROW_LEFT:
						currentPiece->moveLeft();
						refreshWell(currentPiece);
						break;
					// Carry out moveRight function of current piece
					case ARROW_RIGHT:
						currentPiece->moveRight();
						refreshWell(currentPiece);
						break;
					// Carry out arrowUp function of current piece
					case ARROW_UP:
						currentPiece->arrowUp();
						refreshWell(currentPiece);
						break;
					// If piece can't be moved down further, the piece has come to rest.
					// Otherwise, reset the timer and move the piece down one row.
					case ARROW_DOWN:
						if (currentPiece->movementCollision(0, 1)) {
							atRest = true;
							currentPiece->convertPieceArray();
							refreshWell(currentPiece);
							break;
						}
						else {
							timer.start();
							currentPiece->moveDown();
							refreshWell(currentPiece);
						}
						break;
					}
				}
			}
			// If piece can't move down any further, it has come to rest
			if (currentPiece->movementCollision(0, 1)) {
				atRest = true;
				break;
			}
			// Automatically shifts piece down if it's not already at 
			// the bottom of its descent once the timer times out
			currentPiece->increaseYCoord();
			refreshWell(currentPiece);
		}
		atRest = true;
		// Carry out any special actions a piece has when at rest
		// and then redisplay the well
		currentPiece->specialAction();
		currentPiece->convertPieceArray();
		m_well.display(m_screen, WELL_X, WELL_Y);
		int i = m_well.getHeight() - 1;
		int count = 0;
		// Checking all the rows to see if they can be vaporized.
		// If row is vaporized, rowsLeft decreases by 1 and well is redisplayed.
		while (i >= 0) {
			if (m_well.willBeVaporized(i)) {
				rowsLeft -= 1;
				count += 1;
				m_well.vaporize(i);
				m_well.display(m_screen, WELL_X, WELL_Y);
			}
			else {
				i--;
			}
		}
		// Increasing the score depending on how many rows were vaporized simultaneously
		switch (count) {
		case 1:
			score += 100;
			break;
		case 2:
			score += 200;
			break;
		case 3:
			score += 400;
			break;
		case 4:
			score += 800;
			break;
		case 5:
			score += 1600;
			break;
		}
		delete currentPiece;
		// Setting the current piece to the next piece
		currentPiece = nextPiece;
	} 
	delete nextPiece;
    return true;  // [Replace this with the code to play a level.]
}
