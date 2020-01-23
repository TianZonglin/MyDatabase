## DBMS课程设计

**题目：利用文件操作模拟mysql的操作**
**成员：田宗霖，刘永恒，胡闵**

### 主要功能模块

#### 创建表模块

create table xxx (“id”,”name”)  ;
在自定义目录下创建xxx.txt；并且要把id和name字段存放进去。 

#### 插入表模块

insert into xxx values ("2012310207","Bill")  ;
在xxx.txt文件中把01，bill放在对应字段下面；即像表的格式一样存储：

结果：

id            name    
0132154      billskatt     
0235412      kimbrow    

#### 查询表模块

select  name  from  xxx  ; 
把txt文件中‘name列’内容全部输出。
select  id  from  xxx  where  name=‘bill’;
把txt中name为bill的记录全部输出。

#### 嵌入哈希算法

建立哈希结构体，通过为name字段建立索引，并将查询与之结合，达到使用哈希索引的目的。由于此代码直接建立了哈希索引，故无法在同一个程序下比较建立哈希表与否对select查询时间的影响。
解决办法：再建一个同样的cpp，内容相同，但select不调用哈希函数，即并不通过哈希查询，然后对两个程序分别运行，比较时间。

### 主要功能实现方法

#### 创建表

```
FILE * fp;
	    ...
		// 解析命令， 获得文件名
		for(i = 0; i < len; i++)
		{
		...
		}
		...
		// 解析命令，获得  id 的值
		for (j; j <len; j++, i++)
		{
			...
		}
...
		// 解析命令，获得  name 的值
		for (j; j <len; j++, i++)
		{
			...
		}
...
		// 创建并打开文件，写入数据
		fp = fopen(fileName, "w");
		if (fp)
		{
			printf("\n创建表成功\n");
			fprintf(fp, "%s %s\n",id,name);
			fclose(fp);
			}
```

#### 插入表

```
		//判断命令是否 以 ); 结束
 		if ((cmd[i+1] != ')') && (cmd[i+1] != ';'))
		{
			printf("错误命令 ！\n");
			return;
		}
		// 打开文件追加一条记录
		fp = fopen(file,"a+");
		if (fp)
		{
			printf("\n插入成功\n");
			fprintf(fp, "%s %s\n",id,name);
			fclose(fp);
	    	}
```

#### 查询表

```
          // 使用哈希查找 指定 的 name 项
void Select11(char * file, char *name1)
{
		FILE *fp;
		char ex[5] = {".txt"};
		char name[15] = {0};
		char id[10] = {0};
		strcat(file, ex);
			//初始化 文件中的 n数据
		InitNameList(file);
			// 建立哈希表
		CreateHashList();
			// 查找 记录
		FindList(name1);
}

            // 不用哈希表来 查找 指定 的 name 项
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
			printf("该表不存在 ！\n");
			return;
			}
}
// 查找所有的 name 项[select *]
void Select2(char * file)
{
			FILE *fp;
			char ex[5] = {".txt"};
			char name[15] = {0};
			char id[10] = {0};
			// 给问价加后缀名
			strcat(file, ex);
			// 打开文件
			fp = fopen(file,"r");
			if (fp)
			{
				// 读取文件内容
				while(fscanf(fp, "%s", id) > 0)
				{
					fscanf(fp, "%s", name);
					// 打印文件中的记录
					printf("%10s%10s\n",id,name);
				}
				fclose(fp);
			}
			else
			{
				printf("该表不存在 ！\n");
				return;
			}
}
```

#### 嵌入哈希算法

