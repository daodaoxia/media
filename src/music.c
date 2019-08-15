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
	if(shm_id == -1)  //�������ʧ��
	{
		if(errno == EEXIST) //ʧ�ܵ�ԭ�����Ѿ����ڸ���Ϣ����
		{
			shm_id = msgget(key,0,0); //����Ϣ����
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
	//ϵͳ�����ַ�����
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
				//��������
				sprintf(cmd0,"kill -18 %d",pid1);
				system(cmd0);
			}
			else if((music_statu->play_pause == 0) 
			&& (music_statu->play_pause != play_pause))
			{
				play_pause = music_statu->play_pause;
				//��ͣ����
				sprintf(cmd0,"kill -19 %d",pid1);
				system(cmd0);
			}
			else if(music_statu->music_num != music_num)
			{
				music_num = music_statu->music_num;
				//ɱ���Ÿ����
				sprintf(cmd0,"kill -9 %d",pid1);
				system(cmd0);
				wait(&status); //��pid1��"ʬ"
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
    //��ʼ��ֵ 
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
	int back_flag = -1;
	while(1)
	{
		read(fd0,&ev,sizeof(ev));
		//��������������
		//�жϵ�ǰ���¼��Ƿ�Ϊ�����¼�
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

		//��ָ������
		if(ev.type == EV_KEY && 
			ev.code == BTN_TOUCH &&
			ev.value == 0)
		{
			break;
		}	
	}
    //��ֹ��
    if(x1 != -1)
    {
        if((abs(x2 - x1)<30)&&(abs(y2 - y1)<30))
        {
            if((x1 > 689 && x1 < 759) && (y1 > 300 && y1 < 360))//����һ�׷�Χ��
			{
				back_flag = 1;
			}
			if((x1 > 689 && x1 < 759) && (y1 > 112 && y1 < 178))//����һ�׷�Χ��
			{
				back_flag = 2;
			}
			if((x1 > 685 && x1 < 760) && (y1 > 192 && y1 < 289))//�ڲ��š���ͣ��Χ��
			{
				back_flag = 3;
			}
			return back_flag;
        }
    }
	//��ӡ�����ķ���
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
	//��ʾ���ؿ�Ƭ����
	draw_pic("./oprate_pic/music_0.bmp",0,0);
	sleep(1);
	
	char music_bmp[2][3][50] = {
		"./music_pic/hxgl_pause.bmp",
        "./music_pic/mhtc_pause.bmp",
        "./music_pic/k_pause.bmp",
		"./music_pic/hxgl_play.bmp",
        "./music_pic/mhtc_play.bmp",
        "./music_pic/k_play.bmp"};
	
    //��ʾ��������
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
				//��һ��
				if(music_statu->music_num == 0)
				{
					music_statu->music_num = 3;
				}
				music_statu->music_num --;
				draw_pic(music_bmp[0][music_statu->music_num],0,0);
				break;
			case 2:
				//��һ��
				music_statu->music_num ++;
				music_statu->music_num = music_statu->music_num % 3;
				draw_pic(music_bmp[0][music_statu->music_num],0,0);
				break;
			case 3: 
				//��ͣ������
				if(music_statu->play_pause == -1)
				{
					pthread_t tid; //���������̺߳�
					int ret;  //�������� int����ֵ

					ret = pthread_create(&tid, //��ַ,�����洢�̺߳�
										 NULL,//�߳����� Ĭ������
										 music_play, //ָ��Ҫִ�е��̺߳���
										 NULL //���ݸ��̺߳����Ĳ���
										 );
					if(0 != ret)
					{
						perror("pthread create error");
						return ;
					}
					pthread_detach(tid); //����tid�߳�Ϊ ��������,���߳�����������ʬ
					music_statu->play_pause = 1;
                    draw_pic(music_bmp[0][music_statu->music_num],0,0);
				}
				else if(music_statu->play_pause == 0)
				{
					//��������
					music_statu->play_pause = 1;
                    draw_pic(music_bmp[0][music_statu->music_num],0,0);
				}
				else
				{
					//��ͣ����
					music_statu->play_pause = 0;
                    draw_pic(music_bmp[1][music_statu->music_num],0,0);
				}
				break;
			case 0: 
				//�ϻ� ����Ӧ��ѡ�����
				return;
			default:
				break;
		}
		sleep(1);
	}
	return;
}