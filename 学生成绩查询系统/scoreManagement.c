#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

//成绩结构体
typedef struct student
{
	int num; // 学号
	char name[11]; // 姓名
	int chinese;
	int math;
	int english;
	char profession[30]; // 专业
}Student;

typedef struct Index // 定义索引文件结构
{
	int num;// 学号
	char name[11]; // 姓名
	char profession[30]; // 专业
	long offset;// 主文件中的记录号
}Index;

// 存放记录号
static int nameArray[100];
static int professionArray[100];

//函数声明 
void showMenu(void);// 菜单 
void creatIdxFile(void);// 建立索引文件
void insertSort(struct Index index[], int n);
void addStudent(int);// 录入成绩 
void readIndexFile(Index idx[100], int* n);    //读索引文件数据存入idx数组中
int searchNum(Index idx[], int n, int no); //在含有n个记录的索引文件idx中查找学号为no的记录对应的记录号
void getByNum(void);// 学号查询 
void searchName(Index idx[], int n, char name[]);// 在含有n个记录的索引文件idx中查找姓名为name的记录对应的记录号
void getByName(void);// 姓名查询
void searchProfession(Index idx[], int n, char profession[]);// 在含有n个记录的索引文件idx中查找专业为profession的记录对应的记录号
void getByProfession(void);// 专业查询
void inquiry(void);// 查询成绩 
void showScendMenu(void);// 二级菜单，查询菜单 
void showScoreScendMenu(void); // 二级菜单，显示成绩的菜单 
void reviseScore(void);// 修改成绩   
void showScore(void);// 成绩显示 
void deleteScore(void);// 删除成绩 
void saveScore(void);// 保存成绩并退出 
// void showScoreByRank(void); // 按排名显示成绩
void showScoreByName(void);// 按姓名显示成绩
void showScoreByNum(void);// 按学号显示成绩
// void showScoreByProfession(void);// 按专业显示成绩
//main函数 
int main()
{
	int flag, n = 0;//flag标志位
	creatIdxFile();
	memset(nameArray, -1, 400);
	memset(professionArray, -1, 400);
	//---------------------------------//
	do
	{
		showMenu();
		scanf_s("%d", &flag);//选择操作 
		switch (flag)
		{
		case 1: printf("请输入您要录入的人数：");
			scanf_s("%d", &n);
			addStudent(n);
			creatIdxFile();
			break;
		case 2: deleteScore(); creatIdxFile();  break;
		case 3: reviseScore(); creatIdxFile();  break;
		case 4: inquiry(); break;
		case 5: showScoreByNum(); break;
		case 0: saveScore(); break;
		default: printf("输入错误，请重试！\n"); int m = _getch();
		}
	} while (flag != 0);
	int m = _getch();
	return 0;
}

// 显示菜单
void showMenu()
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*               欢迎使用成绩管理系统                 *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1： 录入成绩                         *\n");
	printf("*               2： 删除成绩                         *\n");
	printf("*               3： 修改成绩                         *\n");
	printf("*               4： 成绩查询                         *\n");
	printf("*               5： 显示成绩信息                     *\n");
	printf("*               0： 保存信息并退出系统               *\n");
	printf("*----------------------------------------------------*\n");
	printf("选择操作<0-6>                                         \n");
	return;
}

void insertSort(Index index[], int n)
{
	int i, j;
	Index temp;
	for (i = 1; i < n; i++)
	{
		temp = index[i];
		j = i - 1;
		while (j >= 0 && temp.num < index[j].num)
		{
			index[j + 1] = index[j];    //将关键字大于R[i].key的记录后移
			j--;
		}
		index[j + 1] = temp;        //在j+1处插入R[i]
	}
}
// 创建索引
void creatIdxFile()
{
	FILE* mfile, * idxfile;
	Index idx[100];
	Student st;
	int n = 0, i;
	if ((mfile = fopen("cj.txt", "r")) == NULL)
	{
		printf("  提示:不能打开主文件\n");
		return;
	}
	if ((idxfile = fopen("index.txt", "w")) == NULL)
	{
		printf("  提示:不能建立索引文件\n");
		return;
	}
	i = 0;
	while ((fread(&st, sizeof(Student), 1, mfile)))
	{
		idx[i].num = st.num;
		strcpy_s(idx[i].profession, 30, st.profession);
		strcpy_s(idx[i].name, 11, st.name);
		idx[i].offset = ++n;
		i++;
	}
	// insertSort(idx, n);  //对idx数组按no值排序
	rewind(idxfile);
	for (i = 0; i < n; i++)
		fwrite(&idx[i], sizeof(Index), 1, idxfile);
	fclose(mfile);
	fclose(idxfile);
	printf("  提示:索引文件建立完毕\n");
}
// 学生录入
void addStudent(int n)
{
	Student stu[100];
	FILE* mfile;
	mfile = fopen("cj.txt", "a+");
	if (mfile == NULL)
	{
		printf("打开文件失败,请重试！\n");
		int m = _getch();
		exit(0);
	}
	printf("请输入全部学生的学号，姓名，语文成绩，数学成绩，英语成绩, 专业；\n");
	int i;
	for (i = 0; i < n; i++)
	{
		printf("----第%d个学生----\n", i + 1);
		printf("学号:");
		scanf_s("%d", &stu[i].num);

		printf("姓名：");
		scanf_s("%s,", &stu[i].name, 11);

		printf("语文成绩：");
		scanf_s("%d", &stu[i].chinese);

		printf("数学成绩：");
		scanf_s("%d", &stu[i].math);

		printf("英语成绩：");
		scanf_s("%d", &stu[i].english);

		printf("专业：");
		scanf_s("%s", &stu[i].profession, 30);
		//写入数据 
		fwrite(&stu[i], sizeof(Student), 1, mfile);
		
	}
	fclose(mfile);
	printf("添加学生成功,请按任意键返回\n");
	int m = _getch();
}
// 读取索引数组
void readIndexFile(Index idx[100], int* n)
{
	int lengh;
	FILE* idxfile;
	if ((idxfile = fopen("index.txt", "r")) == NULL)
	{
		printf("  提示:索引文件不能打开\n");
		return;
	}
	fseek(idxfile, 0, SEEK_END);
	lengh = ftell(idxfile);       //求出文件长度
	rewind(idxfile);
	*n = lengh / sizeof(Index);      //n求出文件中的记录个数
	fread(idx, sizeof(Index), *n, idxfile);
	fclose(idxfile);
}