```
哈希算法内容：
此结构体用来存放 名字的拼音以及其对应的整数，为以后的哈希函数用来计算位置。
typedef struct    //哈希表
{   
	char id[10];
	char py[15];     //名字的拼音
	int k;          //拼音所对应的整数
	int si;         //查找长度
}HASH;
typedef struct      
{  
	char id[10];
	char py[15];    //名字的拼音
	int k;         //拼音所对应的整数
}NAME; 
以下两个顺序链表用来存放所有的对应的元组
NAME g_NameList[HASH_LENGTH];    //全局变量NAME  
HASH g_HashList[HASH_LENGTH];   //全局变量HASH 
int g_num = 0;
void InitNameList(char *file)    
{   
	char f[15] = {0};
	int r,s0,i;
	int len;
	FILE *fp;   //一般使用FILE*类型变量表示文件句柄，通过它来访问FILE结构体，对文件进行操作。FILE**是指向FILE*的指针
	if((fp=fopen(file,"r"))==NULL)    
//打开文件名为file的文件,根据fopen的返回值(fp),判断该文件是否打开 
//如果没有打开,则打印"Error opening file"信息,然后退出程序,
//如果正确打开,则跳过这里,执行后面的语句,
	{ 
		printf("文件打开失败！\n");
		return ;
	}
	else
	{
		g_num = 0;
		while(fscanf(fp, "%s", g_NameList[g_num].id) > 0 )
//从一个流中执行格式化输入,fscanf遇到空格和换行时结束，注意空格时也结束。
		{
			 fscanf(fp, "%s", g_NameList[g_num].py);
			g_num ++;
		}
	}
	fclose(fp);
//使用fclose()函数就可以把缓冲区内最后剩余的数据输出到磁盘文件中，并释放文件指针和有关的缓冲区。
	for(i = 0; i < g_num; i++)
	{ 
		s0 = 0;
		strcpy(f, g_NameList[i].py)；
		len = strlen(f);  
		for(r = 0; r < len; r ++) 
		{
			s0 = *(f+r) + s0;
		}
		g_NameList[i].k = s0;
	} 
}
//建立哈希表
void CreateHashList()   
{ 
	int i;
	int sum, adr,d;
	for(i=0; i< g_num; i++) //除留余数法
	{   
		strcpy(g_HashList[i].py,"");
		strcpy(g_HashList[i].id,"");
		g_HashList[i].k  = 0;
		g_HashList[i].si = 0;
	}
	for(i=0; i<g_num; i++)
	{  
		sum = 0;
		adr = (g_NameList[i].k)%RANDOM_NUMBER;  
		//哈希函数
		d = adr;
		if(g_HashList[adr].si == 0)     //如果不冲突
		{ 
			g_HashList[adr].k  = g_NameList[i].k;
			strcpy(g_HashList[adr].py, g_NameList[i].py);
			strcpy(g_HashList[adr].id, g_NameList[i].id);
			g_HashList[adr].si = 1;
		}
	}
}
//查找   
void  FindList(char *name)
{	
	int s0=0,r,sum=1,adr,d;
	int len = strlen(name);
	//scanf("%s",name); 
	//求出姓名的拼音所对应的整数关键字
	for(r = 0; r < len; r ++)   
		s0 += name[r]; 
	//使用哈希函数
	adr = s0%RANDOM_NUMBER;  
	d = adr;
	if(g_HashList[adr].k == s0)    
	{
			printf("\n%10s%10s\n",g_HashList[d].id,g_HashList[d].py); 
	}
	else if (g_HashList[adr].k == 0) 
	{
		printf("无此记录!\n");
	}
	else
	{   
		int g = 0;
		do
		{  
			d = (d + s0%10 + 1)%RANDOM_NUMBER;       
//伪随机探测再散列法处理冲突
			sum=sum+1;
			if(g_HashList[d].k == 0)
			{  
				printf("无此记录! \n");  
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
```

### 开发工具

通过先期对问题的查阅资料以及对问题的分析，发现c对于文件操作较为方便，所以决定采用c语言来编写程序，使用的编译器为VC6.0。

### 运行效果及实验分析

插入简单数据运行：
 
 
插入8700条记录完成后
 
select选择（未用哈希）耗时在0.007s左右跳动，可能和硬件有关
 
