#include "start.h"

struct node *pass;
char * pass_pic[5];
int *plcd,fd0,fd_screen;

void direction(struct node *pass)
{
	//printf("come into direction\n");
	//初始化值 
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
    while(1)
	{
		read(fd0,&ev,sizeof(ev));
		//分析读到的数据
		//判断当前的事件是否为触摸事件
		if(ev.type == EV_ABS)
		{
			if(ev.code == ABS_X)
			{
				if(x1 == -1)
				{
					x1 = ev.value;
				}
				x2 = ev.value;
			}
			else if(ev.code == ABS_Y)
			{
				if(y1 == -1)
				{
					y1 = ev.value;
				}
				y2 = ev.value;
			}
		}
		//手指弹起来
		if(ev.type == EV_KEY && 
			ev.code == BTN_TOUCH &&
			ev.value == 0)
		{
			if(pass->psw_flag == 1)
            {
                if( x1 > 555 && x1< 640 && y1 > 186 && y1 < 298)//数字 0范围
                {
                    pass->pass_right ++;
                    if (pass->pass_right == 4)
                    {
                        draw_pic(pass_pic[4],0,0);
						usleep(300000);
						pass->psw_flag = 2;
                    }
                }
                else if (x1 > 279 && x1 < 551 && y1 > 57 && y1 < 471)//在其他数字范围内
                {
                    pass->pass_fault ++;
                }
            }
			break;
		}	
	}
    //判断是否已经输了4次密码
	if(((pass->pass_right + pass->pass_fault)== 4)&&(pass->pass_right != 4))
	{
		pass->pass_right = 0;
		pass->pass_fault = 0;
	}
	if(pass->psw_flag != 0)
	{
		//显示密码界面i
    	draw_pic(pass_pic[pass->pass_right + pass->pass_fault],0,0);
	}
    
	//防止误触
	if(x1 != -1)
	{
		if((abs(x2 - x1)<30)&&(abs(y2 - y1)<30))
		{
			printf("touch\n");
			return;
		}
	}
	//打印滑动的方向
	if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
	{
		printf("up\n");
	}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
	{
		printf("down\n");
	}else if(((x1 > 700) && (x1 < 800)) && (abs(y2 - y1) < abs(x2 - x1) && x2 < x1))
	{
		if(pass->psw_flag == 0)
		{
			//显示面容界面
			draw_pic("./oprate_pic/lock_face.bmp",0,0);
			sleep(1);
			//显示密码界面
			draw_pic(pass_pic[0],0,0);
			pass->psw_flag = 1;
		}
		else if(pass->psw_flag == 1)
		{
			//显示锁屏界面
			draw_pic("./oprate_pic/lock_0.bmp",0,0);
			pass->pass_right = 0;
			pass->pass_fault = 0;
		}
		
        printf("left\n");
	}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
	{
		printf("right\n");
	}
    return;
}

//开机及进入主界面
void start(void)
{
    //打开触屏文件
	fd0 = open("/dev/input/event0",O_RDWR|O_NONBLOCK);
	if(fd0 == -1)
	{
		printf("open event0 error\n");
		return ;
	}
	//打开显示屏文件
	fd_screen = open("/dev/fb0",O_RDWR);
	if(-1 == fd_screen)
	{
		printf("Open machine error !\n");
		return ;
	}
	//映射	
	plcd = mmap(NULL,SIZE_OF_LCD,
	PROT_WRITE | PROT_READ,MAP_SHARED,fd_screen,0);
	//映射成功检测
	if(MAP_FAILED == plcd)
	{
		printf("Map lcd failed !\n");
		return ;
	}

    //显示开机图片
    draw_pic("./oprate_pic/start.bmp",0,0);
    sleep(3);
    //显示锁屏界面
    draw_pic("./oprate_pic/lock_0.bmp",0,0);

	pass_pic[0] = "./oprate_pic/lock_1.bmp";
	pass_pic[1] = "./oprate_pic/lock_2.bmp";
	pass_pic[2] = "./oprate_pic/lock_3.bmp";
	pass_pic[3] = "./oprate_pic/lock_4.bmp";
	pass_pic[4] = "./oprate_pic/lock_5.bmp";

	music_statu = (struct music_statu*)malloc(sizeof(*music_statu));
	music_statu->music_num = 0;
	music_statu->play_pause = -1;

	pass = (struct node *)malloc(sizeof(*pass));
	pass->pass_fault = pass->pass_right = pass->psw_flag = 0;
    while(pass->psw_flag != 2)
    {
        direction(pass);
    }
	usleep(500000);
	album_in_flag = 0;
	return ;
}