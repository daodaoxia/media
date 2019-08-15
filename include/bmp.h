#ifndef _BMP_H_
#define _BMP_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


#define SIZE_OF_LCD 800*480*4
extern int *plcd,fd_screen;

#endif
