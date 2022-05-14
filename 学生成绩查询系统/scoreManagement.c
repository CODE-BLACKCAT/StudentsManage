#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

// �ɼ��ṹ��
typedef struct student
{
	int num; // ѧ��
	char name[11]; // ����
	int chinese;
	int math;
	int english;
	char profession[30]; // רҵ
	int rank; // ����
}Student; 

typedef struct Index // ���������ļ��ṹ
{
	int num;// ѧ��
	char name[11]; // ����
	char profession[30]; // רҵ
	int rank; // ����
	long offset;// ���ļ��еļ�¼��
}Index;

// ��ż�¼��
static int nameArray[100];
static int professionArray[100];

//�������� 
void showMenu(void);// �˵� 
void creatIdxFile(void);// ���������ļ�
void SortByNum(struct Index index[], int n);// ����ѧ������
void sortByName(struct Index index[], int n);// ������������
void sortByProfession(struct Index index[], int n);// ����רҵ����
void sortByRank(struct Index index[], int n);// ����רҵ����
void addStudent(int);// ¼��ɼ� 
void readIndexFile(Index idx[100], int* n);    //�������ļ����ݴ���idx������
int searchNum(Index idx[], int n, int no); //�ں���n����¼�������ļ�idx�в���ѧ��Ϊno�ļ�¼��Ӧ�ļ�¼��
void getByNum(void);// ѧ�Ų�ѯ 
void searchName(Index idx[], int n, char name[]);// �ں���n����¼�������ļ�idx�в�������Ϊname�ļ�¼��Ӧ�ļ�¼��
void getByName(void);// ������ѯ
void searchProfession(Index idx[], int n, char profession[]);// �ں���n����¼�������ļ�idx�в���רҵΪprofession�ļ�¼��Ӧ�ļ�¼��
void getByProfession(void);// רҵ��ѯ
void inquiry(void);// ��ѯ�ɼ� 
void showScendMenu(void);// �����˵�����ѯ�˵� 
void showScoreScendMenu(void); // �����˵�����ʾ�ɼ��Ĳ˵� 
void reviseScore(void);// �޸ĳɼ�   
void showScore(void);// �ɼ���ʾ 
void deleteScore(void);// ɾ���ɼ� 
void saveScore(void);// ����ɼ����˳� 
void showScoreByRank(void); // ��������ʾ�ɼ�
void showScoreByName(void);// ��������ʾ�ɼ�
void showScoreByNum(void);// ��ѧ����ʾ�ɼ�
void showScoreByProfession(void);// ��רҵ��ʾ�ɼ�
void calcuteRank(); // �������� 
//main���� 
int main()
{
	int flag, n = 0;//flag��־λ
	creatIdxFile();
	memset(nameArray, -1, 400);
	memset(professionArray, -1, 400);
	//---------------------------------//
	do
	{
		showMenu();
		scanf_s("%d", &flag);//ѡ����� 
		switch (flag)
		{
		case 1: printf("��������Ҫ¼���������");
			scanf_s("%d", &n);
			addStudent(n);
			calcuteRank();
			creatIdxFile();
			break;
		case 2: deleteScore(); calcuteRank();  creatIdxFile();  break;
		case 3: reviseScore(); creatIdxFile();  break;
		case 4: inquiry(); break;
		case 5: showScore(); break;
		case 0: saveScore(); break;
		default: printf("������������ԣ�\n"); int m = _getch();
		}
	} while (flag != 0);
	int m = _getch();
	return 0;
}

// ��ʾ�˵�
void showMenu()
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*               ��ӭʹ�óɼ�����ϵͳ                 *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1�� ¼��ɼ�                         *\n");
	printf("*               2�� ɾ���ɼ�                         *\n");
	printf("*               3�� �޸ĳɼ�                         *\n");
	printf("*               4�� �ɼ���ѯ                         *\n");
	printf("*               5�� ��ʾ�ɼ���Ϣ                     *\n");
	printf("*               0�� ������Ϣ���˳�ϵͳ               *\n");
	printf("*----------------------------------------------------*\n");
	printf("ѡ�����<0-6>                                         \n");
	return;
}

