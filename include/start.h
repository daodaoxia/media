#ifndef _START_H_
#define _START_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SIZE_OF_LCD 800*480*4

extern int *plcd,fd0,album_in_flag;
struct node
{
	int pass_right;
	int pass_fault;
	int psw_flag;
};

struct music_statu
{
	int music_num;
	int play_pause;
};

extern struct music_statu *music_statu;

int draw_pic(char *pic_ops,int x,int y);
void draw_point(int x,int y,int color);

#endif 