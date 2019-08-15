#include "bmp.h"

//单像素点上色
void draw_point(int x,int y,int color)
{
	*(plcd + 800*y + x) = color;
}

//显示图片
int draw_pic(char *pic_ops,int x,int y)
{
	int fd_pic = open(pic_ops,O_RDONLY);
	if(-1 == fd_pic)
	{
		printf("Open error !\n");
		return -1;
	}

	//把文件光标的位置定位到离开头0x12个字节处 读取宽度
	lseek(fd_pic,0x12,SEEK_SET);
	char buf1[4]; 
	read(fd_pic,buf1,4);
	//获取图片宽度
	int width = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24);
	
	lseek(fd_pic,0x16,SEEK_SET);
	read(fd_pic,buf1,4);
	//获取图片高度
	int height = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24); 
	
	//显示图片宽度高度
	//printf("width:%d\theight:%d\n",width,height);
	
	//判断图片是否过大
	if(((width + x)> 800)||((y + height)> 480))
	{
		printf("pic input error\n");
		return -1;
	}
	
	//读取图片大小
	int SIZE_OF_PIC =  width * height *3 + 54 ;
	//printf("size_of_pic = %d\n",SIZE_OF_PIC);
	unsigned char buf[SIZE_OF_PIC];
	lseek(fd_pic,0,SEEK_SET);
	read(fd_pic,buf,SIZE_OF_PIC);
	
	//获取像素数组的地址偏移
	char buf3[4];
	lseek(fd_pic,0x0A,SEEK_SET);
	read(fd_pic,buf3,4);
	int bit_offset = (buf3[0] & 0xff) |
		((buf3[1] & 0xff) << 8) | 
		((buf3[2] & 0xff) << 16) | 
		((buf3[3] & 0xff) << 24);
	//printf("bit_offset:%d\n",bit_offset);
	
	int color;
	int w,h,i = 0;
	unsigned char r,g,b;
	for(h = 0;h < height;h++)
	{
		for(w = 0;w < width;w++)
		{
			b = buf[bit_offset + i++];
			g = buf[bit_offset + i++];
			r = buf[bit_offset + i++];
			color = ((r << 16)|(g << 8)|(b));
			//int dy = hight > 0?((y0+hight-1)-y):(y0+y);
			draw_point((w + x),((y + height - 1)- h),color);
		}
	}
	close(fd_pic);
	return 0;
}

//延迟显示图片
int delay_draw_pic(char *pic_ops,int x,int y)
{
	int fd_pic = open(pic_ops,O_RDONLY);
	if(-1 == fd_pic)
	{
		printf("Open error !\n");
		return -1;
	}

	//把文件光标的位置定位到离开头0x12个字节处 读取宽度
	lseek(fd_pic,0x12,SEEK_SET);
	char buf1[4]; 
	read(fd_pic,buf1,4);
	//获取图片宽度
	int width = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24);
	
	lseek(fd_pic,0x16,SEEK_SET);
	read(fd_pic,buf1,4);
	//获取图片高度
	int height = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24); 
	
	//显示图片宽度高度
	//printf("width:%d\theight:%d\n",width,height);
	
	//判断图片是否过大
	if(((width + x)> 800)||((y + height)> 480))
	{
		printf("pic input error\n");
		return -1;
	}
	
	//读取图片大小
	int SIZE_OF_PIC =  width * height *3 + 54 ;
	//printf("size_of_pic = %d\n",SIZE_OF_PIC);
	unsigned char buf[SIZE_OF_PIC];
	lseek(fd_pic,0,SEEK_SET);
	read(fd_pic,buf,SIZE_OF_PIC);
	
	//获取像素数组的地址偏移
	char buf3[4];
	lseek(fd_pic,0x0A,SEEK_SET);
	read(fd_pic,buf3,4);
	int bit_offset = (buf3[0] & 0xff) |
		((buf3[1] & 0xff) << 8) | 
		((buf3[2] & 0xff) << 16) | 
		((buf3[3] & 0xff) << 24);
	//printf("bit_offset:%d\n",bit_offset);
	
	int color;
	int w,h,i = 0;
	unsigned char r,g,b;
	for(w = 0;w < width;w++)
	{
		for(h = 0;h < height;h++)
		{
			//b = buf[bit_offset + i++];
			//g = buf[bit_offset + i++];
			//r = buf[bit_offset + i++];

			b = buf[((height-1-h)*width+w)*3+54];   
			g = buf[((height-1-h)*width+w)*3+55];		
			r = buf[((height-1-h)*width+w)*3+56];

			color = ((r << 16)|(g << 8)|(b));
			//int dy = hight > 0?((y0+hight-1)-y):(y0+y);
			draw_point((w + x),((y + height - 1)- h),color);
		}
		usleep(5000);
	}
	close(fd_pic);
	return 0;
}