void SortByNum(Index index[], int n)
{
	int i, j;
	Index temp;
	for (i = 1; i < n; i++)
	{
		temp = index[i];
		j = i - 1;
		while (j >= 0 && temp.num < index[j].num)
		{
			index[j + 1] = index[j];    //���ؼ��ִ���R[i].key�ļ�¼����
			j--;
		}
		index[j + 1] = temp;        //��j+1������R[i]
	}
}
// ��������
void creatIdxFile()
{
	FILE* mfile, * idxfile;
	Index idx[100];
	Student st;
	int n = 0, i;
	if ((mfile = fopen("cj.txt", "r")) == NULL)
	{
		printf("  ��ʾ:���ܴ����ļ�\n");
		return;
	}
	if ((idxfile = fopen("index.txt", "w")) == NULL)
	{
		printf("  ��ʾ:���ܽ��������ļ�\n");
		return;
	}
	i = 0;
	while ((fread(&st, sizeof(Student), 1, mfile)))
	{
		idx[i].num = st.num;
		strcpy_s(idx[i].profession, 30, st.profession);
		strcpy_s(idx[i].name, 11, st.name);
		idx[i].rank = st.rank;
		idx[i].offset = ++n;
		i++;
	}
	rewind(idxfile);
	for (i = 0; i < n; i++)
		fwrite(&idx[i], sizeof(Index), 1, idxfile);
	fclose(mfile);
	fclose(idxfile);
	printf("  ��ʾ:�����ļ��������\n");
}
// ѧ��¼��
void addStudent(int n)
{
	Student stu[100];
	FILE* mfile;
	mfile = fopen("cj.txt", "a+");
	if (mfile == NULL)
	{
		printf("���ļ�ʧ��,�����ԣ�\n");
		int m = _getch();
		exit(0);
	}
	printf("������ȫ��ѧ����ѧ�ţ����������ĳɼ�����ѧ�ɼ���Ӣ��ɼ�, רҵ��\n");
	int i;
	for (i = 0; i < n; i++)
	{
		printf("----��%d��ѧ��----\n", i + 1);
		printf("ѧ��:");
		scanf_s("%d", &stu[i].num);

		printf("������");
		scanf_s("%s,", &stu[i].name, 11);

		printf("���ĳɼ���");
		scanf_s("%d", &stu[i].chinese);

		printf("��ѧ�ɼ���");
		scanf_s("%d", &stu[i].math);

		printf("Ӣ��ɼ���");
		scanf_s("%d", &stu[i].english);

		printf("רҵ��");
		scanf_s("%s", &stu[i].profession, 30);
		stu[i].rank = -1;
		// д������
		fwrite(&stu[i], sizeof(Student), 1, mfile);
	}
	fclose(mfile);
	printf("���ѧ���ɹ�,�밴���������\n");
	int m = _getch();
}
// ��ȡ��������
void readIndexFile(Index idx[100], int* n)
{
	int lengh;
	FILE* idxfile;
	if ((idxfile = fopen("index.txt", "r")) == NULL)
	{
		printf("  ��ʾ:�����ļ����ܴ�\n");
		return;
	}
	fseek(idxfile, 0, SEEK_END);
	lengh = ftell(idxfile);       //����ļ�����
	rewind(idxfile);
	*n = lengh / sizeof(Index);      //n����ļ��еļ�¼����
	fread(idx, sizeof(Index), *n, idxfile);
	fclose(idxfile);
}

