#ifndef _ALBUM_H_
#define _ALBUM_H_

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>

extern int *plcd,fd0;
int draw_pic(char *pic_ops,int x,int y);
void draw_point(int x,int y,int color);

#endif