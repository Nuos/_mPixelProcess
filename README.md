
# Question
How to combine ALPHA channel extracted from the PNG file into the RGB-pixel of the same source file?


# context
In ETC1 algorithm texture compression
, the alpha channel unhandled will be saved in another single PNG file with the same size (width, height, and block size)
, then after ETC1 compression there are two PNG files, one with rgb pixel, and another aaa pxiel.

However the question is now, how to recover the source PNG file ?


# Reference
the commited file mainly reference to the source file https://gist.github.com/niw/5963798#file-libpng_test-c-L68


# Requirement
 <Png.h> library
