#include "app.h"

int back(void)
{
    //��ʼ��ֵ 
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
    int back_flag = 0;
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
            return -1;
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
        back_flag = 1;
	}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
	{
		printf("right\n");
	}
    return back_flag;
}

void camera(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/camera.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}

void calendar(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/calendar.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}

void clock(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/clock.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}

void messege(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/messege.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}

void phone(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/phone.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}

void wechat(void)
{
    //��ʾ��ǰ����ͼƬ
	draw_pic("./oprate_pic/wechat_0.bmp",0,0);
    sleep(1);
    draw_pic("./oprate_pic/wechat_1.bmp",0,0);
    //�ж��ϻ��˳�
    int back_flag = 0;
    while(back_flag != 1)
    {
        back_flag = back();
    }
    return;
}
