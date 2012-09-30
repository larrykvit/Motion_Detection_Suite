#ifndef TARGET_H
#define TARGET_H

struct Point
{
	int x, y;
};

class Target
{
public:
	Target( const vector< Point >& inputList );
	int xMinF() const, xMaxF() const, yMinF() const, yMaxF() const;
	int targetPixelCount() const;
	Point pointAt( int index ) const;
private:
	vector< Point > pointList;
	void findBorders();
	int xMin, xMax, yMin, yMax;
	int numberOfPoints;
};

#endif