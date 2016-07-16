

#include "PngIO.h"
#include <stdio.h>
#include <stdlib.h>

PngIO::PngIO()
    :mWidth(0)
    , mHeight(0)
    , mRow_pointers(NULL)
{
    //
}
PngIO::~PngIO()
{
    //reserved
}

void PngIO::read_png_file(char * filename)
{
    FILE * fp = fopen(filename, "rb");
    if (nullptr == fp)abort();

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)abort();

    png_infop info = png_create_info_struct(png);
    if (!info)abort();

    if (setjmp(png_jmpbuf(png)))abort();

    png_init_io(png, fp);
    png_read_info(png, info);

    mWidth = png_get_image_width(png, info);
    mHeight = png_get_image_height(png, info);
    mColor_type = png_get_color_type(png, info);
    mBit_depth = png_get_bit_depth(png, info);

    if (16 == mBit_depth)
        png_set_strip_16(png);
    if (mColor_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (PNG_COLOR_TYPE_GRAY == mColor_type && mBit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (PNG_COLOR_TYPE_RGB == mColor_type
        || PNG_COLOR_TYPE_GRAY == mColor_type
        || PNG_COLOR_TYPE_PALETTE == mColor_type)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (PNG_COLOR_TYPE_GRAY == mColor_type
        || PNG_COLOR_TYPE_GRAY_ALPHA == mColor_type)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    mRow_pointers = (png_bytep *)malloc(sizeof(png_bytep) * mHeight);
    for (int y = 0; y < mHeight; ++y)
        mRow_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));

    png_read_image(png, mRow_pointers);

    fclose(fp);
    fp = nullptr;
}

void PngIO::write_png_file(char * filename)
{
    FILE * fp = fopen(filename, "wb");
    if (!fp)abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)abort();

    png_infop info = png_create_info_struct(png);
    if (!info)abort();

    if (setjmp(png_jmpbuf(png)))abort();

    png_init_io(png, fp);

    png_set_IHDR(
              png
            , info
            , mWidth
            , mHeight
            , 8
            , PNG_COLOR_TYPE_RGBA
            , PNG_INTERLACE_NONE
            , PNG_COMPRESSION_TYPE_DEFAULT
            , PNG_FILTER_TYPE_DEFAULT
        );
    png_write_info(png, info);

    // to remove the alpha channel for PNG_color_type_rgb format.
    // use png_set_filler()
    // png_set_filler(png, 0, png_filler_after)
    png_write_image(png, mRow_pointers);
    png_write_end(png, NULL);

    for (int y = 0; y < mHeight; ++y)
        free(mRow_pointers[y]);
    free(mRow_pointers);
    mRow_pointers = nullptr;

    fclose(fp);
    fp = nullptr;
}

void PngIO::process_png_file()
{
    for (int y = 0; y < mHeight; ++y)
    {
        png_bytep row = mRow_pointers[y];
        for (int x = 0; x < mWidth; ++x)
        {
            png_bytep pixel = &(row[x * 4]);
            unsigned char temp;
            temp = pixel[0];
            pixel[0] = pixel[1];
            pixel[1] = pixel[2];
            pixel[2] = temp;
            // access each pixel here goes...
            //     red-pixel    pixel[0]
            //     green-pixel  pixel[1]
            //     blue-pixel   pixel[2]
            //     alpha-pixel  pixel[3]
        }
    }
}

void PngIO::combine_png_rgb_alpha(const PngIO & alpha)//(png_bytep* aRowpointers)
{
    ////need check..
    for (int y = 0; y < mHeight; ++y)
    {
        png_bytep rowRGB = mRow_pointers[y];
        png_bytep rowALPHA = alpha.mRow_pointers[y];////is this legal ?
        for (int x = 0; x < mWidth; ++x)
        {
            png_bytep plRGB = &(rowRGB[x * 4]);
            png_bytep plAPA = &(rowALPHA[x * 4]);
            plRGB[3] = plAPA[3];
        }
    }
}

png_bytep * PngIO::get_png_row_pointers(void)
{
    return mRow_pointers;
}