int searchNum(Index idx[], int n, int num)
{
	int mid, low = 0, high = n - 1;
	while (low <= high)                   //二分查找
	{
		mid = (low + high) / 2;
		if (idx[mid].num > num)
			high = mid - 1;
		else if (idx[mid].num < num)
			low = mid + 1;
		else    
			return idx[mid].offset;
	}
	return -1;
}

void getByNum()
{
	FILE* mfile;
	Index idx[100];
	Student st;
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == NULL) {
		printf("未知错误\n");
		exit(0);
	}
	printf("输入学号:");
	scanf_s("%d", &num);
	insertSort(idx, n);
	i = searchNum(idx, n, num);      //在idx中查找
	if (i == -1)
		printf("  提示:学号%d不存在\n", num);
	else
	{
		fseek(mfile, (i - 1) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
		fread(&st, sizeof(Student), 1, mfile);
		printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
		printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", st.num, st.name, st.chinese, st.math, st.english, st.profession);
		printf("按任意键返回\n");
		int m = _getch();
	}
	fclose(mfile);
}
// 找到返回0没有找到返回-1
void searchName(Index idx[], int n, char name[])
{
	int index = 0;
	for (int i = 0; i < n; ++i) {
		if (strcmp(name, idx[i].name) == 0) {
			nameArray[index++] = i;
		}
	}
}

void getByName()
{
	FILE* mfile;
	Index idx[100];
	Student st;
	char name[11];
	int i = 0, ino = 0;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == NULL) {
		printf("  未知错误\n");
		exit(-1);
	}
	printf("输入姓名:");
	scanf_s("%s", name, 11);
	searchName(idx, n, name);      //在idx中查找
	if (nameArray[0] == -1)
		printf("  提示:姓名%s不存在\n", name);
	else
	{
		printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
		while (nameArray[ino] != -1)
		{
			fseek(mfile, (nameArray[ino]) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
			fread(&st, sizeof(Student), 1, mfile);
			printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", st.num, st.name, st.chinese, st.math, st.english, st.profession);
			ino++;
		}
		memset(nameArray, -1, 400); // 将数组的值全部设置为 -1
		printf("按任意键返回\n");
		int m = _getch();
	}
	fclose(mfile);
}

void searchProfession(Index idx[], int n, char profession[])
{
	int index = 0;
	for (int i = 0; i < n; ++i) {
		if (strcmp(profession, idx[i].profession) == 0) {
			professionArray[index++] = i;
		}
	}
}

