#ifndef TARGET_H
#define TARGET_H

struct Point
{
	int x, y;
};

struct TargetID
{
	int targetNum;
	Point mid;
};

class Target
{
public:
	Target( const vector< Point >& inputList );
	int xMinF() const, xMaxF() const, yMinF() const, yMaxF() const;//return the x,y-Max,Min
	int xMidF() const, yMidF() const;
	int targetPixelCount() const;//returns the number of pixels in the target
	Point pointAt( int index ) const;//returns the point at the index
	void addRelatedTarget( TargetID target );
	Point relatedTargetAt( int index ) const;//returns target at index
	int numRelatesTargets() const;
private:
	vector< Point > pointList;//location of all the points of the target
	vector< TargetID > relatedTargetList;//list of related targets in the previous frame
	void findBorders();//finds the max/min x/y pixel
	int xMin, xMax, yMin, yMax;
	int xMid, yMid;
	int numberOfPoints;
};

#endif