#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#define HASH_LENGTH    1024               //��ϣ��ĳ���     
#define RANDOM_NUMBER  47               //�����

clock_t start, finish;  
double  duration;  
typedef struct      
{  
	char id[10];
	char py[15];    //���ֵ�ƴ��
	int k;       //ƴ������Ӧ������
}NAME;

typedef struct    //��ϣ��
{   
	char id[10];     // id
	char py[15];     //name
	int k;        //ƴ������Ӧ������
	int si;       //���ҳ���
}HASH;

NAME g_NameList[HASH_LENGTH];    //ȫ�ֱ���NAME  
HASH g_HashList[HASH_LENGTH];   //ȫ�ֱ���HASH 

int g_num = 0;   // ��¼����Ŀ

//////////////////////////////////////////////////////////////////////////
// ��ʼ���������
void InitNameList(char *file)    
{   
	char f[15] = {0};
	int r,s0,i;
	int len;

	FILE *fp;

	if((fp=fopen(file,"r"))==NULL)
	{ 
		printf("�ļ���ʧ�ܣ�\n");
		return ;
	}
	else
	{
		g_num = 0;

		while(fscanf(fp, "%s", g_NameList[g_num].id) > 0 )
		{
			 fscanf(fp, "%s", g_NameList[g_num].py);

			g_num ++;
		}

	}

	fclose(fp);


	for(i = 0; i < g_num; i++)
	{ 
		s0 = 0;

		strcpy(f, g_NameList[i].py);
	
		len = strlen(f);  

		for(r = 0; r < len; r ++) 
		{
			s0 = *(f+r) + s0;
		}

		g_NameList[i].k = s0;
	} 
}