void getByProfession()
{
	FILE* mfile;
	Index idx[100];
	Student st;
	char profession[30];
	int i = 0, ino = 0;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == NULL) {
		printf("  未知错误\n");
		exit(-1);
	}
	printf("输入专业:");
	scanf_s("%s", profession, 30);
	searchProfession(idx, n, profession);      //在idx中查找
	if (professionArray[0] == -1) {
		printf("  提示:专业%s不存在\n", profession);
		printf("  按任意键返回\n");
		int m = _getch();
	}
	else
	{
		printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
		while (professionArray[ino] != -1)
		{
			fseek(mfile, (professionArray[ino]) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
			fread(&st, sizeof(Student), 1, mfile);
			printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", st.num, st.name, st.chinese, st.math, st.english, st.profession);
			ino++;
		}
		memset(professionArray, -1, 400); // 将数组的值全部设置为 -1
		printf("按任意键返回\n");
		int m = _getch();
	}
	fclose(mfile);
}
// 查询的二级菜单
void showScendMenu()
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*                    成绩查询菜单                    *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1： 按学号查询                       *\n");
	printf("*               2： 按姓名查询                       *\n");
	printf("*               3： 按专业查询                       *\n");
	printf("*               0： 退出查询系统，返回主系统         *\n");
	printf("*----------------------------------------------------*\n");
	printf("选择操作<0-3>                                         \n");
}
// 成绩查询
void inquiry()
{
	int k;
	do
	{
		showScendMenu();
		scanf_s("%d", &k);
		switch (k)
		{
		case 1:getByNum(); break;//学号查询 
		case 2:getByName(); break;//姓名查询 
		case 3:getByProfession(); break;//专业查询
		case 0:break;
		default:printf("输入错误，请重新选择\n"); int m = _getch();
		}
	} while (k != 0);
	return;
}

void reviseScore() 
{
	FILE* mfile;
	Index idx[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == NULL) {
		printf("未知错误\n");
		exit(0);
	}
	printf("请输出要修改学生的学号\n");
	scanf_s("%d", &num);
	i = searchNum(idx, n, num);      //在idx中查找
	if (i == -1)
		printf("  提示:学号%d不存在\n", num);
	else
	{
		Student stu;
		stu.num = idx[i-1].num;
		strcpy_s(stu.name, 11, idx[i-1].name);

		printf("新语文成绩：");
		scanf_s("%d", &stu.chinese);

		printf("新数学成绩：");
		scanf_s("%d", &stu.math);

		printf("新英语成绩：");
		scanf_s("%d", &stu.english);

		strcpy_s(stu.profession, 30, idx[i-1].profession);
		
		fseek(mfile, (i - 1) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
		fwrite(&stu, sizeof(Student), 1, mfile);
		printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
		printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession);
		printf("按任意键返回\n");
		int m = _getch();
	}
	fclose(mfile);
}
// 显示成绩
void showScore() 
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == 0) {
		printf("无数据\n");
		return;
	}

	printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
	for (int i = 1; i <= n; ++i) {
		fseek(mfile, (i - 1) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession);
	}
	printf("按任意键返回\n");
	int m = _getch();
	fclose(mfile);
}
// 删除信息
void deleteScore()
{
	FILE* mfile;
	Index idx[100];
	Student stu[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == NULL) {
		printf("未知错误\n");
		exit(0);
	}
	printf("请输出要删除学生的学号\n");
	scanf_s("%d", &num);
	i = searchNum(idx, n, num);
	if (i == -1)
		printf("  提示:学号%d不存在\n", num);
	else
	{
		fread(stu, sizeof(Student), n, mfile);
		fclose(mfile);
		if ((mfile = fopen("cj.txt", "w+")) == NULL)
		{
			printf("  提示:主文件中没有任何记录\n");
			return;
		}
		for (int k = i; k < n; ++k) {
			stu[k - 1].num = stu[k].num;
			strcpy_s(stu[k - 1].name, 11, stu[k].name);
			stu[k - 1].chinese = stu[k].chinese;
			stu[k - 1].math = stu[k].math;
			stu[k - 1].english = stu[k].english;
			strcpy_s(stu[k - 1].profession, 30, stu[k].profession);
		}
		rewind(mfile);
		fwrite(stu, sizeof(Student), n - 1, mfile);
		printf("已完成，按任意键返回\n");
		int m = _getch();
	}
	fclose(mfile);
}
// 保存成绩
void saveScore()
{
	printf("感谢您的使用，欢迎下次使用!\n");
	int m = _getch();
}
// 成绩显示二级菜单
void showScoreScendMenu(void)
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*                    成绩显示菜单                    *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1： 按成绩降序显示                   *\n");
	printf("*               2： 按姓名降序显示                   *\n");
	printf("*               3： 按专业降序显示                   *\n");
	printf("*               4： 按学号降序显示                   *\n");
	printf("*               0： 退出查询系统，返回主系统         *\n");
	printf("*----------------------------------------------------*\n");
	printf("选择操作<0-3>                                         \n");
}
// 需要修改
void showScoreByName(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == 0) {
		printf("无数据\n");
		return;
	}
	
	printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession);
		printf("\n");
	}
	printf("按任意键返回\n");
	int m = _getch();
	fclose(mfile);
}

void showScoreByNum(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  提示:主文件中没有任何记录\n");
		return;
	}
	readIndexFile(idx, &n);       //读取索引数组idx
	if (n == 0) {
		printf("无数据\n");
		return;
	}
	insertSort(idx, n);
	printf("学号\t姓名\t语文\t数学\t英语\t专业\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset - 1) * sizeof(Student), SEEK_SET); //由记录号直接跳到主文件中对应的记录
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession);
	}
	printf("按任意键返回\n");
	int m = _getch();
	fclose(mfile);
}