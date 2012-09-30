/*
#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#define ASSERT(a) if(!(a)) throw( __LINE__ )

struct ImageAttr
{
	png_bytep* row_pointers;
	int imageWidth, imageHeight;
};

class ImageControl
{
public:
	ImageControl();
	int openImage( FILE* f, ImageAttr* attr );
private:
	
};

#endif
*/