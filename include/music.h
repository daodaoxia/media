#ifndef _MUSIC_H_
#define _MUSIC_H_
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>


#define PROJ_FILE "/home/shm"
#define PROJ_NUM 3333

extern int *plcd,fd0;

int draw_pic(char *pic_ops,int x,int y);
void draw_point(int x,int y,int color);

#endif