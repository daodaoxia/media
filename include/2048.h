#ifndef _2048_H_
#define _2048_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/input.h>

int draw_pic(char *pic_ops,int x,int y);
void draw_point(int x,int y,int color);
int delay_draw_pic(char *pic_ops,int x,int y);

extern int *plcd,fd0;

#endif