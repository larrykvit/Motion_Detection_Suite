/*
K&M softaware
Written by Larry Kvitnevski and Jack Moseley

The target class
*/
#include "stdafx.h"
#include "target.h"

//constuctor - sets xMin, xMax, yMin, yMax, xMid, yMid, pointlist, number of points
Target::Target( const vector< Point >& inputList )
{
	//sets parameters of object
	pointList = inputList;
	numberOfPoints = pointList.size();

	//finds min,max,mid
	findBorders();
}

//finds the borders of the target
void Target::findBorders()
{
	//initializes the min, max to the first variable
	xMin = pointList[0].x;
	xMax = pointList[0].x;
	yMin = pointList[0].y;
	yMax = pointList[0].y;

	//compares each value (min max) and sets new value if needed
	for( vector< Point >::size_type i = 0; i < pointList.size(); i++ )
	{
		if( pointList[i].x < xMin )
			xMin = pointList[i].x;
		if( pointList[i].x > xMax )
			xMax = pointList[i].x;
		if( pointList[i].y < yMin )
			yMin = pointList[i].y;
		if( pointList[i].y > yMax )
			yMax = pointList[i].y;
	}
	//the mid point of the object
	xMid = ( xMin + xMax) /2;
	yMid = ( yMin + yMax) /2;
}

//the amount of pixels in the target
int Target::targetPixelCount() const
{
	return numberOfPoints;
}

//returns the the point at the index
Point Target::pointAt( int index ) const
{
	return pointList[ index ];
}

//returns the highest pixel of the target
int Target::xMaxF() const
{
	return xMax;
}

//returns the lowest pixel of the target
int Target::xMinF() const
{
	return xMin;
}

//returns the right most pixel of the target
int Target::yMaxF() const
{
	return yMax;
}

//returns the left most pixel of the target
int Target::yMinF() const
{
	return yMin;
}

//returns the centre pixel x wise
int Target::xMidF() const
{
	return xMid;
}

//returns the centre pixel y wise
int Target::yMidF() const
{
	return yMid;
}

//adds the target id of the related trget to the vector
void Target::addRelatedTarget( TargetID target )
{
	relatedTargetList.push_back( target );
}

//returns the amount of relatd targets
int Target::numRelatesTargets() const
{
	return relatedTargetList.size();
}

//returns the related target at the index given
Point Target::relatedTargetAt( int index ) const
{
	return relatedTargetList[ index ].mid;
}