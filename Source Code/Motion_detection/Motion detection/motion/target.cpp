#include "stdafx.h"
#include "target.h"

Target::Target( const vector< Point >& inputList )
{
	pointList = inputList;
	numberOfPoints = pointList.size();
	findBorders();
}

void Target::findBorders()
{
	xMin = pointList[0].x;
	xMax = pointList[0].x;
	yMin = pointList[0].y;
	yMax = pointList[0].y;
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
	xMid = ( xMin + xMax) /2;
	yMid = ( yMin + yMax) /2;
}

int Target::targetPixelCount() const
{
	return numberOfPoints;
}

Point Target::pointAt( int index ) const
{
	return pointList[ index ];
}

int Target::xMaxF() const
{
	return xMax;
}

int Target::xMinF() const
{
	return xMin;
}

int Target::yMaxF() const
{
	return yMax;
}

int Target::yMinF() const
{
	return yMin;
}

int Target::xMidF() const
{
	return xMid;
}

int Target::yMidF() const
{
	return yMid;
}

void Target::addRelatedTarget( TargetID target )
{
	relatedTargetList.push_back( target );
}

int Target::numRelatesTargets() const
{
	return relatedTargetList.size();
}

Point Target::relatedTargetAt( int index ) const
{
	return relatedTargetList[ index ].mid;
}