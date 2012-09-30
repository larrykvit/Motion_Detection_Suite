/*

#include "stdafx.h"
#include "imageControl.h"


ImageControl::ImageControl()
{
	//currently not needed
}

int ImageControl::openImage( FILE* f, ImageAttr* attr )
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
/*
	m_wndView.mr.setDim( width, height );
	m_wndView.mg.setDim( m_wndView.mr.cx(), m_wndView.mr.cy() );
	m_wndView.mb.setDim( m_wndView.mr.cx(), m_wndView.mr.cy() );

	for( unsigned y = 0; y < height; y++ )
	{
		const png_byte* row = row_pointers[y];
		for( unsigned x = 0; x < width ; x++ )
		{
			const png_byte* ptr = &(row[x*3]);
			m_wndView.mr.set( x, y, ptr[0] );
			m_wndView.mg.set( x, y, ptr[1] );
			m_wndView.mb.set( x, y, ptr[2] );
		}
	}

	fclose( f );

	//sets the atributes of the image to a struct
	attr->row_pointers = row_pointers;
	attr->imageWidth = width;
	attr->imageHeight = height;

//	for( unsigned y = 0; y < height; y++ )
//		free( row_pointers[y] );
//	free( row_pointers );

	return 0;// return 0 on succes
}
*/