//////////////////////////////////////////////////////////////////////////
//������ϣ��
void CreateHashList()   
{ 
	int i;
	int sum, adr,d;

	// ��ʼ�� g_hashList �е�����
	for(i=0; i< g_num; i++) 
	{   
		strcpy(g_HashList[i].py,"");
		strcpy(g_HashList[i].id,"");
		g_HashList[i].k  = 0;
		g_HashList[i].si = 0;
	}

	// ������ϣ��
	for(i=0; i<g_num; i++)
	{  
		sum = 0;
		adr = (g_NameList[i].k)%RANDOM_NUMBER;  
		//��ϣ����
		d = adr;
		if(g_HashList[adr].si == 0)     //�������ͻ
		{ 
			g_HashList[adr].k  = g_NameList[i].k;
			strcpy(g_HashList[adr].py, g_NameList[i].py);
			strcpy(g_HashList[adr].id, g_NameList[i].id);
			g_HashList[adr].si = 1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ����name ���Ҽ�¼   
void  FindList(char *name)
{	
	int s0=0,r,sum=1,adr,d;

	int len = strlen(name);

	//���������ƴ������Ӧ�������ؼ���
	for(r = 0; r < len; r ++)   
		s0 += name[r]; 

	//ʹ�ù�ϣ����
	adr = s0%RANDOM_NUMBER;  

	d = adr;

	if(g_HashList[adr].k == s0)    
	{
		printf("\n%10s%10s\n",g_HashList[d].id,g_HashList[d].py); 
	}
	else if (g_HashList[adr].k == 0) 
	{
		printf("�޴˼�¼!\n");
	}
	else
	{   
		int g = 0;
		do
		{  
			d = (d + s0%10 + 1)%RANDOM_NUMBER;       //α���̽����ɢ�з������ͻ

			sum=sum+1;

			if(g_HashList[d].k == 0)
			{  
				printf("�޴˼�¼! \n");  
				g = 1;     
			}	

			if(g_HashList[d].k == s0)
			{ 
				printf("\n%10s%10s\n",g_HashList[d].id,g_HashList[d].py); 

				g = 1;  
			}

		}while(g == 0);   
	}  
}



//////////////////////////////////////////////////////////////////////////
// ������
void Create(char * cmd)
{
	FILE * fp;
	char fileName[15] = {0};
	char id[10] = {0};
	char name[10] = {0};
	int len = 0;
	int i,j,m;
	int find = 0;
	char ex[5] = {".txt"};

	// ����ĳ���
	len = strlen(cmd);

	// ������� ����ļ���
	for(i = 0; i < len; i++)
	{
		if ('(' == cmd[i])
		{
			strcat(fileName,ex);
			break;
		} 

		if(0 == find)
		{
			fileName[i] = cmd[i];
		}
	}

	find = 0;

	j = i +2;
	i = 0;

	// ����������  id ��ֵ
	for (j; j <len; j++, i++)
	{
		if(cmd[j] == '\"')
		{
			find = 1;
			id[i] = '\0';
			break;
		}

		if (0 == find)
		{
			id[i] = cmd[j];
		}
	}

	j = j +3;
	i = 0;
	find = 0;

	// ����������  name ��ֵ
	for (j; j <len; j++, i++)
	{
		if(cmd[j] == '\"')
		{
			find = 1;
			name[i] = '\0';
			break;
		}

		if (0 == find)
		{
			name[i] = cmd[j];
		}
	}

	if((cmd[j+1] != ')') && ((cmd[j+2] != ';')))
	{
		printf("�������� ��\n");
		return;
	}

	// ���������ļ���д������
	fp = fopen(fileName, "w");

	if (fp)
	{
		printf("\n������ɹ�\n");
		fprintf(fp, "%s %s\n",id,name);

		fclose(fp);
	}
	else
	{
		printf("\n������ʧ��\n");
	}
}

//////////////////////////////////////////////////////////////////////////
// �����¼
void Insert(char *file,char *cmd)
{
	FILE * fp;
	char value[8] = {0};
	char ex[5] = {".txt"};
	char id[10] = {0};
	char name[15] = {0};
	int i , j;

	int len = strlen(cmd);

	strcat(file,ex);

	fp = fopen(file,"r");
	if (!fp)
	{
		printf(" �ñ�����");
		return;
	}
	else
	{
		fclose(fp);

		// ���������� �ļ���
		for(i = 0; i < len; i++)
		{
			if('(' == cmd[i])
			{
				break;
			}
			value[i] = cmd[i];
		}

		if(cmd[i+1] != '\"')
		{
			printf("�������� ��\n");
			return;
		}


		i = i+2;
		j = 0;
		// ����������  id ��ֵ
		for(i; i< len; i++,j++)
		{
			if(cmd[i] == '\"')
			{
				id[j] = '\0';
				break;
			}

			id[j] = cmd[i];
		}

		i = i + 3;
		j = 0;

		// ����������  name ��ֵ
		for (i; i< len; i++, j++)
		{
			if(cmd[i] == '\"')
			{
				name[j] = '\0';
				break;
			}

			name[j] = cmd[i];
		}
		
		//�ж������Ƿ� �� ); ����
 		if ((cmd[i+1] != ')') && (cmd[i+1] != ';'))
		{
			printf("�������� ��\n");
			return;
		}

		// ���ļ�׷��һ����¼
		fp = fopen(file,"a+");
		if (fp)
		{
			printf("\n����ɹ�\n");
			fprintf(fp, "%s %s\n",id,name);

			fclose(fp);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ���� ָ�� �� name ��
void Select11(char * file, char *name1)
{
	FILE *fp;
	char ex[5] = {".txt"};

	char name[15] = {0};
	char id[10] = {0};

	strcat(file, ex);

	//��ʼ�� �ļ��е� n����
	InitNameList(file);

	// ������ϣ��
	CreateHashList();

	// ���� ��¼
	FindList(name1);
}


///////////////////////////////////////////      ��  ��  ��  ��     //////////////////////////////////////
// ���ù�ϣ���� ���� ָ�� �� name ��

void Select1(char * file, char *name1)
{

	FILE *fp;
	char ex[5] = {".txt"};

	char name[15] = {0};
	char id[10] = {0};
	
	strcat(file, ex);

	fp = fopen(file,"r");

	if (fp)
	{
		while(fscanf(fp, "%s", id) > 0)
		{
			fscanf(fp, "%s", name);

			if (0 ==  strcmp(name1, name))
			{
				printf("%10s%10s\n",id,name);
			}
		}
		
		fclose(fp);
	}
	else
	{
		printf("�ñ����� ��\n");
		return;
	}
}


///////////////////////////////////////////      ��  ��  ��  ��     //////////////////////////////////////
// �������е� name ��
void Select2(char * file)
{
	FILE *fp;
	char ex[5] = {".txt"};

	char name[15] = {0};
	char id[10] = {0};

	// ���ʼۼӺ�׺��
	strcat(file, ex);

	// ���ļ�
	fp = fopen(file,"r");

	if (fp)
	{
		// ��ȡ�ļ�����
		while(fscanf(fp, "%s", id) > 0)
		{
			fscanf(fp, "%s", name);

			// ��ӡ�ļ��еļ�¼
			printf("%10s%10s\n",id,name);
	
		}

		fclose(fp);
	}
	else
	{
		printf("�ñ����� ��\n");
		return;
	}
}


//////////////////////////////////////////////////////////////////////////
// ��������
void Update(char *file, char *id, char *name)
{

}

//////////////////////////////////////////////////////////////////////////
// ���˵�
void Menu()
{
	printf("\n");
	printf(">>>>>>>>������[create]>>>>>>�����¼[insert]>>>>>>>��ѯ��¼[select]  \n");
	printf("\n");
}

//////////////////////////////////////////////////////////////////////////
// ������
void main()
{
	// �������� �ܵ�����
	char cmd[1024] = {0};

	// ��������create ,insert select ,update 
	char cmd1[7] = {0};

	// ���������ļ���
	char file[10] = {0};

	// �������� name ֵ
	char name[15] = {0};

	// �������� id ֵ
	char id[10] = {0};

	char table[6] = {0};
	char temp[30] = {0};
	char temp1[30] = {0};
	char temp2[30] = {0};

	char *p = NULL;
    printf("********************************************************\n");
	printf("**********       WELCOME TO OUR SQL            *********\n");
	printf("**********                                     *********\n");
	printf("**********                                     *********\n");
	printf("**********       �鳤:���α�                   *********\n");
	printf("**********       ��Ա:���ɣ������أ�������     *********\n");
	printf("********************************************************\n");

	int i = 0;
	int len = 0;

	while(1)
	{
		Menu();

		printf("----------------------------------------------------------------------\n");

		// �����ܵ�����
		scanf("%s", cmd);

		// ���������ò���
		for (i = 0; i < 7; i++)
		{
			cmd1[i] = cmd[i];
		}

		// ����������������
		if (0 == strcmp(cmd1, "create"))
		{
			scanf("%s", table);

			// �ж��������Ƿ���table
			if (0 != strcmp(table, "table"))
			{
				printf("�������� ��\n");
				fflush(stdin);

			}
			else
			{
				scanf("%s", temp);
				Create(temp);
				
			}
			
		}
		else if (0 == strcmp(cmd1, "insert"))  // ������������¼����
		{
			scanf("%s", table);

			// �ж������Ƿ��� into
			if (0 != strcmp(table, "into"))
			{
				printf("�������� ��\n");
				fflush(stdin);

			}
			else
			{
				scanf("%s",file);
				scanf("%s", temp);
				Insert(file,temp);
				
			}
		}
		else if (0 == strcmp(cmd1, "select")) // ���������ѯ��¼����
		{
			scanf("%s", name);

			// �ж��������Ƿ���  id
			if(0 == strcmp(name, "id"))
			{
				scanf("%s %s",table,file);
				if (0 != strcmp(table, "from"))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				len = strlen(file);

				
				scanf("%s %s", temp,temp1);

					// �ж��������Ƿ��� where name= 
				if ((0 != strcmp(temp,"where")) && (0 != strcmp(temp1,"name=")))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", temp2);

				len = strlen(temp2);

				// �ж��Ƿ��е����� ���ԷֺŽ���
				if ((temp2[0] != '\'') && (temp2[len-2] != '\'') && (temp2[len-1] != ';'))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				temp2[len-2] = '\0';

				// ȥ���ֺţ���������� name ֵ
				for (i = 0; i < len-2; i++)
				{
					temp2[i] = temp2[i+1]; 
				}
                start = clock();
				Select1(file,temp2);
				finish = clock();  
				duration = (double)(finish - start) / CLOCKS_PER_SEC;  
				printf("������ʱ %f seconds\n", duration );  
			}
			else if(0 == strcmp(name, "name")) // �ж��������Ƿ��� name
			{
				scanf("%s %s",table,file);
				if (0 != strcmp(table, "from"))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				len = strlen(file);

				// �ж������Ƿ� �� �� ����
				if (file[len -1] != ';')
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}
				else
				{
					// ȥ���ֺţ�����������ļ���
					file[len-1] = '\0';
					Select2(file);
				}

			}
			else
			{
				printf("�������� ��\n");
				fflush(stdin);
				continue;

			}

		}
		else if (0 == strcmp(cmd1, "update"))   // ����������¼�¼����
		{
			scanf("%s", file);

			if (0 != strcmp(file, ""))
			{
				printf("�������� ��\n");
				fflush(stdin);
				continue;
			}
			else
			{
				scanf("%s %s",table,temp);

				// �ж��������Ƿ��� set id= 
				if ((0 != strcmp(table, "set")) || (0 != strcmp(temp, "id=")))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", id);

				len = strlen(id);

				// �ж��Ƿ��е�����
				if ((id[0] != '\'') && (id[len-1] != '\''))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				id[len -1] = '\0';

				for (i = 0; i < len-1; i++)
				{
					id[i] = id[i+1];
				}
				
				scanf("%s %s", temp,temp1);

				// �ж��������Ƿ��� where name= 
				if ((0 != strcmp(temp,"where")) && (0 != strcmp(temp1,"name=")))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", name);

				len = strlen(name);

				//�ж����� �Ƿ��� ; ����
				if ((name[0] != '\'') && (name[len-2] != '\'') && (name[len-1] != ';'))
				{
					printf("�������� ��\n");
					fflush(stdin);
					continue;
				}

				name[len-2] = '\0';
				
				// ȥ������ �� �ֺţ���������� name ֵ
				for (i = 0; i < len-2; i++)
				{
					name[i] = name[i+1]; 
				}

				Update(file, id, name);
			}

		}
		else
		{
			printf("�������� \n");
		}
		

		printf("------------------------------------------------------\n");

	}

}