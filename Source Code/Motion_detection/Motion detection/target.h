/*
K&M softaware
Written by Larry Kvitnevski and Jack Moseley

The target class
*/
#ifndef TARGET_H
#define TARGET_H

//for ease of passing x,y coordinates
struct Point
{
	int x, y;
};

//used for related targets
struct TargetID
{
	int targetNum;
	Point mid;
};

class Target
{
public:
	//constuctor - sets Min/Max-y/x and Mid x/y 
	Target( const vector< Point >& inputList );

	//return the x,y-Max,Min and xMid, yMid
	int xMinF() const, xMaxF() const, yMinF() const, yMaxF() const;
	int xMidF() const, yMidF() const;

	//returns the number of pixels in the target
	int targetPixelCount() const;

	//returns the point at the index
	Point pointAt( int index ) const;

	//adds target that could have been this target in the previous frame
	void addRelatedTarget( TargetID target );

	//returns number of related targets
	int numRelatesTargets() const;

	//returns the mid point of the target at the index
	Point relatedTargetAt( int index ) const;
	
private:
	//location of all the points of the target
	vector< Point > pointList;

	//list of related targets in the previous frame
	vector< TargetID > relatedTargetList;

	//finds the max/min x/y pixel
	void findBorders();

	//the min max mid values
	int xMin, xMax, yMin, yMax;
	int xMid, yMid;

	//number of pixels in the target
	int numberOfPoints;
};

#endif