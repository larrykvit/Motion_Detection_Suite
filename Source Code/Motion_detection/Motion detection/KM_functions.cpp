/*
K&M softaware
Written by Larry Kvitnevski and Jack Moseley

Set of functions used by the motion detection program
*/
#include "stdafx.h"
#include "KM_functions.h"

//colours for filling targets, colouring the target borders, and drawing the line fro related targets
Colour fillIn = { 0, 0, 0 };
Colour border = {255, 0, 255 };
Colour line = {255, 255, 255};
//this struct (white) was used for testing purposes
Colour white = { 255, 255, 255 };

bool equalColour( png_bytep col_ptr_img, png_bytep col_ptr_bkg, int x, int threshold )
{
	//the pointer has rgb values consecutively, thus x*3 + (rgb)
	//compares the rgb values two pixels
	bool a =	( *( col_ptr_img + 3*x + 0 ) >= (*( col_ptr_bkg + 3*x + 0 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 0 ) <= (*( col_ptr_bkg + 3*x + 0 ) ) + threshold ) &&
				( *( col_ptr_img + 3*x + 1 ) >= (*( col_ptr_bkg + 3*x + 1 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 1 ) <= (*( col_ptr_bkg + 3*x + 1 ) ) + threshold ) &&
				( *( col_ptr_img + 3*x + 3 ) >= (*( col_ptr_bkg + 3*x + 3 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 3 ) <= (*( col_ptr_bkg + 3*x + 3 ) ) + threshold ) ;
	//returns true if the two colours are within a certain threshold
	return a;
}

void compareImg( ImageAttr* attr_bkg, ImageAttr* attr_img, int** row_motion, int threshold )
{
	//for simplicity
	int height = attr_bkg->imageHeight;
	int width = attr_bkg->imageWidth;
	
	//create constant pointers to make sure they dont change and for ease of use
	const png_bytep* row_ptr_img = attr_img->row_pointers;
	const png_bytep* row_ptr_bkg = attr_bkg->row_pointers;
	
	for( int y = 0; y < height; y++ )
	{
		
		//create constant pointers to make sure they dont change and for ease of use
		const png_bytep col_ptr_img = row_ptr_img[y];
		const png_bytep col_ptr_bkg = row_ptr_bkg[y];

		int* col_motion = row_motion[y];
		
		for( int x = 0; x < width; x++ )
		{
			//for every pixel, from left to right, top to bottom
			
			//bool equalColour( png_bytep col_ptr_img, png_bytep col_ptr_bkg, int x, int threshold )
			if( equalColour(  col_ptr_img,  col_ptr_bkg, x, threshold) )
			{
				//equal colour, no motion
				col_motion[x] = 0;
				//printf("%i,%i\n",0,x);
			}
			else
			{
				//not equal colours, motion
				col_motion[x] = -1;
				//printf("%i,%i\n",1,x);
			}
		}
	}
}

void openImage( FILE* f, ImageAttr* attr )
{
	
	//the following code opens a .png image
	BYTE bom[8];
	
	fread( bom, 1, sizeof( bom ), f );

	bool isPng = ( 0 == png_sig_cmp( bom, 0, sizeof( bom ) ) );
	ASSERT( isPng );

	png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    ASSERT( png_ptr );
	png_infop info_ptr = png_create_info_struct( png_ptr );
	ASSERT( info_ptr );
	png_init_io( png_ptr, f );
	png_set_sig_bytes( png_ptr, sizeof( bom ) );

	png_read_info( png_ptr, info_ptr );

	unsigned width = png_get_image_width(png_ptr, info_ptr);
	unsigned height = png_get_image_height(png_ptr, info_ptr);
	unsigned color_type = png_get_color_type(png_ptr, info_ptr);
	unsigned bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	ASSERT( color_type == PNG_COLOR_TYPE_RGB );
	ASSERT( bit_depth == 8 );

	int number_of_passes = png_set_interlace_handling( png_ptr );
	png_read_update_info( png_ptr, info_ptr );
	
	png_bytep* row_pointers = (png_bytep*) malloc( sizeof(png_bytep) * height );
	for( unsigned y = 0; y < height; y++ )
		row_pointers[y] = (png_byte*) malloc( png_get_rowbytes( png_ptr, info_ptr ) );
	png_read_image( png_ptr, row_pointers );

	fclose( f );

	//sets the atributes of the image to a struct
	attr->row_pointers = row_pointers;
	attr->imageWidth = width;
	attr->imageHeight = height;
}

void writePNG( FILE* f, ImageAttr* attr_img )
{
	//the following code saves a .png image
	
	int height = attr_img->imageHeight;
	int width = attr_img->imageWidth;
	
	png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	ASSERT( png_ptr );
	png_infop info_ptr = png_create_info_struct( png_ptr );
	ASSERT( info_ptr );
	png_init_io( png_ptr, f );

	png_set_IHDR( png_ptr, info_ptr, width, height,
                     8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
	png_write_info( png_ptr, info_ptr );

	png_bytep* row_pointers = (png_bytep*) malloc( sizeof( png_bytep ) * height );
	for( int y = 0; y < height; y++ )
		row_pointers[y] = (png_byte*) malloc( png_get_rowbytes( png_ptr, info_ptr ) );

	const png_bytep* row_ptr_img = attr_img->row_pointers;
	for( int y = 0; y < height; y++ )
	{
		png_byte* row = row_pointers[y];
		const png_bytep col_ptr_img = row_ptr_img[y];

		for( int x = 0; x < width ; x++ )
		{
			png_byte* ptr = &(row[x*3]);
			ptr[0] = *(col_ptr_img + 3*x + 0);
			ptr[1] = *(col_ptr_img + 3*x + 1);
			ptr[2] = *(col_ptr_img + 3*x + 2);
			
		}
	}

	png_write_image( png_ptr, row_pointers );
	png_write_end( png_ptr, NULL );

	for( int y = 0; y < height; y++ )
		free( row_pointers[y] );
	free( row_pointers );
	row_pointers = NULL;
}

bool affirmImageSize( ImageAttr* attr_bg, ImageAttr* attr_img )
{
	//checks if the width and height of the two images are equal
	bool check;
	check = attr_bg->imageHeight == attr_img->imageHeight && attr_bg->imageWidth == attr_img->imageWidth;
	return check;
}

bool affirmImageSizeAll( ImageAttr* attr_bg, const vector< ImageAttr >& attr_img)
{
	//unused function
	//checks that all the images are the same size
	bool check = true;
	for( vector< ImageAttr >::size_type i = 0; i < attr_img.size() ; i++ )
	{
		check = check && attr_bg->imageHeight == attr_img[i].imageHeight && attr_bg->imageWidth == attr_img[i].imageWidth;
	}
	
	return check;
}

void getTargets( int imageHeight, int imageWidth, int** row_motion, vector< Target >* target, const int minTargetSize )
{
	int numOfTargets = 0;

	for( int y = 0; y < imageHeight; y++ )
	{
		for( int x = 0; x < imageWidth; x++ )
		{
			//for every pixel in the image, moving from left to right, top to bottom
			
			//Target grouping algorithm summary
			/*
			1- the first point of a target is found
			2- point is added to the list (vector) of target points
			3- point is removed from the motion image
			4- enters loop for every point (currently 1)
			5- for the given point looks to 8 surrounding pixels
			6- if it is a motion pixel - adds to the list of target points
			7- when loop exits, all points in the target are in the list
			8- if the target size is above the threshold then the list is used as the constuctor for a new target object
			side note - the target objects are sorted by size, first is largest
			*/

			//if the pixel contains motion
			if( row_motion[y][x] == -1 )
			{
				//this is the next target
				numOfTargets++;

				//vector that will contain all the points of the target
				vector< Point > pointList;

				//point struct used for ease of use
				Point p;
				p.x = x;
				p.y = y;

				//adds the first point to the list
				pointList.push_back (p);
				
				//makes the motion pixel not equal to -1, so that the algorithm will not detect it twice
				row_motion[p.y][p.x] = numOfTargets;

				//the current point at which the algorithm is at (in the loop)
				int curX;
				int curY;
				
				//loops for every point in a target
				for( unsigned int index = 0; index < pointList.size(); index++ )
				{
					//sets the current point to the point being looked at this iteration
					p = pointList[index];
					curX = p.x;
					curY = p.y;

					//checks that the pixel left is within bounds
					if( ((curX-1) >= 0) )
					{
						p.x = curX-1;
						
						//checks the pixel above (the left pixel) is within bounds
						if( (curY-1) >= 0 && (row_motion[curY-1][curX-1] == -1) )
						{
							//adds it the the list of target points
							p.y = curY-1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						//repeated for the middle pixel
						if( (row_motion[curY][curX-1] == -1) )
						{
							p.y = curY;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						//repeated for pixel down
						if( (curY+1) < imageHeight && (row_motion[curY+1][curX-1] == -1) )
						{
							p.y = curY+1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
					}

					//same as above piece of code, but checks above and below the current pixel
					{
						p.x = curX;
					
						if( (curY-1) >= 0 && (row_motion[curY-1][curX] == -1) )
						{
							p.y = curY-1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						if( (curY+1) < imageHeight && (row_motion[curY+1][curX] == -1) )
						{
							p.y = curY+1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
					}

					//same as above code except for right pixel
					if( ((curX+1) < imageWidth) )
					{
						p.x = curX+1;
					
						if( (curY-1) >= 0 && (row_motion[curY-1][curX-1] == -1) )
						{
							p.y = curY-1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						if( (row_motion[curY][curX+1] == -1) )
						{
							p.y = curY;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						if( (curY+1) < imageHeight && (row_motion[curY+1][curX+1] == -1) )
						{
							p.y = curY+1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
					}
				}

				//pointList contains all the points of the object now

				int size = pointList.size();

				if( size > minTargetSize )
				{
					//adds the Target if it is above a certain size
					
					if( target->size() == 0 )
					{
						//this is the first target
						target->push_back( pointList );
					}
					else
					{
						//inserts the target into order (sortd by pointlist size)
						int n;
						int end = target->size();
						for( n = 0; n < end; n++ )
						{
							if( size > target->at(n).targetPixelCount() )
							{
								target->insert( target->begin() + n, pointList );
								break;
							}
						}
						if( n == end )
						{
							//the target was not added in the loop since it was the smallest one
							target->push_back( pointList );
						}
					}
					//prints out the target number so that the user can monitor the algorithm
					printf("%i,", target->size() );
				}

			}

		}
	}
}

void colourImage( ImageAttr* attr_img, const vector< Target >& target, const int numOfTargets )
{
	//for ease of use
	png_bytep* row_ptr_img = attr_img->row_pointers;
	
	//determines till what target to loop to
	int maxTargets;
	if( numOfTargets == 0 )
		maxTargets = target.size();
	else
		maxTargets = numOfTargets;
	if( maxTargets >= target.size() )
		maxTargets = target.size();
	
	//for every taget [0] to [maxTarget - 1]
	for( vector< Target >::size_type i = 0; i < maxTargets; i++ )
	{
		//fills in the targets in the image
		for( int j = 0; j < target[i].targetPixelCount(); j++)
		{
			Point curPoint = target[i].pointAt( j );
			fillColour( row_ptr_img, curPoint.x, curPoint.y, &fillIn );
		}
		
		//boxes the targets
		for( int y = target[i].yMinF(); y <= target[i].yMaxF(); y++)
		{
			int xMin = target[i].xMinF();
			int xMax = target[i].xMaxF();
			
			//colours the left and right vertical border
			fillColour( row_ptr_img, xMin, y, &border );
			fillColour( row_ptr_img, xMax, y, &border );
			
			//if y is at the horizontal border, colour it in
			if( y == target[i].yMinF() || y == target[i].yMaxF())
			{
				//colours the top and bottom horizontal border
				for( int x = target[i].xMinF(); x <= target[i].xMaxF(); x++)
				{
					fillColour( row_ptr_img, x, y, &border );
				}
			}
		}
	}	
}

void fillColour( png_bytep* row_ptr_img, int x, int y, Colour* colour )
{
	//colours the given pixel the given colour
	*( row_ptr_img[y] + 3*x + 0 ) = colour->R;
	*( row_ptr_img[y] + 3*x + 1 ) = colour->G;
	*( row_ptr_img[y] + 3*x + 2 ) = colour->B;
}

void drawLine( int x0, int y0, int x1, int y1, png_bytep* row_ptr_img )
{
	//draws line between two points

	//this is Bresenham's line algorithm
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if( steep )
	{
		swap(x0, y0);
		swap(x1, y1);
	}
	if( x0 > x1 )
	{
		swap(x0, x1);
		swap(y0, y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = deltax / 2;
	int ystep;
	int y = y0;
	if( y0 < y1 )
		ystep = 1;
	else
		ystep = -1;
	for( int x = x0; x <= x1; x++ )
	{
		if( steep )
			fillColour( row_ptr_img, y, x, &line );
		else
			fillColour( row_ptr_img, x, y, &line );
		error = error - deltay;
		if( error < 0 )
		{
			y = y + ystep;
			error = error + deltax;
		}
	}
}

void getRelatedTargets( vector< Target >* curTarget, vector< Target >* prevTarget, const double sizeThreshold, const int searchRadius )
{
	int dX, dY;
	Point ori;

	//for every target of this frame
	for( unsigned int targetNumber = 0; targetNumber < curTarget->size(); targetNumber++ )
	{
		//the middle of the current target
		ori.x = (*curTarget)[targetNumber].xMidF();
		ori.y = (*curTarget)[targetNumber].yMidF();

		//checks against every target of the previous frame
		for( unsigned int i = 0; i < prevTarget->size(); i++ )
		{
			//middle of the previous frame target
			Point mid;
			mid.x = (*prevTarget)[i].xMidF();
			mid.y = (*prevTarget)[i].yMidF();

			//the distance between the (x,y coordinate) target and previous frame target 
			dX = mid.x - ori.x;
			dY = mid.y - ori.y;
			
			//size0 is the size of target, size1 is size of the previous frame target
			int size0 = (*prevTarget)[i].targetPixelCount();
			int size1 = (*curTarget)[targetNumber].targetPixelCount();
			
			//checks that the target from frame-1 is within searchRadius and within size limit
			if( withinRange(dX, dY, searchRadius) && withinSize( size0, size1, sizeThreshold ) )
			{
				//it is a related target
				//new struct to store info about the related target
				TargetID relatedTarget;
				relatedTarget.targetNum = i;
				relatedTarget.mid = mid; 

				//adds the related taret to current target
				(*curTarget)[targetNumber].addRelatedTarget( relatedTarget );
			}
		}
	}
}

bool withinRange( int x, int y, int searchRadius )
{
	//returns true if diffrence is x and y are within the circle searchradius
	return (x*x + y*y <= searchRadius*searchRadius);
}

bool withinSize( int size1, int size2, double sizeThreshold )
{
	//returns true if the two sizes are within the percentage threshold
	return ( size1 <= size2 * ( 1 + sizeThreshold) ) && ( size1 >= size2 * ( 1 - sizeThreshold ) );
}

void colourLines( const vector< vector< Target > >& target, int frameNum, ImageAttr* attr_img, const int numOfTargets )
{
	png_bytep* row_ptr_img = attr_img->row_pointers;

	//determines till what target to loop to
	int maxTargets;
	if( numOfTargets == 0 )
		maxTargets = target[frameNum].size();
	else
		maxTargets = numOfTargets;
	if( maxTargets >= target[frameNum].size() )
		maxTargets = target[frameNum].size();
	
	//for every taget [0] to [maxTarget - 1]
	for( vector< vector< Target > >::size_type i = 0; i < maxTargets; i++ )
	{
		
		//determines to what related target to loop to
		int numRT;
		if( numOfTargets == 0 )
			numRT = target[frameNum][i].numRelatesTargets();
		else
			numRT = numOfTargets;
		if( numRT >= target[frameNum][i].numRelatesTargets() )
			numRT = target[frameNum][i].numRelatesTargets();

		//for every related target
		for( int n = 0; n < numRT; n++ )
		{

			//coordinate of current target
			int x0 = target[frameNum][i].xMidF();
			int y0 = target[frameNum][i].yMidF(); 
			
			//coordinates of related targets
			int x1 = target[frameNum][i].relatedTargetAt( n ).x;
			int y1 = target[frameNum][i].relatedTargetAt( n ).y;

			//draws a line between them
			drawLine( x0, y0, x1, y1, row_ptr_img );

			//if the frame looked at is not the first one
			if( frameNum > 1 )
			{
				//colour the lines for the related targets
				colourLines( target, frameNum - 1, attr_img, numOfTargets );
				/* ASIDE
				this recursion can be seen as traversing a tree
				each target can have any number of related targets
				and each related target has any number of targets
				*/
			}
		}
	}
}

int abs( int x )
{
	//returns abdolut value of x
	if( x > 0 )
		return x;
	else
		return -1*x;
}

void outputRelatedTargets( const vector<Target>& target, const int numOfTargets )
{
	//similair to the function colourLines, but does not have recursion
	
	//determines till what target to loop to
	int maxTargets;
	if( numOfTargets == 0 )
		maxTargets = target.size();
	else
		maxTargets = numOfTargets;
	if( maxTargets >= target.size() )
		maxTargets = target.size();
	
	//for every taget [0] to [maxTarget - 1]
	for( vector< Target >::size_type i = 0; i < maxTargets; i++ )
	{
		printf( "Target:%i; related to: ", i + 1 );
		
		//determines to what related target to loop to
		int numRT;
		if( numOfTargets == 0 )
			numRT = target[i].numRelatesTargets();
		else
			numRT = numOfTargets;
		if( numRT >= target[i].numRelatesTargets() )
			numRT = target[i].numRelatesTargets();

		//for every related target
		for( int n = 0; n < numRT; n++ )
		{
			//print out to which targets it is related to
			printf( "%i,", n + 1 );
		}
		printf( "\n" );
	}
}

bool fileOpen(Parameter* para)
{
	// Numeric values are later converted to reduce errors from incorrect characters in fields
	string filePathOpen, filePathSave, imgName, imgStartNum, imgEndNum, colourThreshold, searchRadius, sizeThreshold, minTargetSize, numOfTargets;
	// Open the input file
	ifstream inputFile ("input.txt");
	// Check if the file has been successfully opened
	if (inputFile.is_open())
	{
		// Take and save information from the file line by line
		// getline used as file paths may have spaces
		getline(inputFile, filePathOpen);
		getline(inputFile, filePathSave);
		inputFile >> imgName >> imgStartNum >> imgEndNum;
		inputFile.close();
	}
	else
	{
		//the file is not found
		return false;
	}
	// Read the preferences file
	ifstream prefFileIn ("preferences.txt");
	if (prefFileIn.is_open())
	{
		prefFileIn >> colourThreshold >> searchRadius >> sizeThreshold >> minTargetSize >> numOfTargets;
		prefFileIn.close();
	}
	// If the preferences file is not found and is created with default values
	else
	{
		ofstream prefFileOut ("preferences.txt");
		prefFileOut << 10 << endl << 10 << endl << 10 << endl << 10 << endl << 10;
		prefFileOut.close();
		colourThreshold = 10; 
		searchRadius = 10;
		sizeThreshold = 10;
		minTargetSize = 10;
		numOfTargets = 10;
	}
	// Store all settings in structure para
	para->filePathOpen = filePathOpen;
	para->filePathSave = filePathSave;
	para->imgName = imgName;
	para->imgStartNum = atoi( imgStartNum.c_str() );
	para->imgEndNum = atoi( imgEndNum.c_str() );
	para->colourThreshold = atoi(colourThreshold.c_str());
	para->searchRadius = atoi(searchRadius.c_str());
	para->sizeThreshold = atof(sizeThreshold.c_str());
	para->minTargetSize = atoi(minTargetSize.c_str());
	para->numOfTargets = atoi(numOfTargets.c_str());

	return true;
}

void writeInfo( const vector< vector<Target> >& target, const int numOfImages, const int numOfTargets )
{
	//similair to outputRelatedTargets, but this function does it for every frame and saves the ouput to a file
	
	ofstream fileInS("info.txt");

	//for every frame
	for( unsigned int frame = 1; frame < numOfImages; frame++ )
	{
		fileInS << endl << "Frame:" << frame << endl;
		//determines till what target to loop to
		int maxTargets;
		if( numOfTargets == 0 )
			maxTargets = target[frame].size();
		else
			maxTargets = numOfTargets;
		if( maxTargets >= target[frame].size() )
			maxTargets = target[frame].size();
	
		//for every taget [0] to [maxTarget - 1]
		for( vector< vector< Target > >::size_type i = 0; i < maxTargets; i++ )
		{
			fileInS << "Target:"<< i + 1 << "; related to: ";
			//printf( "Target:%i; related to: ", i + 1 );
		
			//determines to what related target to loop to
			int numRT;
			if( numOfTargets == 0 )
				numRT = target[frame][i].numRelatesTargets();
			else
				numRT = numOfTargets;
			if( numRT >= target[frame][i].numRelatesTargets() )
				numRT = target[frame][i].numRelatesTargets();

			for( int n = 0; n < numRT; n++ )
			{
				fileInS << n + 1 << ",";
				//printf( "%i,", n + 1 );
			}
			fileInS << endl;
			printf( "\n" );
		}
	}
	fileInS.close();
}

