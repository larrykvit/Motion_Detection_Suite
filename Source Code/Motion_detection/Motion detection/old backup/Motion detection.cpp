
// Motion detection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct ImageAttr
{
	png_bytep* row_pointers;
	int imageWidth, imageHeight;
};

struct Colour
{
	int R, G, B;
};

Colour fillIn = { 0, 0, 0 };
Colour border = {255, 0, 255 };
Colour white = { 255, 255, 255 };

bool equalColour( png_bytep col_ptr_img, png_bytep col_ptr_bkg, int x, int threshold );//returns true if the two colours are within a certain threshold
void openImage( FILE* f, ImageAttr* attr );//opens image
void writePNG( FILE* f, ImageAttr* attr_img );//saves the given image
void compareImg( ImageAttr* attr_bkg, ImageAttr* attr_img, int** row_motion, int threshold );//compares the two given images and fills up the motion array
bool affirmImageSize( ImageAttr* attr_bg, ImageAttr* attr_img );//returns true if the images are the same size
bool affirmImageSizeAll( ImageAttr* attr_bg, const vector< ImageAttr >& attr_img);//returns true if the images are the same size
void getTargets( int imageHeight, int imageWidth, int** row_motion, vector< Target >* target );//searchs the motion image and fills the vector with target objects
void colourImage( ImageAttr* attr_img, const vector< Target >& target );//colours in targets and boxes them
void fillColour( png_bytep* row_ptr_img, int x, int y, Colour* colour );//fills in the pixel x,y with the specified colour
void fillSearchArea( int Xori, int Yori, int searchRadius, vector< Point >* searchArea );

//old or unused functions
/*
void writePNG_ol( FILE* f, int** row_motion, ImageAttr* attr_bkg );//overlay
void writePNG_bw( FILE* f, int** row_motion, int height , int width );//black white
*/

/*
void func( const vector< int >& x );
void func2( vector< int >* x );
vector< int > v;
func( v );
func2( &v ); 
func2( 0 ); 
*/

int _tmain(int argc, _TCHAR* argv[])
{
	const int numOfImages = 2;
	const int threshold = 1;
	const int searchRadius = 12;

	ImageAttr imageAttrBG;//bg image is seperated from array of images for ease of use
	
	vector< ImageAttr > imageAttr( numOfImages );//ImageAttr imageAttr[numImages];

	string filePathOpen(".\\images\\set_cur\\");
	string filePathSave(".\\images\\!motion\\");
	string fileDir;

	fileDir = filePathOpen + "imagebg.png";

	FILE* fBG = fopen( fileDir.c_str() , "rb" );
	
	vector< FILE* > f;//vector of pointers to the files to be opened

	vector< FILE* > fSave;//vector of pointers to the files to be opened

	openImage( fBG, &imageAttrBG );//opens image and sets its attributes to the struct - png_bytep*, imageWidth, imageHeight
	
	//affirmImageSize returns true if all the images have the same size as the bkg image (imageAttr[0])
	//if( !affirmImageSizeAll( &imageAttrBG, imageAttr ) )
		//return 1;//TO DO: print that there are image(s) of diffrent size and continue
	
	//imageWidth and Height are same for all images, loops will use imageWidth instead of imageAttr[0].imageWidth
	int imageHeight = imageAttrBG.imageHeight;
	int imageWidth = imageAttrBG.imageWidth;

	vector< vector< Target > > target( numOfImages );//2d vector; [frame number][targetNumber] 

	//creates a 2d array for motion
	int** row_motion = (int**) malloc( imageHeight * sizeof( *row_motion ) );
	for( int y = 0; y < imageHeight; y++ )
		row_motion[y] = (int*) malloc( imageWidth * sizeof( *row_motion[y] )  );

	for( int frame = 0; frame < numOfImages; frame++)
	{
		
		//concatinates the file path and image number to be opened
		char numstr[21]; //enough to hold all numbers up to 64-bits
		fileDir = filePathOpen + "image" + itoa( frame, numstr, 10 ) + ".png";
		
		f.push_back( fopen( fileDir.c_str() , "rb" ));//create FILE* f[frame]

		openImage( f[frame], &imageAttr[frame] );//opens image and sets its attributes to the struct

		if ( !affirmImageSize( &imageAttrBG, &imageAttr[frame] ) )//checks if the image is the same size as the imageBG
			return 1;//exits program

		//compares the two images given and writes the motion to the 2d array given
		compareImg( &imageAttrBG, &imageAttr[frame], row_motion, threshold );
	
		//searchs the motion image and fills the vector with target objects
		getTargets( imageHeight, imageWidth, row_motion, &target[frame] );//fills the vector of target objects with the targets

		//void colourImage( ImageAttr* attr_img, int** row_motion, const vector< Target >& target )
		colourImage( &imageAttr[frame], target[frame] );

		/*
		//code for tracking targets goes here
		if( frame > 0 )//every frame but initial
		{
			vector< Point > searchArea;
			//fillSearchArea( int Xori, int Yori, int searchRadius, vecotr< Point >* searchArea ); 
		}
		*/

		//creates fSave[frame]
		fileDir = filePathSave + "image_m" + itoa( frame, numstr, 10 ) + ".png";
		fSave.push_back( fopen( fileDir.c_str() , "wb" ));

		writePNG( fSave[frame], &imageAttr[frame] );//save the imageAttr[frame] to f[frame]

		printf("%i\n", target[frame].size() );//prints the number of targets in frame
	}
	return 0;
}