int searchNum(Index idx[], int n, int num)
{
	int mid, low = 0, high = n - 1;
	while (low <= high)                   //���ֲ���
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
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("δ֪����\n");
		exit(0);
	}
	printf("����ѧ��:");
	scanf_s("%d", &num);
	SortByNum(idx, n);
	i = searchNum(idx, n, num);      //��idx�в���
	if (i == -1)
		printf("  ��ʾ:ѧ��%d������\n", num);
	else
	{
		fseek(mfile, (i - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fread(&st, sizeof(Student), 1, mfile);
		printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", st.num, st.name, st.chinese, st.math, st.english, st.profession, st.rank);
		printf("�����������\n");
		int m = _getch();
	}
	fclose(mfile);
}
// �ҵ�����0û���ҵ�����-1
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
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("  δ֪����\n");
		exit(-1);
	}
	printf("��������:");
	scanf_s("%s", name, 11);
	searchName(idx, n, name);      //��idx�в���
	if (nameArray[0] == -1)
		printf("  ��ʾ:����%s������\n", name);
	else
	{
		printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
		while (nameArray[ino] != -1)
		{
			fseek(mfile, (nameArray[ino]) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
			fread(&st, sizeof(Student), 1, mfile);
			printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", st.num, st.name, st.chinese, st.math, st.english, st.profession, st.rank);
			ino++;
		}
		memset(nameArray, -1, 400); // �������ֵȫ������Ϊ -1
		printf("�����������\n");
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
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("  δ֪����\n");
		exit(-1);
	}
	printf("����רҵ:");
	scanf_s("%s", profession, 30);
	searchProfession(idx, n, profession);      //��idx�в���
	if (professionArray[0] == -1) {
		printf("  ��ʾ:רҵ%s������\n", profession);
		printf("  �����������\n");
		int m = _getch();
	}
	else
	{
		printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
		while (professionArray[ino] != -1)
		{
			fseek(mfile, (professionArray[ino]) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
			fread(&st, sizeof(Student), 1, mfile);
			printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", st.num, st.name, st.chinese, st.math, st.english, st.profession, st.rank);
			ino++;
		}
		memset(professionArray, -1, 400); // �������ֵȫ������Ϊ -1
		printf("�����������\n");
		int m = _getch();
	}
	fclose(mfile);
}
// ��ѯ�Ķ����˵�
void showScendMenu()
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*                    �ɼ���ѯ�˵�                    *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1�� ��ѧ�Ų�ѯ                       *\n");
	printf("*               2�� ��������ѯ                       *\n");
	printf("*               3�� ��רҵ��ѯ                       *\n");
	printf("*               0�� �˳���ѯϵͳ��������ϵͳ         *\n");
	printf("*----------------------------------------------------*\n");
	printf("ѡ�����<0-3>                                         \n");
}
// �ɼ���ѯ
void inquiry()
{
	int k;
	do
	{
		showScendMenu();
		scanf_s("%d", &k);
		switch (k)
		{
		case 1:getByNum(); break;//ѧ�Ų�ѯ 
		case 2:getByName(); break;//������ѯ 
		case 3:getByProfession(); break;//רҵ��ѯ
		case 0:break;
		default:printf("�������������ѡ��\n"); int m = _getch();
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
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("δ֪����\n");
		exit(0);
	}
	printf("�����Ҫ�޸�ѧ����ѧ��\n");
	scanf_s("%d", &num);
	i = searchNum(idx, n, num);      //��idx�в���
	if (i == -1)
		printf("  ��ʾ:ѧ��%d������\n", num);
	else
	{
		Student stu;
		stu.num = idx[i-1].num;
		strcpy_s(stu.name, 11, idx[i-1].name);

		printf("�����ĳɼ���");
		scanf_s("%d", &stu.chinese);

		printf("����ѧ�ɼ���");
		scanf_s("%d", &stu.math);

		printf("��Ӣ��ɼ���");
		scanf_s("%d", &stu.english);

		strcpy_s(stu.profession, 30, idx[i-1].profession);
		
		fseek(mfile, (i - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fwrite(&stu, sizeof(Student), 1, mfile);
		calcuteRank();
		printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession, stu.rank);
		printf("�����������\n");
		int m = _getch();
	}
	fclose(mfile);
}
// ��ʾ�ɼ�
void showScore() 
{
	int k;
	do
	{
		showScoreScendMenu();
		scanf_s("%d", &k);
		switch (k)
		{
		case 1:showScoreByRank(); break;//�ɼ� 
		case 2:showScoreByName(); break;//����
		case 3:showScoreByProfession(); break;//רҵ
		case 4:showScoreByNum(); break;//ѧ��
		case 0:break;
		default:printf("�������������ѡ��\n"); int m = _getch();
		}
	} while (k != 0);
	return;
}
// ɾ����Ϣ
void deleteScore()
{
	FILE* mfile;
	Index idx[100];
	Student stu[100];
	int i, num;
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("δ֪����\n");
		exit(0);
	}
	printf("�����Ҫɾ��ѧ����ѧ��\n");
	scanf_s("%d", &num);
	i = searchNum(idx, n, num);
	if (i == -1)
		printf("  ��ʾ:ѧ��%d������\n", num);
	else
	{
		fread(stu, sizeof(Student), n, mfile);
		fclose(mfile);
		if ((mfile = fopen("cj.txt", "w+")) == NULL)
		{
			printf("  ��ʾ:���ļ���û���κμ�¼\n");
			return;
		}
		for (int k = i; k < n; ++k) {
			stu[k - 1].num = stu[k].num;
			strcpy_s(stu[k - 1].name, 11, stu[k].name);
			stu[k - 1].chinese = stu[k].chinese;
			stu[k - 1].math = stu[k].math;
			stu[k - 1].english = stu[k].english;
			strcpy_s(stu[k - 1].profession, 30, stu[k].profession);
			stu[k - 1].rank = stu[k].rank;
		}
		rewind(mfile);
		fwrite(stu, sizeof(Student), n - 1, mfile);
		printf("����ɣ������������\n");
		int m = _getch();
	}
	fclose(mfile);
}
// ����ɼ�
void saveScore()
{
	printf("��л����ʹ�ã���ӭ�´�ʹ��!\n");
	int m = _getch();
}
// �ɼ���ʾ�����˵�
void showScoreScendMenu(void)
{
	system("cls");
	printf("*----------------------------------------------------*\n");
	printf("*                    �ɼ���ʾ�˵�                    *\n");
	printf("*----------------------------------------------------*\n");
	printf("*               1�� ���ɼ�������ʾ                   *\n");
	printf("*               2�� ������������ʾ                   *\n");
	printf("*               3�� ��רҵ������ʾ                   *\n");
	printf("*               4�� ��ѧ��������ʾ                   *\n");
	printf("*               0�� �˳���ѯϵͳ��������ϵͳ         *\n");
	printf("*----------------------------------------------------*\n");
	printf("ѡ�����<0-3>                                         \n");
}

void showScoreByName(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("������\n");
		return;
	}
	sortByName(idx, n);
	printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession, stu.rank);
	}
	printf("�����������\n");
	int m = _getch();
	fclose(mfile);
}

