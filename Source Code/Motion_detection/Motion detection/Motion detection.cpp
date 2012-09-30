/*
K&M software
Created by Larry Kvitnevski and Jack Moseley

The main code for the motion track program - the entry point for the executable ( _tmain )
*/
/*
Jargon:
motion pixel	- pixels that changed from the previous frame ( ie. there was motion )
target			- motion pixels that were grouped together
related target	- a target in the previous frame that is considered to have moved to the target in the next frame
image attribute	- a struct that contains a pointer to the image and its height & width
target size		- the amount of pixels in a target
motion image	- the image with the motion coloured in ( targets filled in, boxed, and predicted related target motion )
point			- x,y coordinates of a pixel
*/

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	//number of images to compare to the background
	int numOfImages;
	//diffrence allowed in the rgb of a pixel to not be considered a motion pixel
	int colourThreshold;
	//radius that a target in a previoud frame can be to be considered related
	int searchRadius; 
	//size range that a target can be in to be considered a related target
	double sizeThreshold;
	//smalles allowed size of targets, used to filter out small targets and noise
	int minTargetSize;
	//number of targets displayed and tracked ( ranked by size ) ( 0 == all the targets )
	int numOfTargets;
	//offset for the start image
	int startFrame;
	//location of the images to be opened
	string filePathOpen;
	//location where the motion images are saved
	string filePathSave;
	//name of the image to be opened
	string fileName;
	//total file path that is used to open the image
	string fileDir;
	//sturct to be filled out when the input file is opened
	Parameter para;

	if( !fileOpen( &para ) )
	{
		printf( "Input file not found\nPress enter key to exit");
		getchar();
		return 1;
	}

	//initilizing the variables
	numOfImages = abs( para.imgEndNum ) - abs( para.imgStartNum ) + 1;
	colourThreshold = para.colourThreshold;
	searchRadius = para.searchRadius; 
	sizeThreshold = para.sizeThreshold;
	minTargetSize = para.minTargetSize;
	numOfTargets = para.numOfTargets;
	startFrame = para.imgStartNum;
	filePathOpen = para.filePathOpen;
	filePathSave = para.filePathSave;
	fileName = para.imgName;

	//checks if the save directory exists
	if( GetFileAttributesA( filePathSave.c_str() ) == INVALID_FILE_ATTRIBUTES )
	{
		//file path does not exist
		printf( "Save location \"");
		printf( filePathSave.c_str() );
		printf("\" does not exist\nPress enter to exit");
		getchar();
		return 1;
	}

	//first file to be opened is the background image
	fileDir = filePathOpen + fileName + "bg.png";

	//bg image is separated from array of images for ease of use
	ImageAttr imageAttrBG;
	
	//vector of image attributes for the frames
	vector< ImageAttr > imageAttr( numOfImages );

	//pointer to the bg image
	FILE* fBG = fopen( fileDir.c_str() , "rb" );
	
	if( fBG == 0 )
	{
		//the file coud not be found
		printf( "Could not open bg image at location \"");
		printf(  fileDir.c_str() );
		printf( "\"\nPress enter key to exit" );
		getchar();
		return 1;
	}

	//vector of pointers to the files to be opened / saved
	vector< FILE* > f;
	vector< FILE* > fSave;

	//opens image and sets its attributes to the struct - png_bytep*, imageWidth, imageHeight
	openImage( fBG, &imageAttrBG );
	
	//old code
	//affirmImageSize returns true if all the images have the same size as the bkg image (imageAttr[0])
	//if( !affirmImageSizeAll( &imageAttrBG, imageAttr ) )
		//return 1;//TO DO: print that there are image(s) of diffrent size and continue
	
	//imageWidth and Height are same for all images, loops will use imageWidth instead of imageAttr[0].imageWidth
	int imageHeight = imageAttrBG.imageHeight;
	int imageWidth = imageAttrBG.imageWidth;

	//2d vector; [frame number][targetNumber] 
	vector< vector< Target > > target( numOfImages );

	//creates a 2d array for motion
	int** row_motion = (int**) malloc( imageHeight * sizeof( *row_motion ) );
	for( int y = 0; y < imageHeight; y++ )
		row_motion[y] = (int*) malloc( imageWidth * sizeof( *row_motion[y] )  );

	//the last image prossesced (used in the case of an error when the loop is prematurely exited)
	int lastImage = 0;

	//for every image
	for( unsigned int frame = 0; frame < numOfImages; frame++)
	{
		//outputs the frame number
		printf("\nFrame %i\n", frame + startFrame );
		
		//concatinates the file path and image number to be opened
		char numstr[21]; //enough to hold all numbers up to 64-bits
		fileDir = filePathOpen + fileName + itoa( frame + startFrame, numstr, 10 ) + ".png";
		
		//create FILE* for the file of this frame
		f.push_back( fopen( fileDir.c_str() , "rb" ));
		
		if( f[frame] == 0 )
		{
			//the file coud not be found
			printf( "Could not open image at location \"" );
			printf(  fileDir.c_str() );
			printf( "\"\nPress enter key to exit" );
			getchar();
			break;
		}
		
		//opens image and sets its attributes to the struct
		openImage( f[frame], &imageAttr[frame] );
		
		//checks if the image is the same size as the imageBG
		if ( !affirmImageSize( &imageAttrBG, &imageAttr[frame] ) )
		{
			printf("Wrong image size\nPress enter key to exit");
			getchar();
			break;
		}
		
		//compares the two images given and writes the motion to the 2d array given
		compareImg( &imageAttrBG, &imageAttr[frame], row_motion, colourThreshold );
		
		//searchs the motion image and fills the vector with target objects
		getTargets( imageHeight, imageWidth, row_motion, &target[frame], minTargetSize );

		//colours the given image
		colourImage( &imageAttr[frame], target[frame], numOfTargets );

		printf("\nfinal target count:%i\n", target[frame].size() );

		//tracks targets between frames
		//every frame but initial
		if( frame > 0 )
		{
			//finds the realted targets for all targets in target[frame]
			getRelatedTargets( &target[frame], &target[frame-1], sizeThreshold, searchRadius );

			//draws lines between each target and where the related target was
			colourLines( target, frame, &imageAttr[frame], numOfTargets );

			//print out related targets
			outputRelatedTargets( target[frame], numOfTargets );
		}

		//creates fSave[frame]
		fileDir = filePathSave + fileName + "_m" + itoa( frame + startFrame, numstr, 10 ) + ".png";
		fSave.push_back( fopen( fileDir.c_str() , "wb" ));

		//save the imageAttr[frame] to f[frame]
		writePNG( fSave[frame], &imageAttr[frame] );

		lastImage++;
	}

	//output target and related target information to info.txt
	writeInfo( target, lastImage, numOfTargets );

	//motion detection completed sucessfully
	printf( "Motion detection complete\nPress enter to exit" );
	getchar();
	return 0;
}

