
# _mPixelProcess
How to combine ALPHA channel extracted from the PNG file into the RGB-pixel of the same source file?

In ETC1 algorithm texture compression
, the alpha channel unhandled will be saved in another single PNG file with the same size (width, height, and block size)
, then after ETC1 there are two PNG files..

However the question is now, how to recover the source PNG file ?

#
the commited file mainly reference to the source file https://gist.github.com/niw/5963798#file-libpng_test-c-L68

#
Requirement
 <Png.h> library
