#include "start.h"

struct node *pass;
char * pass_pic[5];
int *plcd,fd0,fd_screen;

void direction(struct node *pass)
{
	//printf("come into direction\n");
	//��ʼ��ֵ 
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
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
			if(pass->psw_flag == 1)
            {
                if( x1 > 555 && x1< 640 && y1 > 186 && y1 < 298)//���� 0��Χ
                {
                    pass->pass_right ++;
                    if (pass->pass_right == 4)
                    {
                        draw_pic(pass_pic[4],0,0);
						usleep(300000);
						pass->psw_flag = 2;
                    }
                }
                else if (x1 > 279 && x1 < 551 && y1 > 57 && y1 < 471)//���������ַ�Χ��
                {
                    pass->pass_fault ++;
                }
            }
			break;
		}	
	}
    //�ж��Ƿ��Ѿ�����4������
	if(((pass->pass_right + pass->pass_fault)== 4)&&(pass->pass_right != 4))
	{
		pass->pass_right = 0;
		pass->pass_fault = 0;
	}
	if(pass->psw_flag != 0)
	{
		//��ʾ�������i
    	draw_pic(pass_pic[pass->pass_right + pass->pass_fault],0,0);
	}
    
	//��ֹ��
	if(x1 != -1)
	{
		if((abs(x2 - x1)<30)&&(abs(y2 - y1)<30))
		{
			printf("touch\n");
			return;
		}
	}
	//��ӡ�����ķ���
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
			//��ʾ���ݽ���
			draw_pic("./oprate_pic/lock_face.bmp",0,0);
			sleep(1);
			//��ʾ�������
			draw_pic(pass_pic[0],0,0);
			pass->psw_flag = 1;
		}
		else if(pass->psw_flag == 1)
		{
			//��ʾ��������
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

//����������������
void start(void)
{
    //�򿪴����ļ�
	fd0 = open("/dev/input/event0",O_RDWR|O_NONBLOCK);
	if(fd0 == -1)
	{
		printf("open event0 error\n");
		return ;
	}
	//����ʾ���ļ�
	fd_screen = open("/dev/fb0",O_RDWR);
	if(-1 == fd_screen)
	{
		printf("Open machine error !\n");
		return ;
	}
	//ӳ��	
	plcd = mmap(NULL,SIZE_OF_LCD,
	PROT_WRITE | PROT_READ,MAP_SHARED,fd_screen,0);
	//ӳ��ɹ����
	if(MAP_FAILED == plcd)
	{
		printf("Map lcd failed !\n");
		return ;
	}

    //��ʾ����ͼƬ
    draw_pic("./oprate_pic/start.bmp",0,0);
    sleep(3);
    //��ʾ��������
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