bool equalColour( png_bytep col_ptr_img, png_bytep col_ptr_bkg, int x, int threshold )//returns true if the two colours are within a certain threshold
{
	//the pointer has rgb values consecutively, thus x*3 + (rgb)
	bool a =	( *( col_ptr_img + 3*x + 0 ) >= (*( col_ptr_bkg + 3*x + 0 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 0 ) <= (*( col_ptr_bkg + 3*x + 0 ) ) + threshold ) &&
				( *( col_ptr_img + 3*x + 1 ) >= (*( col_ptr_bkg + 3*x + 1 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 1 ) <= (*( col_ptr_bkg + 3*x + 1 ) ) + threshold ) &&
				( *( col_ptr_img + 3*x + 3 ) >= (*( col_ptr_bkg + 3*x + 3 ) ) - threshold ) && ( *( col_ptr_img + 3*x + 3 ) <= (*( col_ptr_bkg + 3*x + 3 ) ) + threshold ) ;
	return a;
}

void compareImg( ImageAttr* attr_bkg, ImageAttr* attr_img, int** row_motion, int threshold )
{
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
			
			/*if( ( *( col_ptr_img + 3*x + 0 ) == *( col_ptr_bkg + 3*x + 0 ) ) ||
				( *( col_ptr_img + 3*x + 1 ) == *( col_ptr_bkg + 3*x + 1 ) ) ||
				( *( col_ptr_img + 3*x + 2 ) == *( col_ptr_bkg + 3*x + 2 ) ) )
			*/
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

//	for( unsigned y = 0; y < height; y++ )
//		free( row_pointers[y] );
//	free( row_pointers );
}

void writePNG( FILE* f, ImageAttr* attr_img )
{
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
	bool check;
	
	check = attr_bg->imageHeight == attr_img->imageHeight && attr_bg->imageWidth == attr_img->imageWidth;
	
	return check;
}

bool affirmImageSizeAll( ImageAttr* attr_bg, const vector< ImageAttr >& attr_img)
{
	bool check = true;
	for( vector< ImageAttr >::size_type i = 0; i < attr_img.size() ; i++ )
	{
		check = check && attr_bg->imageHeight == attr_img[i].imageHeight && attr_bg->imageWidth == attr_img[i].imageWidth;
	}
	
	return check;
}

void getTargets( int imageHeight, int imageWidth, int** row_motion, vector< Target >* target )
{
	int numOfTargets = 0;
	for( int y = 0; y < imageHeight; y++ )
	{
		
		for( int x = 0; x < imageWidth; x++ )
		{
			if( row_motion[y][x] == -1 )
			{
				numOfTargets++;
				vector< Point > pointList;
				Point p;
				
				p.x = x;
				p.y = y;
				pointList.push_back (p);
				unsigned int index = 0;
				
				row_motion[p.y][p.x] = numOfTargets;

				int curX = x;
				int curY = y;

				while( true )
				{
					if( ((curX-1) >= 0) )
					{
						p.x = curX-1;
					
						if( (curY-1) >= 0 && (row_motion[curY-1][curX-1] == -1) )
						{
							p.y = curY-1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						if( (row_motion[curY][curX-1] == -1) )
						{
							p.y = curY;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
						if( (curY+1) < imageHeight && (row_motion[curY+1][curX-1] == -1) )
						{
							p.y = curY+1;
							pointList.push_back (p);
							row_motion[p.y][p.x] = numOfTargets;
						}
					}

					//curX
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
					index++;
					if(index == pointList.size() )
					{
						break;
					}
					
					p = pointList[index];
					curX = p.x;
					curY = p.y;
				}
				//pointList contains all the points of the object now
				target->push_back( pointList );
			}

		}
	}
}

void colourImage( ImageAttr* attr_img, const vector< Target >& target )
{
	
	png_bytep* row_ptr_img = attr_img->row_pointers;

	for( vector< Target >::size_type i = 0; i < target.size(); i++ )//for every target
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
	*( row_ptr_img[y] + 3*x + 0 ) = colour->R;
	*( row_ptr_img[y] + 3*x + 1 ) = colour->G;
	*( row_ptr_img[y] + 3*x + 2 ) = colour->B;
}
/*
void fillSearchArea( int Xori, int Yori, int searchRadius, vector< Point >* searchArea )
{
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