/*
* A simple libpng example program
* http://zarb.org/~gc/html/libpng.html
*
* Modified by Yoshimasa Niwa to make it much simpler
* and support all defined color_type.
*
* To build, use the next instruction on OS X.
* $ brew install libpng
* $ clang -lz -lpng15 libpng_test.c
*
* Copyright 2002-2010 Guillaume Cottenceau.
*
* This software may be freely redistributed under the terms
* of the X11 license.
*
*/
#pragma once

#include <png.h>

class PngIO
{
public:
    PngIO();
    ~PngIO();

    void read_png_file(char * filename);
    void write_png_file(char * filename);
    void process_png_file();
    png_bytep * get_png_row_pointers(void);

    void combine_png_rgb_alpha(const PngIO & alpha);

private:
    int mWidth;
    int mHeight;
    png_byte mColor_type;
    png_byte mBit_depth;
    png_bytep * mRow_pointers;
};
