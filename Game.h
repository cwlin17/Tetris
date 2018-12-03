#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>
// [Add other #include directives as necessary.]

class Game
{
  public:
    Game(int width, int height);
	Piece* generatePiece();
	void clearNextPieceDisplay();
	int maxTimer();
	void refreshWell(Piece* currentPiece);
    void play();
    bool playOneLevel();
    void displayPrompt(std::string s);
    void displayStatus();
    // [Add other members as necessary.]

  private:
    Well    m_well;
    Screen  m_screen;
    int     m_level;
	int     startX, startY, score, rowsLeft;
    // [Add other members as necessary.]
};

#endif // GAME_INCLUDED
