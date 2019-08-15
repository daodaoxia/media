#include "album.h"

struct node *save_node;
struct head *all,*all_0,*all_1;
int play_flag,node_flag,album_in_flag;

struct node
{
	char *name;     //���������·����bmp�ļ���
	struct node *prev;
    struct node *next;
};

struct head
{
	struct node *first; //bmp�ļ�ͷ�ڵ�
	struct node *last;//bmp�ļ�β�ڵ�
	int bmp_num; //��¼�ѱ����bmp�ļ�����
};

//��ӡ����
void print_linkedlist(struct head *all_2)
{
	printf("==============\n");
	if(all_2->first == NULL)
	{
		printf("������\n");
		return;
	}
	struct node*p = all_2->first;
	while(p)
	{
		printf("%s\n",p->name);
		p = p->next;
	}
	printf("\n");
}

//��������
struct head *create_linkedlist(void)
{
	struct head *all_2 = (struct head*)malloc(sizeof(*all_2));
	all_2->first = NULL;
	all_2->last = NULL;
	all_2->bmp_num = 0;
	return all_2;
}

//��������ڵ㵽��������
struct head *add_linklist(char *file_path,struct head *all_2)
{
	struct node *list = (struct node*)malloc(sizeof(*list));
	list->name = (char*)malloc(strlen(file_path) + 1);
	strcpy(list->name,file_path);
	list->next = NULL;
    list->prev = NULL;

	if(all_2->first == NULL)
	{
		all_2->first = list;
		all_2->last = list;
	}
	else
	{
		list->prev = all_2->last;
        all_2->last->next = list;
        all_2->last = list;
	}
	all_2->bmp_num ++;
	return all_2;
}


/*
	dir Ŀ¼��������
		@dir_name :��·����Ŀ¼��,��ʾҪ������Ŀ¼
*/
struct head *dir(char *dir_name,struct head *all_2)
{
	/*
		1.��Ŀ¼
	*/
	DIR* dirp = opendir(dir_name);
	if(NULL == dirp)
	{
		perror("open directory error");
		return NULL;
	}
	/*
		2.��ȡĿ¼��
	*/
	struct dirent * p;
	struct stat st;
	int ret = 0;
	char path_file[100] = {0};
	while(1)
	{
		//���path_file
		memset(path_file,0,100);
		p = readdir(dirp); //��ȡһ��Ŀ¼��
		if(NULL == p)
		{
			perror("read dir over");
			all_2->last->next = all_2->first;
			all_2->first->prev = all_2->last;
			break;
		}
		printf("%s\n",p->d_name);
		/*�ж϶�ȡ����Ŀ¼����һ����ͨ�ļ�,����Ŀ¼�ļ�*/
		sprintf(path_file,"%s/%s",dir_name,p->d_name); //��ϳɴ�·�����ļ���
		/*��ȡ�ļ�״̬*/
		ret = stat(path_file,&st);
		/*�ж��ļ�����*/
		if(-1 == ret)
		{
			perror("get status error");
		}
		else if(S_ISREG(st.st_mode))
		{
			if(strcmp(p->d_name+strlen(p->d_name)-4,".bmp")==0
			||strcmp(p->d_name+strlen(p->d_name)-4,".BMP")==0)
			{
				add_linklist(path_file,all_2);
				printf("bmp file\n");
			}
			
			printf("is REG\n");
		}
		else if(S_ISDIR(st.st_mode))
		{
			if((strcmp(p->d_name,".")!= 0)&&((strcmp(p->d_name,"..")!= 0)))
			{
				all_2 = dir(path_file,all_2);
				printf("is DIR\n");
			}
		}
	}
	/*
		3.�ر�Ŀ¼
	*/
	closedir(dirp);
	return all_2;
}

struct node *pic_choose(struct head *all)
{
	//��ʾ����ͼ
	draw_pic("./oprate_pic/album_0.bmp",0,0);
	
	struct node *pic = all->first;
	struct input_event ev;
	int x1=-1,y1=-1;
	int x2=-1,y2=-1;
	int pic_flag = 0;
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
			if((x2 > 125) && (x2 < 215))//����Ƭx1��Χ��
			{
				if( (y2 > 361)&&(y2 < 480) )//����Ƭy1��Χ��
				{
					pic_flag = 0;
				}
				else if((y2 > 240)&&(y2 < 361))//����Ƭy2��Χ��
				{
					pic_flag = 1;
				}
				else if((y2 > 120)&&(y2 < 240))//����Ƭy3��Χ��
				{
					pic_flag = 2;
				}
				else if((y2 > 0)&&(y2 < 120))//����Ƭy4��Χ��
				{
					pic_flag = 3;
				}
			}
			else if((x2 > 215) && (x2 < 312))//����Ƭx2��Χ��
			{
				if((y2 > 361)&&(y2 < 480))//����Ƭy1��Χ��
				{
					pic_flag = 4;
				}
				else if((y2 > 240)&&(y2 < 361))//����Ƭy2��Χ��
				{
					pic_flag = 5;
				}
				else if((y2 > 120)&&(y2 < 240))//����Ƭy3��Χ��
				{
					pic_flag = 6;
				}
				else if((y2 > 0)&&(y2 < 120))//����Ƭy4��Χ��
				{
					pic_flag = 7;
				}
			}
			break;
		}
	}
	while(pic_flag --)
	{
		pic = pic->next;
	}
	printf("pic %s\n",pic->name);
	//��ӡ�����ķ���
	if( (abs(y2-y1) > abs(x2-x1)) && (y2 < y1) )
	{
		printf("up\n");
	}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
	{
		printf("down\n");
	}else if(((x1 > 700) && (x1 < 800)) && (abs(y2-y1) < abs(x2-x1)) && (x2 < x1) )
	{
		printf("left\n");
		return NULL;
	}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
	{
		printf("right\n");
	}

	return pic;
}