/*
void fillSearchArea( int Xori, int Yori, int searchRadius, vector< Point >* searchArea, const int yMax, const int xMax )
{
	
	int xLeft = Xori - searchRadius;
	int xRight = Xori + searchRadius;
	int yTop = Yori - searchRadius;
	int yBottom = Yori + searchRadius;
	
	//makes sure that the search box is in the bounds of the image
	if( xLeft < 0 )
		xLeft = 0;
	if( xRight > xMax )
		xRight = xMax;
	if( yTop < 0 )
		yTop = 0;
	if( yBottom > yMax )
		yBottom = yMax;
	
	Point set;

	//loops in a square
	for( int x = xLeft; x <= xRight; x++ )
	{
		for( int y = yTop; y <+ yBottom; y++ )
		{
			//checks to see if inside circle
			if( x*x + y*y <= searchRadius )
			{
				set.x = x;
				set.y = y;
				searchArea->push_back( set );
			}
		}
	}

	/*
	vector< Point > v;
	
	int radius = 12;
	int fc = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int half;
	int x = 0;
	int y = radius;
	int x0 = 0;
	int y0 = 0;

	//printf("x%i y%i \n" ,x0, y0 + radius);
	
	if( radius/2 == radius/2.0 )
		half = radius/2 +1;
	else
		half = radius/2 + 2;

	int yCur = y0 + radius;
	int xCur = x0;
	
	for( int setY = yCur; setY >= -1*yCur; setY-- )
	{
		printf("x%i y%i \n" ,xCur, setY);
		//set.y = setY;
		//v.push_back( set );
	}
	
	//printf("x%i y%i \n" ,x0 + radius, y0);
	xCur = x0+y;
	for( int setX = xCur; setX > half; setX--)
	{
		printf("x%i y%i \n" ,setX, y0+x);
	}

	xCur = x0-y;
	for( int setX = xCur; setX < -1*half; setX++)
	{
		printf("x%i y%i \n" ,setX, y0+x);
	}

	while(x < y)
	{
		// ddF_x == 2 * x + 1;
		// ddF_y =	= -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(fc >= 0) 
		{
			y--;
			ddF_y += 2;
			fc += ddF_y;
		}
		x++;
		ddF_x += 2;
		fc += ddF_x;
		//printf("x%i y%i \n" ,x0 + x, y0 + y);
		yCur = y0 + y;
		xCur = x0+x;
		for( int setY = yCur; setY > -1*yCur; setY-- )
		{
			printf("x%i y%i \n" ,x0, setY);
			//set.y = setY;
			//v.push_back( set );
		}
		//printf("x%i y%i \n" ,x0 + y, y0 + x);
		xCur = x0+y;
		for( int setX = xCur; setX > half; setX--)
		{
			printf("x%i y%i \n" ,setX, y0+x);
			printf("x%i y%i \n" ,setX, y0-x);
		}

		xCur = x0-y;
		for( int setX = xCur; setX < -1*half; setX++)
		{
			printf("x%i y%i \n" ,setX, y0+x);
			printf("x%i y%i \n" ,setX, y0-x);
		}
	}
	
}
*/
//old or unused functions
/*

void writePNG_ol( FILE* f, int** row_motion, ImageAttr* attr_bkg )
{
	int height = attr_bkg->imageHeight;
	int width = attr_bkg->imageWidth;
	
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

	const png_bytep* row_ptr_bkg = attr_bkg->row_pointers;
	for( int y = 0; y < height; y++ )
	{
		png_byte* row = row_pointers[y];
		int* col_motion = row_motion[y];
		const png_bytep col_ptr_bkg = row_ptr_bkg[y];

		for( int x = 0; x < width ; x++ )
		{
			png_byte* ptr = &(row[x*3]);
			if( col_motion[x] == 0 )
			{
				ptr[0] = *(col_ptr_bkg + 3*x + 0);
				ptr[1] = *(col_ptr_bkg + 3*x + 1);
				ptr[2] = *(col_ptr_bkg + 3*x + 2);
			}
			else
			{
				//fills in motion pixels with black (0,0,0)
				ptr[0] = 0;
				ptr[1] = 0;
				ptr[2] = 0;
			}
		}
	}

	png_write_image( png_ptr, row_pointers );
	png_write_end( png_ptr, NULL );

	for( int y = 0; y < height; y++ )
		free( row_pointers[y] );
	free( row_pointers );
	row_pointers = NULL;
}

void writePNG_bw( FILE* f, int** row_motion, int height , int width )
{

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

	for( int y = 0; y < height; y++ )
	{
		png_byte* row = row_pointers[y];
		int* col_motion = row_motion[y];

		for( int x = 0; x < width ; x++ )
		{
			png_byte* ptr = &(row[x*3]);
			if( col_motion[x] == 0 )
			{
				ptr[0] = 255;
				ptr[1] = 255;
				ptr[2] = 255;
			}
			
			else if( col_motion[x] == 1 )
			{
				ptr[0] = 255;
				ptr[1] = 0;
				ptr[2] = 0;
			}
			else if( col_motion[x] == 2 )
			{
				ptr[0] = 0;
				ptr[1] = 255;
				ptr[2] = 0;
			}
			
			else
			{
				ptr[0] = 0;
				ptr[1] = 0;
				ptr[2] = 0;
			}
		}
	}

	png_write_image( png_ptr, row_pointers );
	png_write_end( png_ptr, NULL );

	for( int y = 0; y < height; y++ )
		free( row_pointers[y] );
	free( row_pointers );
	row_pointers = NULL;
}
*/