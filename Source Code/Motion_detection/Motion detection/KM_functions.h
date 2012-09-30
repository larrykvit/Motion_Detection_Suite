/*
K&M softaware
Written by Larry Kvitnevski and Jack Moseley

Set of functions used by the motion detection program
*/

#ifndef KM_FUNCTIONS_H
#define KM_FUNCTIONS_H

//attributes of an image
struct ImageAttr
{
	//pointer to image
	png_bytep* row_pointers;
	//image's width and height
	int imageWidth, imageHeight;
};

//used for simplicity when colouring images
struct Colour
{
	int R, G, B;
};

//parameters of motion detaction
struct Parameter
{
	string filePathOpen, filePathSave, imgName;
	int imgStartNum, imgEndNum, colourThreshold, searchRadius, minTargetSize, numOfTargets;
	double sizeThreshold;
};

//returns true if the two colours are within a certain threshold
bool equalColour( png_bytep col_ptr_img, png_bytep col_ptr_bkg, int x, int threshold );

//opens image (.png) and returns a pointer to the image in the struct
void openImage( FILE* f, ImageAttr* attr );

//saves the given image as a png file
void writePNG( FILE* f, ImageAttr* attr_img );

//compares the two given images and fills up the motion array
void compareImg( ImageAttr* attr_bkg, ImageAttr* attr_img, int** row_motion, int threshold );

//returns true if the images are the same size
bool affirmImageSize( ImageAttr* attr_bg, ImageAttr* attr_img );

//returns true if the images are the same size
bool affirmImageSizeAll( ImageAttr* attr_bg, const vector< ImageAttr >& attr_img);

//searchs the motion image and fills the vector with target objects
void getTargets( int imageHeight, int imageWidth, int** row_motion, vector< Target >* target, const int minTargetSize );

//colours in targets and boxes them
void colourImage( ImageAttr* attr_img, const vector< Target >& target, const int numOfTargets );

//fills in the pixel x,y with the specified colour
void fillColour( png_bytep* row_ptr_img, int x, int y, Colour* colour );

//draws line between x0,y0 and x1,y1 on the image pointer given
void drawLine( int x0, int y0, int x1, int y1, png_bytep* row_ptr_img );

//finds related targets for all target[frame] targets
void getRelatedTargets( vector< Target >* curTarget, vector< Target >* prevTarget, const double sizeThreshold, const int searchRadius);

//recursively draws lines between the related targets throught the frames
void colourLines( const vector< vector<Target> >& target, int frameNum, ImageAttr* attr_img, const int numOfTargets );

//returns true if x&&y are within the range
bool withinRange( int x, int y, int searchRadius );

//returns true if parameters enterd are within the size threshold
bool withinSize( int size1, int size2, double sizeThreshold );

//returns absolute value of x
int abs( int x );

//prints out the related targets of the given frame
void outputRelatedTargets( const vector<Target>& target, const int numOfTargets );

//opens input & parameters .txt (creates parameters.txt if not present) and sets the parameter struct
bool fileOpen(Parameter* para);

//save the information about the related targets to info.txt
void writeInfo( const vector< vector<Target> >& target, const int numOfImages, const int numOfTargets );

#endif