#include "app.h"

int bg_app_choose(int app_flag)
{
	
	printf("bg1  %d\n",app_flag);
	struct input_event ev;
	int x1,y1,x2,y2;
	x1=-1,y1=-1;
	x2=-1,y2=-1;
	while(1)
	{
		//��ʾ��̨��Ƭ����
		draw_pic("./oprate_pic/background.bmp",0,0);
		app_flag = -1;
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
			if( (x1 > 111) && (x1 < 670) )//��app x1��Χ��
			{
				if( (y1 > 219) && (y1 < 408) )//��app y1��Χ��
				{
					app_flag = 0;
				}
				else if((y1 > 0) && (y1 < 218))//��app y2��Χ��
				{
					app_flag = 2;
				}
				else if((y1 > 408) && (y1 < 800))//��app y3��Χ��
				{
					app_flag = 6;
				}
				break;
			}
			else
			{
				app_flag = -1;
				break;
			}
		}	
	}
	//��ӡ�����ķ���
	if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
	{
		printf("up\n");
	}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
	{
		printf("down\n");
	}else if(((x1 > 700) && (x1 < 800)) &&(abs(y2-y1) < abs(x2-x1) && x2 < x1))
	{
		printf("left\n");
		app_flag = -1;
	}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
	{
		printf("right\n");
	}
	return app_flag;
}

int app_choose(void)
{
	struct input_event ev;
	int x1,y1,x2,y2;
	int app_flag;
	while(1)
	{
		//��ʾ�������
		draw_pic("./oprate_pic/home.bmp",0,0);
		x1=-1,y1=-1;
		x2=-1,y2=-1;
		app_flag = -1;
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
				if(x2 > 64 && x2 < 136)//��app x1��Χ��
				{
					if(y2 > 364 && y2 < 451)//��app y1��Χ��
					{
						app_flag = 0;
					}
					else if((y1 > 256) && (y1 < 389))//��app y2��Χ��
					{
						app_flag = 1;
					}
					else if((y1 > 146) && (y1 < 223))//��app y3��Χ��
					{
						app_flag = 2;
					}
					else if((y1 > 32) && (y1 < 111))//��app y4��Χ��
					{
						app_flag = 3;
					}
				}
				else if ((x1 > 167) && (x1 < 237))//��app x2��Χ��
				{
					if(y2 > 364 && y2 < 451)//��app y1��Χ��
					{
						app_flag = 7;
					}
					else if((y1 > 256) && (y1 < 389))//��app y2��Χ��
					{
						app_flag = 8;
					}

				}
				else if(x2 > 712 && x2 < 775)//��app x3��Χ��
				{
					if(y2 > 312 && y2 < 392)//��app y1��Χ��
					{
						app_flag = 4;
					}
					else if((y1 > 201) && (y1 < 281))//��app y2��Χ��
					{
						app_flag = 5;
					}
					else if((y1 > 89) && (y1 < 168))//��app y3��Χ��
					{
						app_flag = 6;
					}
				}
				break;
			}	
		}
		//��ӡ�����ķ���
		if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
		{
			printf("up\n");
		}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
		{
			printf("down\n");
		}else if(((x1 > 700) && (x1 < 800)) &&(abs(y2-y1) < abs(x2-x1) && x2 < x1))
		{
			printf("left\n");
			app_flag = bg_app_choose(app_flag);
		}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
		{
			printf("right\n");
		}

   		 //Ӧ��ѡ��
		switch (app_flag)
		{
		case 0:
			//���
			album();
			break;
		case 1:
			//���
			camera();
			break;
		case 2:
			//����
			calendar();
			break;
		case 3:
			//ʱ��
			clock();
			break;
		case 4:
			//��Ϣ
			messege();
			break;
		case 5:
			//�绰
			phone();
			break;
		case 6:
			//΢��
			wechat();
			break;
		case 7:
			//����
			music();
			break;
		case 8:
			//2048��Ϸ
			game_2048();
			break;
		default:
			break;
		}
	}
}