void showScoreByNum(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("������\n");
		return;
	}
	SortByNum(idx, n);
	printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession, stu.rank);
	}
	printf("�����������\n");
	int m = _getch();
	fclose(mfile);
}

void sortByName(Index index[], int n)
{
	int i, j;
	Index temp;
	for (i = 1; i < n; i++)
	{
		temp = index[i];
		j = i - 1;
		while (j >= 0 && strcmp(temp.name, index[j].name) < 0)
		{
			index[j + 1] = index[j];  
			j--;
		}
		index[j + 1] = temp;     
	}
}

void sortByProfession(Index index[], int n)
{
	int i, j;
	Index temp;
	for (i = 1; i < n; i++)
	{
		temp = index[i];
		j = i - 1;
		while (j >= 0 && strcmp(temp.profession, index[j].profession) < 0)
		{
			index[j + 1] = index[j];
			j--;
		}
		index[j + 1] = temp;
	}
}

void sortByRank(Index index[], int n)
{
	int i, j;
	Index temp;
	for (i = 1; i < n; i++)
	{
		temp = index[i];
		j = i - 1;
		while (j >= 0 && temp.rank < index[j].rank)
		{
			index[j + 1] = index[j];
			j--;
		}
		index[j + 1] = temp;
	}
}

void showScoreByRank(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("������\n");
		return;
	}
	sortByRank(idx, n);
	printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession, stu.rank);
	}
	printf("�����������\n");
	int m = _getch();
	fclose(mfile);
}

void showScoreByProfession(void)
{
	FILE* mfile;
	Student stu;
	Index idx[100];
	int n;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:���ļ���û���κμ�¼\n");
		return;
	}
	readIndexFile(idx, &n);       //��ȡ��������idx
	if (n == 0) {
		printf("������\n");
		return;
	}
	sortByProfession(idx, n);
	printf("ѧ��\t����\t����\t��ѧ\tӢ��\tרҵ\t����\n");
	for (int i = 0; i < n; ++i) {
		fseek(mfile, (idx[i].offset - 1) * sizeof(Student), SEEK_SET); //�ɼ�¼��ֱ���������ļ��ж�Ӧ�ļ�¼
		fread(&stu, sizeof(Student), 1, mfile);
		printf("%d\t%s\t%d\t%d\t%d\t%s\t%d\n", stu.num, stu.name, stu.chinese, stu.math, stu.english, stu.profession, stu.rank);
	}
	printf("�����������\n");
	int m = _getch();
	fclose(mfile);
}

void calcuteRank()
{
	typedef struct RankNode
	{
		int score;
		int* address;
	}RankNode;
	RankNode rn[100];
	// ���һ���ж�������¼
	int lengh, n;
	Student st[100] = {0};
	FILE* mfile;
	if ((mfile = fopen("cj.txt", "r+")) == NULL)
	{
		printf("  ��ʾ:�����ļ����ܴ�\n");
		return;
	}
	fseek(mfile, 0, SEEK_END);
	lengh = ftell(mfile);       //����ļ�����
	rewind(mfile);
	n = lengh / sizeof(Student);      //n
	rewind(mfile);
	if (n < 100) {
		fread(st, sizeof(Student), n, mfile);
	}
	else {
		printf("δ֪����\n��");
		exit(-1);
	}
	// ����
	for (int i = 0; i < n; ++i) {
		rn[i].score = st[i].chinese + st[i].english + st[i].math;
		rn[i].address = &st[i].rank;
	}
	// ð������
	RankNode tmp;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n - i - 1; ++j) {
			if (rn[j].score < rn[j + 1].score) {
				tmp = rn[j];
				rn[j] = rn[j + 1];
				rn[j + 1] = tmp;
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		*(rn[i].address) = i + 1;
	}
	rewind(mfile);
	if (n < 100) {
		fwrite(st, sizeof(Student), n, mfile);
	}
	else {
		printf("δ֪����\n��");
		exit(-1);
	}
	fclose(mfile);
}