//百叶窗显示图片
int blind_draw_pic(char* pic_ops,int x0,int y0)
{
	int fd_pic = open(pic_ops,O_RDONLY);
	if(-1 == fd_pic)
	{
		printf("Open error !\n");
		return -1;
	}

	//把文件光标的位置定位到离开头0x12个字节处 读取宽度
	lseek(fd_pic,0x12,SEEK_SET);
	char buf1[4]; 
	read(fd_pic,buf1,4);
	//获取图片宽度
	int pic_width = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24);
	printf("%d\n",pic_width);
	
	lseek(fd_pic,0x16,SEEK_SET);
	read(fd_pic,buf1,4);
	//获取图片高度
	int pic_height = (buf1[0] & 0xff) |
			((buf1[1] & 0xff) << 8) | 
			((buf1[2] & 0xff) << 16) | 
			((buf1[3] & 0xff) << 24); 
	printf("%d\n",pic_height);
	
	//判断图片是否过大
	if(((pic_width + x0)> 800)||((y0 + pic_height)> 480))
	{
		printf("pic input error\n");
		return -1;
	}
	
	//读取图片大小
	int SIZE_OF_PIC =  pic_width * pic_height *3 + 56;
	printf("size_of_pic = %d\n",SIZE_OF_PIC);
	unsigned char buf[SIZE_OF_PIC];
	//lseek(fd_pic,54,SEEK_SET);
	read(fd_pic,buf,SIZE_OF_PIC);
	
	//获取像素数组的地址偏移
	char buf3[4];
	lseek(fd_pic,0x0A,SEEK_SET);
	read(fd_pic,buf3,4);
	int bit_offset = (buf3[0] & 0xff) |
		((buf3[1] & 0xff) << 8) | 
		((buf3[2] & 0xff) << 16) | 
		((buf3[3] & 0xff) << 24);
	printf("bit_offset:%d\n",bit_offset);
	int color;
	int x = 0,y = 0;
	unsigned char r,g,b;
	
	int part_num = 6;
	int blind_len=pic_height/part_num;	
	while(1)
	{
		usleep(1);
		//printf("y=%d blind_len=%d pic_width=%d pic_height=%d\n",y,blind_len,pic_width,pic_height);
		for(x=0;x<pic_width;x++)
		{
			b = buf[((pic_height-1-y)*pic_width+x)*3+54];   
			g = buf[((pic_height-1-y)*pic_width+x)*3+55];		
			r = buf[((pic_height-1-y)*pic_width+x)*3+56];
			color = (r<<16) | (g<<8) | b;
			*(plcd+(x+x0)+(y+y0)*800)=color;
		}
		y=y+blind_len;
		
		if(y==blind_len+479)
		{
			return 0;
		}
		else if(y>479)
		{
			while(y-blind_len>0)
			{
				y=y-blind_len;
			}
			y++;
		}
	}
	close(fd_pic);
	return 0;
}