//�õ�Ƭ��ӳ���
void *roll_pic(void *arg)
{
	printf("come into roll_pic\n");
	while(1)
	{
		if(play_flag == 1)
		{
			printf("pic1 %s\n",save_node->name);
			draw_pic(save_node->name,0,0);
			sleep(2);
			save_node = save_node->next;
		}
		else
		{
			usleep(300000);
		}
	}
	return;
}

int pic_loop(struct node *pic_now)
{
	struct input_event ev;
	struct node *p;
	struct head *change = NULL;
	int x1,y1,x2,y2;
	play_flag == 0;
	while (1)
	{
		//��ʾ��ǰͼƬ
		draw_pic(pic_now->name,0,0);
		x1=-1,y1=-1;
		x2=-1,y2=-1;
		while(1)
		{
			node_flag = 0;
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
				if( ( (x1 > 718 && x1 < 769)&&(y1 > 216 && y1 < 266) ) && (play_flag == 0))//�ڻõ�Ƭ��ӳ��ť��Χ��
				{
					if(all == all_0)
					{
						p = all->first;
						int cmp = strcmp(pic_now->name,p->name);
						while(1)
						{
							cmp = strcmp(pic_now->name,p->name);
							if(cmp != 0)
							{
								node_flag ++;
							}
							else
							{
								break;
							}
							p = p->next;
						}
						int travel = node_flag;
						pic_now = all_1->first;
						while(travel--)
						{
							pic_now = pic_now->next;
						}
						all = all_1;
					}
					//ͨ��ȫ�ֱ�����ȡnode����
					save_node = pic_now;
					play_flag = 1;
				}
				else if(play_flag == 1)
				{
					play_flag = 0;
					pic_now = save_node;
				}
				break;
			}
		}
		if(play_flag == 0)
		{
			//��ֹ��
			if(x1 != -1)
			{
				if((abs(x2 - x1)<30)&&(abs(y2 - y1)<30))
				{
					if( (x1 > 89) && (x1 < 673) )//��ͼƬ��Χ��
					{
						p = all->first;
						int cmp;
						while(1)
						{
							cmp = strcmp(pic_now->name,p->name);
							if( cmp != 0)
							{
								node_flag ++;
							}
							else
							{
								break;
							}
							p = p->next;
						}
						if(all == all_0)
						{
							change = all_1;
						}
						else
						{
							change = all_0;
						}
						int travel = node_flag;
						pic_now = change->first;
						while(travel--)
						{
							pic_now = pic_now->next;
						}
						all = change;
					}
					continue;
				}
			}
			//��ӡ�����ķ���
			if( abs(y2-y1) > abs(x2-x1) && y2 < y1)
			{
				pic_now = pic_now->prev;
				printf("up\n");
			}else if( abs(y2-y1) > abs(x2-x1) && y2 > y1)
			{
				pic_now = pic_now->next;
				printf("down\n");
			}else if(  ((x1 > 700) && (x1 < 800)) && ( (abs(y2-y1) < abs(x2-x1) ) && (x2 < x1) ))
			{
				printf("left\n");
				return -1;
			}else if( abs(y2-y1) < abs(x2-x1) && x2 > x1)
			{
				printf("right\n");
				return 1;
			}
		}
	}
	return 0;
}

void album(void)
{
	if(album_in_flag == 0)
	{
		all_0 = create_linkedlist();
		all_1 = create_linkedlist();
		all_0 = dir("./album_pic",all_0);//ͼƬ�ļ���·��
		all_1 = dir("./roll_pic",all_1);//�ڵ�ͼƬ�ļ���·��
		all = all_0;
		//�����߳�����ͼƬ�õ�Ƭ����
		play_flag = 0;
		pthread_t pid10;
		pthread_create(&pid10,NULL,roll_pic,NULL);
		album_in_flag = 1;
	}
	struct node *pic_get;
	int loop_flag = 0;
	while(loop_flag != -1)
	{
		//ѡ��ͼƬ��ʾ
		pic_get = pic_choose(all);
		//�ײ��ϻ�����������
		if(pic_get == NULL)
		{
			return;
		}
		loop_flag = pic_loop(pic_get);
	}
	
	//��ӡ�ҵ���ͼƬ
	//print_linkedlist(all);
	
	return ;
}