#ifndef WELL_INCLUDED
#define WELL_INCLUDED

#include <vector>
using namespace std;

class Screen;

class Well
{
public:
	Well(int width, int height);
	int getHeight() const { return m_height; }
    void display(Screen& screen, int x, int y);
	void update(int x, int y, char c);
	char getChar(int x, int y) const;
	vector<vector<char> > getWell() const { return m_well; }
	bool willBeVaporized(int row) const;
	void vaporize(int row);
	void clearWell();
private:
	vector<vector<char> > m_well;
	int m_width, m_height;
};

#endif // WELL_INCLUDED
