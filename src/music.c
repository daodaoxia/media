#include"music.h"

struct music_statu
{
	int music_num;
	int play_pause;
};

struct music_statu *music_statu;

void* create_share_memory(int n)
{
	key_t key = ftok(PROJ_FILE,PROJ_NUM);
	int shm_id = shmget(key,n*4096,IPC_CREAT|0664);
	if(shm_id == -1)  //如果创建失败
	{
		if(errno == EEXIST) //失败的原因是已经存在该消息队列
		{
			shm_id = msgget(key,0,0); //打开消息队列
		}
		perror("shmget error\n");
		exit(0);
	}
	char * p = shmat(shm_id,NULL,0);
	return (void*)p;
	//return shm_id;
}

void free_share_memory(void*p)
{
	shmdt(p);
}

void* music_play(void* arg)
{
	char *p = (char*)create_share_memory(4);
	stpcpy(p,"play");
	char music[3][10] = {"hxgl.mp3","mhtc.mp3","k.mp3"};
	//系统控制字符数组
	unsigned char cmd0[32];
	int status = 0;
	int music_num = music_statu-> music_num;
	int play_pause = music_statu->play_pause;
	pid_t pid1 = fork();
	if(pid1 > 0)
	{
		while(1)
		{
			if(strcmp(p,"over") == 0)
			{
				wait(&status);
				if(music_num == 2)
				{
					music_num = -1;
				}
				music_statu->music_num = ++ music_num;
				pid1 = fork();
				if(pid1 > 0)
				{
					stpcpy(p,"play");
				}
				else if(pid1 == 0)
				{
					execlp("madplay","madplay",music[music_num],NULL);
					stpcpy(p,"over");
					return;
				}
			}
			else if((music_statu->play_pause == 1) 
			&& (music_statu->play_pause != play_pause))
			{
				play_pause = music_statu->play_pause;
				//播放音乐
				sprintf(cmd0,"kill -18 %d",pid1);
				system(cmd0);
			}
			else if((music_statu->play_pause == 0) 
			&& (music_statu->play_pause != play_pause))
			{
				play_pause = music_statu->play_pause;
				//暂停音乐
				sprintf(cmd0,"kill -19 %d",pid1);
				system(cmd0);
			}
			else if(music_statu->music_num != music_num)
			{
				music_num = music_statu->music_num;
				//杀掉放歌进程
				sprintf(cmd0,"kill -9 %d",pid1);
				system(cmd0);
				wait(&status); //收pid1的"尸"
				pid1 = fork();
				if(pid1 == 0)
				{
					execlp("madplay","madplay",music[music_num],NULL);
					stpcpy(p,"over");
					return;
				}
			}
		}
		free_share_memory(p);
		pthread_exit(NULL);
	}
	else if(pid1 == 0)
	{
		execlp("madplay","madplay",music[music_num],NULL);
		stpcpy(p,"over");
	}
}

int music_touch(void)
{
    //初始化值 
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
	int back_flag = -1;
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
			break;
		}	
	}
    //防止误触
    if(x1 != -1)
    {
        if((abs(x2 - x1)<30)&&(abs(y2 - y1)<30))
        {
            if((x1 > 689 && x1 < 759) && (y1 > 300 && y1 < 360))//在上一首范围内
			{
				back_flag = 1;
			}
			if((x1 > 689 && x1 < 759) && (y1 > 112 && y1 < 178))//在下一首范围内
			{
				back_flag = 2;
			}
			if((x1 > 685 && x1 < 760) && (y1 > 192 && y1 < 289))//在播放、暂停范围内
			{
				back_flag = 3;
			}
			return back_flag;
        }
    }
	//打印滑动的方向
	if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
	{
		printf("up\n");
        back_flag = 1;
	}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
	{
		printf("down\n");
        back_flag = 2;
	}else if(((x1 > 700) && (x1 < 800)) &&(abs(y2-y1) < abs(x2-x1) && x2 < x1))
	{
		printf("left\n");
        back_flag = 0;
	}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
	{
		printf("right\n");
	}
    return back_flag;
}

void music(void)
{
	//显示加载卡片界面
	draw_pic("./oprate_pic/music_0.bmp",0,0);
	sleep(1);
	
	char music_bmp[2][3][50] = {
		"./music_pic/hxgl_pause.bmp",
        "./music_pic/mhtc_pause.bmp",
        "./music_pic/k_pause.bmp",
		"./music_pic/hxgl_play.bmp",
        "./music_pic/mhtc_play.bmp",
        "./music_pic/k_play.bmp"};
	
    //显示歌曲界面
    if(music_statu->play_pause == -1)
    {
        draw_pic(music_bmp[1][music_statu->music_num],0,0);
    }
    else
    {
        draw_pic(music_bmp[music_statu->play_pause][music_statu->music_num],0,0);
    }
	while(1)
	{
		int ret_tc = music_touch();
		switch(ret_tc)
		{
			case 1:	
				//上一首
				if(music_statu->music_num == 0)
				{
					music_statu->music_num = 3;
				}
				music_statu->music_num --;
				draw_pic(music_bmp[0][music_statu->music_num],0,0);
				break;
			case 2:
				//下一首
				music_statu->music_num ++;
				music_statu->music_num = music_statu->music_num % 3;
				draw_pic(music_bmp[0][music_statu->music_num],0,0);
				break;
			case 3: 
				//暂停、播放
				if(music_statu->play_pause == -1)
				{
					pthread_t tid; //用来保存线程号
					int ret;  //用来保存 int返回值

					ret = pthread_create(&tid, //地址,用来存储线程号
										 NULL,//线程属性 默认属性
										 music_play, //指定要执行的线程函数
										 NULL //传递给线程函数的参数
										 );
					if(0 != ret)
					{
						perror("pthread create error");
						return ;
					}
					pthread_detach(tid); //设置tid线程为 分离属性,该线程无需它人收尸
					music_statu->play_pause = 1;
                    draw_pic(music_bmp[0][music_statu->music_num],0,0);
				}
				else if(music_statu->play_pause == 0)
				{
					//继续进程
					music_statu->play_pause = 1;
                    draw_pic(music_bmp[0][music_statu->music_num],0,0);
				}
				else
				{
					//暂停进程
					music_statu->play_pause = 0;
                    draw_pic(music_bmp[1][music_statu->music_num],0,0);
				}
				break;
			case 0: 
				//上划 返回应用选择界面
				return;
			default:
				break;
		}
		sleep(1);
	}
	return;
}