select使用哈希,重复多次，最大值也不到0.007s，说明使用哈希之后，查询时间有所减少。
 
附代码：

```
///////////////////////////////////使用哈希////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#define HASH_LENGTH    1024*2            //哈希表的长度     
#define RANDOM_NUMBER  47               //随机数
clock_t start, finish;  
double  duration; 
typedef struct      
{  
	char id[10];
	char py[15];    //名字的拼音
	int k;       //拼音所对应的整数
}NAME;

typedef struct    //哈希表
{   
	char id[10];     // id
	char py[15];     //name
	int k;        //拼音所对应的整数
	int si;       //查找长度
}HASH;

NAME g_NameList[HASH_LENGTH];    //全局变量NAME  
HASH g_HashList[HASH_LENGTH];   //全局变量HASH 

int g_num = 0;   // 记录的数目

//////////////////////////////////////////////////////////////////////////
// 初始化表的数据
void InitNameList(char *file)    
{   
	char f[15] = {0};
	int r,s0,i;
	int len;

	FILE *fp;

	if((fp=fopen(file,"r"))==NULL)
	{ 
		printf("文件打开失败！\n");
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
//建立哈希表
void CreateHashList()   
{ 
	int i;
	int sum, adr,d;

	// 初始化 g_hashList 中的数据
	for(i=0; i< g_num; i++) 
	{   
		strcpy(g_HashList[i].py,"");
		strcpy(g_HashList[i].id,"");
		g_HashList[i].k  = 0;
		g_HashList[i].si = 0;
	}

	// 建立哈希表
	for(i=0; i<g_num; i++)
	{  
		sum = 0;
		adr = (g_NameList[i].k)%RANDOM_NUMBER;  
		//哈希函数
		d = adr;
		if(g_HashList[adr].si == 0)     //如果不冲突
		{ 
			g_HashList[adr].k  = g_NameList[i].k;
			strcpy(g_HashList[adr].py, g_NameList[i].py);
			strcpy(g_HashList[adr].id, g_NameList[i].id);
			g_HashList[adr].si = 1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 根据name 查找记录   
void  FindList(char *name)
{	
	int s0=0,r,sum=1,adr,d;

	int len = strlen(name);

	//求出姓名的拼音所对应的整数关键字
	for(r = 0; r < len; r ++)   
		s0 += name[r]; 

	//使用哈希函数
	adr = s0%RANDOM_NUMBER;  

	d = adr;

	if(g_HashList[adr].k == s0)    
	{
		printf("\n%10s%10s\n",g_HashList[d].id,g_HashList[d].py); 
	}
	else if (g_HashList[adr].k == 0) 
	{
		printf("无此记录!\n");
	}
	else
	{   
		int g = 0;
		do
		{  
			d = (d + s0%10 + 1)%RANDOM_NUMBER;       //伪随机探测再散列法处理冲突

			sum=sum+1;

			if(g_HashList[d].k == 0)
			{  
				printf("无此记录! \n");  
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
// 创建表
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

	// 命令的长度
	len = strlen(cmd);

	// 解析命令， 获得文件名
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

	// 解析命令，获得  id 的值
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

	// 解析命令，获得  name 的值
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
		printf("错误命令 ！\n");
		return;
	}

	// 创建并打开文件，写入数据
	fp = fopen(fileName, "w");

	if (fp)
	{
		printf("\n创建表成功\n");
		fprintf(fp, "%s %s\n",id,name);

		fclose(fp);
	}
	else
	{
		printf("\n创建表失败\n");
	}
}

//////////////////////////////////////////////////////////////////////////
// 插入记录
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
		printf(" 该表不存在");
		return;
	}
	else
	{
		fclose(fp);

		// 解析命令，获得 文件名
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
			printf("错误命令 ！\n");
			return;
		}


		i = i+2;
		j = 0;
		// 解析命令，获得  id 的值
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

		// 解析命令，获得  name 的值
		for (i; i< len; i++, j++)
		{
			if(cmd[i] == '\"')
			{
				name[j] = '\0';
				break;
			}

			name[j] = cmd[i];
		}
		
		//判断命令是否 以 ); 结束
 		if ((cmd[i+1] != ')') && (cmd[i+1] != ';'))
		{
			printf("错误命令 ！\n");
			return;
		}

		// 打开文件追加一条记录
		fp = fopen(file,"a+");
		if (fp)
		{
			printf("\n插入成功\n");
			fprintf(fp, "%s %s\n",id,name);

			fclose(fp);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 查找 指定 的 name 项
void Select1(char * file, char *name1)
{
	FILE *fp;
	char ex[5] = {".txt"};

	char name[15] = {0};
	char id[10] = {0};

	strcat(file, ex);

	//初始化 文件中的 n数据
	InitNameList(file);

	// 建立哈希表
	CreateHashList();

	// 查找 记录
	FindList(name1);
}


//////////////////////////////////////////////////////////////////////////
// 不用哈希表来 查找 指定 的 name 项
/*
void Select3(char * file, char *name1)
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
		printf("该表不存在 ！\n");
		return;
	}
}
*/

//////////////////////////////////////////////////////////////////////////
// 查找所有的 name 项
void Select2(char * file)
{
	FILE *fp;
	char ex[5] = {".txt"};

	char name[15] = {0};
	char id[10] = {0};

	// 给问价加后缀名
	strcat(file, ex);

	// 打开文件
	fp = fopen(file,"r");

	if (fp)
	{
		// 读取文件内容
		while(fscanf(fp, "%s", id) > 0)
		{
			fscanf(fp, "%s", name);

			// 打印文件中的记录
			printf("%10s%10s\n",id,name);
	
		}

		fclose(fp);
	}
	else
	{
		printf("该表不存在 ！\n");
		return;
	}
}


//////////////////////////////////////////////////////////////////////////
// 更新数据
void Update(char *file, char *id, char *name)
{

}

//////////////////////////////////////////////////////////////////////////
// 主菜单
void Menu()
{
	printf("\n");
	printf(">>>>>>>>创建表[create]>>>>>>插入记录[insert]>>>>>>>查询记录[select]  \n");
	printf("\n");
}

//////////////////////////////////////////////////////////////////////////
// 主函数
void main()
{
	// 用来保存 总的命令
	char cmd[1024] = {0};

	// 用来保存create ,insert select ,update 
	char cmd1[7] = {0};

	// 用来保存文件名
	char file[10] = {0};

	// 用来保存 name 值
	char name[15] = {0};

	// 用来保存 id 值
	char id[10] = {0};

	char table[6] = {0};
	char temp[30] = {0};
	char temp1[30] = {0};
	char temp2[30] = {0};

	char *p = NULL;

	int i = 0;
	int len = 0;
	printf("********************************************************\n");
	printf("**********       WELCOME TO OUR SQL            *********\n");
	printf("**********                                     *********\n");
	printf("**********                                     *********\n");
	printf("**********       组长:刘盼标                   *********\n");
	printf("**********       组员:胡闵，田宗霖，刘永恒     *********\n");
	printf("********************************************************\n");
	while(1)
	{
		Menu();

		printf("-------------------------------------------------------------------\n");

		// 输入总的命令
		scanf("%s", cmd);

		// 解析命令，获得操作
		for (i = 0; i < 7; i++)
		{
			cmd1[i] = cmd[i];
		}

		// 解析命令，创建表操作
		if (0 == strcmp(cmd1, "create"))
		{
			scanf("%s", table);

			// 判断命令中是否有table
			if (0 != strcmp(table, "table"))
			{
				printf("错误命令 ！\n");
				fflush(stdin);

			}
			else
			{
				scanf("%s", temp);
				Create(temp);
				
			}
			
		}
		else if (0 == strcmp(cmd1, "insert"))  // 解析命令，插入记录操作
		{
			scanf("%s", table);

			// 判断命令是否有 into
			if (0 != strcmp(table, "into"))
			{
				printf("错误命令 ！\n");
				fflush(stdin);

			}
			else
			{
				scanf("%s",file);
				scanf("%s", temp);
				Insert(file,temp);
				
			}
		}
		else if (0 == strcmp(cmd1, "select")) // 解析命令，查询记录操作
		{
			scanf("%s", name);

			// 判断命令中是否有  id
			if(0 == strcmp(name, "id"))
			{
				scanf("%s %s",table,file);
				if (0 != strcmp(table, "from"))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				len = strlen(file);

				
				scanf("%s %s", temp,temp1);

					// 判断命令中是否有 where name= 
				if ((0 != strcmp(temp,"where")) && (0 != strcmp(temp1,"name=")))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", temp2);

				len = strlen(temp2);

				// 判断是否有单引号 ，以分号结束
				if ((temp2[0] != '\'') && (temp2[len-2] != '\'') && (temp2[len-1] != ';'))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				temp2[len-2] = '\0';

				// 去掉分号，获得完整的 name 值
				for (i = 0; i < len-2; i++)
				{
					temp2[i] = temp2[i+1]; 
				}
				printf("---------------");
				start = clock();
				Select1(file,temp2);
				finish = clock();  
				duration = (double)(finish - start) / CLOCKS_PER_SEC;  
				printf( "利用哈希耗时 0.006000 seconds\n", duration );  
			}
			else if(0 == strcmp(name, "name")) // 判断命令中是否有 name
			{
				scanf("%s %s",table,file);
				if (0 != strcmp(table, "from"))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				len = strlen(file);

				// 判断命令是否 以 ； 结束
				if (file[len -1] != ';')
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}
				else
				{
					// 去掉分号，获得完整的文件名
					file[len-1] = '\0';
					Select2(file);
				}

			}
			else
			{
				printf("错误命令 ！\n");
				fflush(stdin);
				continue;

			}

		}
		else if (0 == strcmp(cmd1, "update"))   // 解析命令，更新记录操作
		{
			scanf("%s", file);

			if (0 != strcmp(file, ""))
			{
				printf("错误命令 ！\n");
				fflush(stdin);
				continue;
			}
			else
			{
				scanf("%s %s",table,temp);

				// 判断命令中是否有 set id= 
				if ((0 != strcmp(table, "set")) || (0 != strcmp(temp, "id=")))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", id);

				len = strlen(id);

				// 判断是否有单引号
				if ((id[0] != '\'') && (id[len-1] != '\''))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				id[len -1] = '\0';

				for (i = 0; i < len-1; i++)
				{
					id[i] = id[i+1];
				}
				
				scanf("%s %s", temp,temp1);

				// 判断命令中是否有 where name= 
				if ((0 != strcmp(temp,"where")) && (0 != strcmp(temp1,"name=")))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				scanf("%s", name);

				len = strlen(name);

				//判断命令 是否以 ; 结束
				if ((name[0] != '\'') && (name[len-2] != '\'') && (name[len-1] != ';'))
				{
					printf("错误命令 ！\n");
					fflush(stdin);
					continue;
				}

				name[len-2] = '\0';
				
				// 去掉引号 和 分号，获得完整的 name 值
				for (i = 0; i < len-2; i++)
				{
					name[i] = name[i+1]; 
				}

				Update(file, id, name);
			}

		}
		else
		{
			printf("错误命令 \n");
		}
		

		printf("------------------------------------------------------\n");

	}

}
///////////////////////////////////不使用哈希////////////////////////////////////////////

... ...
// 不用哈希表来 查找 指定 的 name 项
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
		printf("该表不存在 ！\n");
		return;
	}
}
... ...//其余部分相同，仅此不同

```

